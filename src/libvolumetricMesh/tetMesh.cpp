/*************************************************************************
 *                                                                       *
 * Vega FEM Simulation Library Version 1.0                               *
 *                                                                       *
 * "volumetricMesh" library , Copyright (C) 2007 CMU, 2009 MIT, 2012 USC *
 * All rights reserved.                                                  *
 *                                                                       *
 * Code author: Jernej Barbic                                            *
 * http://www.jernejbarbic.com/code                                      *
 *                                                                       *
 * Research: Jernej Barbic, Fun Shing Sin, Daniel Schroeder,             *
 *           Doug L. James, Jovan Popovic                                *
 *                                                                       *
 * Funding: National Science Foundation, Link Foundation,                *
 *          Singapore-MIT GAMBIT Game Lab,                               *
 *          Zumberge Research and Innovation Fund at USC                 *
 *                                                                       *
 * Version 3.0                                                           *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of the BSD-style license that is            *
 * included with this library in the file LICENSE.txt                    *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the file     *
 * LICENSE.TXT for more details.                                         *
 *                                                                       *
 *************************************************************************/

#include "tetMesh.h"
#include "volumetricMeshParser.h"

const VolumetricMesh::elementType TetMesh::elementType_ = TET;

TetMesh::TetMesh(char * filename) : VolumetricMesh(filename, 4, &temp)
{
  if (temp != elementType_)
  {
    printf("Error: mesh is not a tet mesh.\n");
    throw 11;
  }
}

TetMesh::TetMesh(char * filename, int specialFileType): VolumetricMesh(4)
{
  double E = 1E8;
  double nu = 0.45;
  double density = 1000;

  if (specialFileType != 0)
  {
    printf("Unknown special file type %d requested.\n", specialFileType);
    throw 1;
  }

  char lineBuffer[1024];
  VolumetricMeshParser parser;

  // first, read the nodes
  sprintf(lineBuffer, "%s.node", filename);
  if (parser.open(lineBuffer) != 0)
    throw 2;
  
  parser.getNextLine(lineBuffer, 1);
  int dim;
  sscanf(lineBuffer, "%d %d", &numVertices, &dim);
  if (dim != 3)
    throw 3;

  vertices = (Vec3d**) malloc (sizeof(Vec3d*) * numVertices);

  for(int i=0; i<numVertices; i++)
  {
    parser.getNextLine(lineBuffer, 1);
    int index;
    double x,y,z;
    sscanf(lineBuffer, "%d %lf %lf %lf", &index, &x, &y, &z);
    if (index != (i+1))
      throw 3;
    vertices[i] = new Vec3d(x,y,z);
  }
  
  parser.close();

  // next, read the elements
  sprintf(lineBuffer, "%s.ele", filename);
  if (parser.open(lineBuffer) != 0)
    throw 4;

  parser.getNextLine(lineBuffer, 1);
  sscanf(lineBuffer, "%d %d", &numElements, &dim);
  if (dim != 4)
  {
    printf("Error: not a tet mesh file (%d vertices per tet encountered).\n", dim);
    throw 5;
  }

  elements = (int**) malloc (sizeof(int*) * numElements);

  for(int i=0; i<numElements; i++)
  {
    parser.getNextLine(lineBuffer, 1);
    int index;
    int v[4];
    sscanf(lineBuffer, "%d %d %d %d %d", &index, &v[0], &v[1], &v[2], &v[3]);
    if (index != (i+1))
      throw 6;
    elements[i] = (int*) malloc (sizeof(int) * 4);
    for(int j=0; j<4; j++) // vertices are 1-indexed in .ele files
    {
      v[j]--;
      elements[i][j] = v[j]; 
    }
  }
  
  parser.close();

  numMaterials = 0;
  numSets = 0;
  numRegions = 0;
  materials = NULL;
  sets = NULL;
  regions = NULL;

  setSingleMaterial(E, nu, density);
}

TetMesh::TetMesh(int numVertices_, double * vertices_,
               int numElements_, int * elements_,
               double E, double nu, double density): VolumetricMesh(numVertices_, vertices_, numElements_, 4, elements_, E, nu, density) {}

TetMesh::TetMesh(const TetMesh & source): VolumetricMesh(source) {}

VolumetricMesh * TetMesh::clone()
{
  TetMesh * mesh = new TetMesh(*this);
  return mesh;
}

TetMesh::TetMesh(const TetMesh & tetMesh, int numElements_, int * elements_, map<int,int> * vertexMap_): VolumetricMesh(tetMesh, numElements_, elements_, vertexMap_) {}

TetMesh::~TetMesh() {}

int TetMesh::save(char * filename) const
{
  return VolumetricMesh::save(filename, elementType_);
}

void TetMesh::computeElementMassMatrix(int el, double * massMatrix) const
{
/*
  Consistent mass matrix of a tetrahedron =

                 [ 2  1  1  1  ]
                 [ 1  2  1  1  ]
     mass / 20 * [ 1  1  2  1  ]
                 [ 1  1  1  2  ]

  Note: mass = density * volume. Other than via the mass, the
  consistent mass matrix does not depend on the shape of the tetrahedron.
  (This can be seen after a long algebraic derivation; see:
   Singiresu S. Rao: The finite element method in engineering, 2004)
*/

  const double mtx[16] = { 2, 1, 1, 1, 
                           1, 2, 1, 1,
                           1, 1, 2, 1,
                           1, 1, 1, 2 } ;

  double density = getElementDensity(el);
  double factor = density * getElementVolume(el) / 20;

  for(int i=0; i<16; i++)
    massMatrix[i] = factor * mtx[i];

  // lumped mass
/*
  double mass = element(el)->density() * getElementVolume(el);
  massMatrix[0] = massMatrix[5] = massMatrix[10] = massMatrix[15] = mass / 4.0;
*/
}

double TetMesh::getTetVolume(Vec3d * a, Vec3d * b, Vec3d * c, Vec3d * d)
{
  // volume = 1/6 * | (a-d) . ((b-d) x (c-d)) |
  return (1.0 / 6 * fabs( dot(*a - *d, cross(*b - *d, *c - *d)) ));
}

double TetMesh::getElementVolume(int el) const
{
  Vec3d * a = getVertex(el, 0);
  Vec3d * b = getVertex(el, 1);
  Vec3d * c = getVertex(el, 2);
  Vec3d * d = getVertex(el, 3);
  return getTetVolume(a, b, c, d);
}

void TetMesh::getElementInertiaTensor(int el, Mat3d & inertiaTensor) const
{
  Vec3d a = *getVertex(el, 0);
  Vec3d b = *getVertex(el, 1);
  Vec3d c = *getVertex(el, 2);
  Vec3d d = *getVertex(el, 3);

  Vec3d center = getElementCenter(el);
  a -= center;
  b -= center;
  c -= center;
  d -= center;

  double absdetJ = 6.0 * getElementVolume(el);

  double x1 = a[0], x2 = b[0], x3 = c[0], x4 = d[0];
  double y1 = a[1], y2 = b[1], y3 = c[1], y4 = d[1];
  double z1 = a[2], z2 = b[2], z3 = c[2], z4 = d[2];

  double A = absdetJ * (y1*y1 + y1*y2 + y2*y2 + y1*y3 + y2*y3 + y3*y3 + y1*y4 + y2*y4 + y3*y4 + y4*y4 + z1*z1 + z1 * z2 + z2 * z2 + z1 * z3 + z2 * z3 + z3 * z3 + z1 * z4 + z2 * z4 + z3 * z4 + z4 * z4) / 60.0;

  double B = absdetJ * (x1*x1 + x1*x2 + x2*x2 + x1*x3 + x2*x3 + x3*x3 + x1*x4 + x2*x4 + x3*x4 + x4*x4 + z1*z1 + z1 * z2 + z2 * z2 + z1 * z3 + z2 * z3 + z3 * z3 + z1 * z4 + z2 * z4 + z3 * z4 + z4 * z4) / 60.0;

  double C = absdetJ * (x1*x1 + x1*x2 + x2*x2 + x1*x3 + x2*x3 + x3*x3 + x1*x4 + x2*x4 + x3*x4 + x4*x4 + y1*y1 + y1 * y2 + y2 * y2 + y1 * y3 + y2 * y3 + y3 * y3 + y1 * y4 + y2 * y4 + y3 * y4 + y4 * y4) / 60.0;

  double Ap = absdetJ * (2*y1*z1 + y2*z1 + y3*z1 + y4*z1 + y1*z2 + 2*y2*z2 + y3*z2 + y4*z2 + y1*z3 + y2*z3 + 2*y3*z3 + y4*z3 + y1*z4 + y2*z4 + y3*z4 + 2*y4*z4) / 120.0;

  double Bp = absdetJ * (2*x1*z1 + x2*z1 + x3*z1 + x4*z1 + x1*z2 + 2*x2*z2 + x3*z2 + x4*z2 + x1*z3 + x2*z3 + 2*x3*z3 + x4*z3 + x1*z4 + x2*z4 + x3*z4 + 2*x4*z4) / 120.0;

  double Cp = absdetJ * (2*x1*y1 + x2*y1 + x3*y1 + x4*y1 + x1*y2 + 2*x2*y2 + x3*y2 + x4*y2 + x1*y3 + x2*y3 + 2*x3*y3 + x4*y3 + x1*y4 + x2*y4 + x3*y4 + 2*x4*y4) / 120.0;

  inertiaTensor = Mat3d(A, -Bp, -Cp,   -Bp, B, -Ap,   -Cp, -Ap, C);
}

bool TetMesh::containsVertex(int el, Vec3d pos) const // true if given element contain given position, false otherwise
{
  double weights[4];
  computeBarycentricWeights(el, pos, weights);

  // all weights must be non-negative
  return ((weights[0] >= 0) && (weights[1] >= 0) && (weights[2] >= 0) && (weights[3] >= 0));
}

void TetMesh::computeBarycentricWeights(int el, Vec3d pos, double * weights) const
{
/*
       |x1 y1 z1 1|
  D0 = |x2 y2 z2 1|
       |x3 y3 z3 1|
       |x4 y4 z4 1|

       |x  y  z  1|
  D1 = |x2 y2 z2 1|
       |x3 y3 z3 1|
       |x4 y4 z4 1|

       |x1 y1 z1 1|
  D2 = |x  y  z  1|
       |x3 y3 z3 1|
       |x4 y4 z4 1|

       |x1 y1 z1 1|
  D3 = |x2 y2 z2 1|
       |x  y  z  1|
       |x4 y4 z4 1|

       |x1 y1 z1 1|
  D4 = |x2 y2 z2 1|
       |x3 y3 z3 1|
       |x  y  z  1|

  wi = Di / D0
*/

  Vec3d vtx[4];
  for(int i=0; i<4; i++)
    vtx[i] = *getVertex(el,i);

  double D[5];
  D[0] = getTetDeterminant(&vtx[0], &vtx[1], &vtx[2], &vtx[3]);

  for(int i=1; i<=4; i++)
  {
    Vec3d buf[4];
    for(int j=0; j<4; j++)
      buf[j] = vtx[j];
    buf[i-1] = pos;
    D[i] = getTetDeterminant(&buf[0], &buf[1], &buf[2], &buf[3]);
    weights[i-1] = D[i] / D[0];
  }
}

double TetMesh::getTetDeterminant(Vec3d * a, Vec3d * b, Vec3d * c, Vec3d * d)
{
  // computes the determinant of the 4x4 matrix
  // [ a 1 ]
  // [ b 1 ]
  // [ c 1 ]
  // [ d 1 ]

  Mat3d m0 = Mat3d(*b, *c, *d);
  Mat3d m1 = Mat3d(*a, *c, *d);
  Mat3d m2 = Mat3d(*a, *b, *d);
  Mat3d m3 = Mat3d(*a, *b, *c);

  return (-det(m0) + det(m1) - det(m2) + det(m3));
}

int TetMesh::getNumElementEdges() const
{
  return 6;
}

void TetMesh::getElementEdges(int el, int * edgeBuffer) const
{
  int v[4];
  for(int i=0; i<4; i++)
    v[i] = getVertexIndex(el,i);

  int edgeMask[6][2] = {
   { 0, 1 }, { 1, 2 }, { 2, 0 }, 
   { 0, 3 }, { 1, 3 }, { 2, 3 } };

  for(int edge=0; edge<6; edge++)
  {
    edgeBuffer[2*edge+0] = v[edgeMask[edge][0]];
    edgeBuffer[2*edge+1] = v[edgeMask[edge][1]];
  }
}

