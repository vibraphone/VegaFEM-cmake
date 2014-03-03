/*************************************************************************
 *                                                                       *
 * Vega FEM Simulation Library Version 2.0                               *
 *                                                                       *
 * "volumetricMesh" library , Copyright (C) 2007 CMU, 2009 MIT, 2013 USC *
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

#include <float.h>
#include <string.h>
#include <assert.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include "cubicMesh.h"
#include "matrixMacros.h"
#include "triple.h"
using namespace std;

const VolumetricMesh::elementType CubicMesh::elementType_ = CUBIC;

CubicMesh::CubicMesh(char * filename, int verbose) : VolumetricMesh(filename, 8, verbose, &temp)
{
  if (temp != elementType_)
  {
    printf("Error: mesh is not a cubic mesh.\n");
    throw 11;
  } 

  // set cube size
  cubeSize = len(*getVertex(0,1) - *getVertex(0,0));
}

CubicMesh::CubicMesh(int numVertices, double * vertices,
               int numElements, int * elements,
               double E, double nu, double density): VolumetricMesh(numVertices, vertices, numElements, 8, elements, E, nu, density)
{
  if (numElements > 0)
    cubeSize = len(*getVertex(0,1) - *getVertex(0,0));
  else
    cubeSize = 0.0;
}

CubicMesh::CubicMesh(int numVertices, double * vertices,
         int numElements, int * elements,
         int numMaterials, Material ** materials,
         int numSets, Set ** sets,
         int numRegions, Region ** regions): 
  VolumetricMesh(numVertices, vertices, numElements, 8, elements, numMaterials, materials, numSets, sets, numRegions, regions)
{
  if (numElements > 0)
    cubeSize = len(*getVertex(0,1) - *getVertex(0,0));
  else
    cubeSize = 0.0;
}

CubicMesh::CubicMesh(const CubicMesh & source): VolumetricMesh(source), cubeSize(source.cubeSize) {}

VolumetricMesh * CubicMesh::clone()
{
  CubicMesh * mesh = new CubicMesh(*this);
  return mesh;
}

CubicMesh * CubicMesh::createFromUniformGrid(int resolution, int numVoxels, int * voxels, double E, double nu, double density)
{
  int numElementVertices = 8;

  // create the indices of all vertices
  typedef triple<int,int,int> tripleIndex;
  set<tripleIndex> vertexSet;
  int vtxI[8] = { 0, 1, 1, 0, 0, 1, 1, 0 };
  int vtxJ[8] = { 0, 0, 1, 1, 0, 0, 1, 1 };
  int vtxK[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };
  for(int vox=0; vox<numVoxels; vox++)
  {
    int i = voxels[3*vox+0];
    int j = voxels[3*vox+1];
    int k = voxels[3*vox+2];
 
    for(int corner=0; corner<numElementVertices; corner++)
    {
      tripleIndex triIndex(i+vtxI[corner], j+vtxJ[corner], k+vtxK[corner]);
      vertexSet.insert(triIndex);
    }
  }

  int numVertices = (int)vertexSet.size();
  double * vertices = (double*) malloc (sizeof(double) * 3 * numVertices);
  int count = 0;
  map<tripleIndex, int> vertexMap;
  for(set<tripleIndex> :: iterator iter = vertexSet.begin(); iter != vertexSet.end(); iter++)
  {
    int i = iter->first;
    int j = iter->second;
    int k = iter->third;
    vertices[3*count+0] = -0.5 + 1.0 * i / resolution;
    vertices[3*count+1] = -0.5 + 1.0 * j / resolution;
    vertices[3*count+2] = -0.5 + 1.0 * k / resolution;
    vertexMap.insert(make_pair(tripleIndex(i,j,k), count));
    //printf("%d %d %d: %d\n", i,j,k, count);
    count++;
  }

  int numElements = numVoxels;

  int * elements = (int*) malloc (sizeof(int) * numElements * numElementVertices);
  for(int vox=0; vox<numElements; vox++)
  {
    int i = voxels[3*vox+0];
    int j = voxels[3*vox+1];
    int k = voxels[3*vox+2];
    for(int corner=0; corner<numElementVertices; corner++)
    {
      int I = i + vtxI[corner];
      int J = j + vtxJ[corner];
      int K = k + vtxK[corner];
      //printf("I=%d J=%d K=%d\n", I, J, K);

      // find I, J, K
      int vtxIndex = vertexMap[tripleIndex(I, J, K)];
      //printf("vtxIndex = %d\n", vtxIndex);
      elements[numElementVertices * vox + corner] = vtxIndex;
    }
  }

  CubicMesh * cubeMesh = new CubicMesh(numVertices, vertices, numElements, elements, E, nu, density);

  free(vertices);
  free(elements);

  return cubeMesh;
}

CubicMesh::CubicMesh(const CubicMesh & cubeMesh, int numElements, int * elements, map<int,int> * vertexMap_): VolumetricMesh(cubeMesh, numElements, elements, vertexMap_)
{
  cubeSize = cubeMesh.getCubeSize();
}

CubicMesh::~CubicMesh() {}

int CubicMesh::save(char * filename) const
{
  return VolumetricMesh::save(filename, elementType_);
}

bool CubicMesh::containsVertex(int element, Vec3d pos) const
{
  Vec3d bmin = *(getVertex(element, 0));
  Vec3d bmax = *(getVertex(element, 6));

  return (pos[0] >= bmin[0]) && (pos[1] >= bmin[1]) && (pos[2] >= bmin[2]) &&
         (pos[0] <= bmax[0]) && (pos[1] <= bmax[1]) && (pos[2] <= bmax[2]);
}

// interpolates given cubic mesh vertex 3D data to the destination locations
// vertexData size must a vector of length 3 * nElements
// destMatrix must be a vector of length 3 * numLocations
// if a given location does not belong to any element, extrapolation to the nearest element will be used
// vertices more than distanceThreshold away from any element vertex are assigned zero data
int CubicMesh::interpolateData(double * vertexData, int numInterpolationLocations, int r, double * interpolationLocations, double * destMatrix, double distanceThreshold) const
{
  assert(numElementVertices == 8);

  int numExternalVertices = 0;

  for (int i=0; i < numInterpolationLocations; i++) // over all interpolation locations
  {
    if (i % 100 == 0)
    {
      printf("%d ", i); fflush(NULL);
    }

    Vec3d pos = Vec3d(interpolationLocations[3*i+0], interpolationLocations[3*i+1], interpolationLocations[3*i+2]);

    // find element containing pos
    int element = getClosestElement(pos);

    if (!containsVertex(element, pos))
      numExternalVertices++;

    if (distanceThreshold > 0)
    {
      // check whether vertex is close enough to the cube mesh
      double minDistance = DBL_MAX;
      int assignedZero = 0;
      for(int ii=0; ii< numElementVertices; ii++)
      {
        Vec3d * vpos = getVertex(element, ii);
        if (len(*vpos-pos) < minDistance)
        {
          minDistance = len(*vpos-pos);
        }
      }

      if (minDistance > distanceThreshold)
      {
        // assign zero data
        for(int j=0; j<r; j++)
        {
          destMatrix[ELT(3*numInterpolationLocations, 3 * i + 0, j)] = 0;
          destMatrix[ELT(3*numInterpolationLocations, 3 * i + 1, j)] = 0;
          destMatrix[ELT(3*numInterpolationLocations, 3 * i + 2, j)] = 0;
        }
        assignedZero++;
        continue;
      }
    }

    // compute barycentric coordinates
    Vec3d w = pos - *(getVertex(element, 0));
    double alpha = w[0] / cubeSize;
    double beta = w[1] / cubeSize;
    double gamma = w[2] / cubeSize;

    double f000 = (1-alpha)*(1-beta)*(1-gamma);
    double f100 = (alpha)*(1-beta)*(1-gamma);
    double f110 = (alpha)*(beta)*(1-gamma);
    double f010 = (1-alpha)*(beta)*(1-gamma);

    double f001 = (1-alpha)*(1-beta)*(gamma);
    double f101 = (alpha)*(1-beta)*(gamma);
    double f111 = (alpha)*(beta)*(gamma);
    double f011 = (1-alpha)*(beta)*(gamma);

    int v000 = getVertexIndex(element, 0);
    int v100 = getVertexIndex(element, 1);
    int v110 = getVertexIndex(element, 2);
    int v010 = getVertexIndex(element, 3);
    int v001 = getVertexIndex(element, 4);
    int v101 = getVertexIndex(element, 5);
    int v111 = getVertexIndex(element, 6);
    int v011 = getVertexIndex(element, 7);

    for(int j=0; j<r; j++)
    {
      Vec3d data000 = Vec3d (vertexData[ELT(3*numVertices, 3 * v000 + 0, j)], vertexData[ELT(3*numVertices, 3 * v000 + 1, j)], vertexData[ELT(3*numVertices, 3 * v000 + 2, j)]);
      Vec3d data100 = Vec3d (vertexData[ELT(3*numVertices, 3 * v100 + 0, j)], vertexData[ELT(3*numVertices, 3 * v100 + 1, j)], vertexData[ELT(3*numVertices, 3 * v100 + 2, j)]);
      Vec3d data110 = Vec3d (vertexData[ELT(3*numVertices, 3 * v110 + 0, j)], vertexData[ELT(3*numVertices, 3 * v110 + 1, j)], vertexData[ELT(3*numVertices, 3 * v110 + 2, j)]);
      Vec3d data010 = Vec3d (vertexData[ELT(3*numVertices, 3 * v010 + 0, j)], vertexData[ELT(3*numVertices, 3 * v010 + 1, j)], vertexData[ELT(3*numVertices, 3 * v010 + 2, j)]);

      Vec3d data001 = Vec3d (vertexData[ELT(3*numVertices, 3 * v001 + 0, j)], vertexData[ELT(3*numVertices, 3 * v001 + 1, j)], vertexData[ELT(3*numVertices, 3 * v001 + 2, j)]);
      Vec3d data101 = Vec3d (vertexData[ELT(3*numVertices, 3 * v101 + 0, j)], vertexData[ELT(3*numVertices, 3 * v101 + 1, j)], vertexData[ELT(3*numVertices, 3 * v101 + 2, j)]);
      Vec3d data111 = Vec3d (vertexData[ELT(3*numVertices, 3 * v111 + 0, j)], vertexData[ELT(3*numVertices, 3 * v111 + 1, j)], vertexData[ELT(3*numVertices, 3 * v111 + 2, j)]);
      Vec3d data011 = Vec3d (vertexData[ELT(3*numVertices, 3 * v011 + 0, j)], vertexData[ELT(3*numVertices, 3 * v011 + 1, j)], vertexData[ELT(3*numVertices, 3 * v011 + 2, j)]);

      Vec3d interpolatedData = f000 * data000 + f100 * data100 + f110 * data110 + f010 * data010 +
                               f001 * data001 + f101 * data101 + f111 * data111 + f011 * data011 ;

      destMatrix[ELT(3*numInterpolationLocations,3*i+0,j)] = interpolatedData[0];
      destMatrix[ELT(3*numInterpolationLocations,3*i+1,j)] = interpolatedData[1];
      destMatrix[ELT(3*numInterpolationLocations,3*i+2,j)] = interpolatedData[2];
    }
  }

  printf("\n");

  return numExternalVertices;
}

// computes approximation to the normal correction for the given deformation
// vertexData size must a vector of length 3 * nElements
// normalCorrection (output) must be a vector of length 3 * numLocations
// staticNormals must be a vector of length 3 * numLocations
// vertices more than distanceThreshold away from any element vertex are assigned zero data
int CubicMesh::normalCorrection(double * vertexData, int numInterpolationLocations, int r, double * interpolationLocations, double * staticNormals, double * normalCorrection, double distanceThreshold) const
{
  assert(numElementVertices == 8);

  int numExternalVertices = 0;

  for (int i=0; i < numInterpolationLocations; i++) // over all interpolation locations
  {

    if (i % 100 == 0)
    {
      printf("%d ", i); fflush(NULL);
    }

    Vec3d pos = Vec3d(interpolationLocations[3*i+0], interpolationLocations[3*i+1], interpolationLocations[3*i+2]);

    // find element containing pos
    int element = getClosestElement(pos);

    if (!containsVertex(element, pos))
      numExternalVertices++;

    if (distanceThreshold > 0)
    {
      // check whether vertex is close enough to the cube mesh
      double minDistance = DBL_MAX;
      int assignedZero = 0;
      for(int ii=0; ii< numElementVertices; ii++)
      {
        Vec3d * vpos = getVertex(element, ii);
        if (len(*vpos-pos) < minDistance)
        {
          minDistance = len(*vpos-pos);
        }
      }

      if (minDistance > distanceThreshold)
      {
        // assign zero data
        for(int j=0; j<r; j++)
        {
          normalCorrection[ELT(3*numInterpolationLocations, 3 * i + 0, j)] = 0;
          normalCorrection[ELT(3*numInterpolationLocations, 3 * i + 1, j)] = 0;
          normalCorrection[ELT(3*numInterpolationLocations, 3 * i + 2, j)] = 0;
        }
        assignedZero++;

        continue;
      }
    }

    // compute barycentric coordinates
    Vec3d w = pos - *(getVertex(element, 0));
    double alpha = w[0] / cubeSize;
    double beta = w[1] / cubeSize;
    double gamma = w[2] / cubeSize;

    //double f000 = (1-alpha)*(1-beta)*(1-gamma);
    //double f100 = (alpha)*(1-beta)*(1-gamma);
    //double f110 = (alpha)*(beta)*(1-gamma);
    //double f010 = (1-alpha)*(beta)*(1-gamma);

    //double f001 = (1-alpha)*(1-beta)*(gamma);
    //double f101 = (alpha)*(1-beta)*(gamma);
    //double f111 = (alpha)*(beta)*(gamma);
    //double f011 = (1-alpha)*(beta)*(gamma);

    int v000 = getVertexIndex(element, 0);
    int v100 = getVertexIndex(element, 1);
    int v110 = getVertexIndex(element, 2);
    int v010 = getVertexIndex(element, 3);
    int v001 = getVertexIndex(element, 4);
    int v101 = getVertexIndex(element, 5);
    int v111 = getVertexIndex(element, 6);
    int v011 = getVertexIndex(element, 7);

    Vec3d gradf000(1.0 / cubeSize * -(1-beta)*(1-gamma), 1.0 / cubeSize * -(1-alpha)*(1-gamma), 1.0 / cubeSize * -(1-alpha)*(1-beta));
    Vec3d gradf100(1.0 / cubeSize * (1-beta)*(1-gamma), 1.0 / cubeSize * -alpha*(1-gamma), 1.0 / cubeSize * -alpha*(1-beta));
    Vec3d gradf110(1.0 / cubeSize * beta*(1-gamma), 1.0 / cubeSize * alpha*(1-gamma), 1.0 / cubeSize * -alpha*beta);
    Vec3d gradf010(1.0 / cubeSize * -beta*(1-gamma), 1.0 / cubeSize * (1-alpha)*(1-gamma), 1.0 / cubeSize * (1-alpha)*-beta);

    Vec3d gradf001(1.0 / cubeSize * -(1-beta)*gamma, 1.0 / cubeSize * -(1-alpha)*gamma, 1.0 / cubeSize * (1-alpha)*(1-beta));
    Vec3d gradf101(1.0 / cubeSize * (1-beta)*gamma, 1.0 / cubeSize * -alpha*gamma, 1.0 / cubeSize * alpha*(1-beta));
    Vec3d gradf111(1.0 / cubeSize * beta*gamma, 1.0 / cubeSize * alpha*gamma, 1.0 / cubeSize * alpha*beta);
    Vec3d gradf011(1.0 / cubeSize * -beta*gamma, 1.0 / cubeSize * (1-alpha)*gamma, 1.0 / cubeSize * (1-alpha)*beta);

    Vec3d normal = Vec3d(staticNormals[3*i+0], staticNormals[3*i+1], staticNormals[3*i+2]);

    for(int j=0; j<r; j++)
    {
      Vec3d u000 = Vec3d(vertexData[ELT(3*numVertices, 3 * v000 + 0, j)], vertexData[ELT(3*numVertices, 3 * v000 + 1, j)], vertexData[ELT(3*numVertices, 3 * v000 + 2, j)]);
      Vec3d u100 = Vec3d(vertexData[ELT(3*numVertices, 3 * v100 + 0, j)], vertexData[ELT(3*numVertices, 3 * v100 + 1, j)], vertexData[ELT(3*numVertices, 3 * v100 + 2, j)]);
      Vec3d u110 = Vec3d(vertexData[ELT(3*numVertices, 3 * v110 + 0, j)], vertexData[ELT(3*numVertices, 3 * v110 + 1, j)], vertexData[ELT(3*numVertices, 3 * v110 + 2, j)]);
      Vec3d u010 = Vec3d(vertexData[ELT(3*numVertices, 3 * v010 + 0, j)], vertexData[ELT(3*numVertices, 3 * v010 + 1, j)], vertexData[ELT(3*numVertices, 3 * v010 + 2, j)]);

      Vec3d u001 = Vec3d(vertexData[ELT(3*numVertices, 3 * v001 + 0, j)], vertexData[ELT(3*numVertices, 3 * v001 + 1, j)], vertexData[ELT(3*numVertices, 3 * v001 + 2, j)]);
      Vec3d u101 = Vec3d(vertexData[ELT(3*numVertices, 3 * v101 + 0, j)], vertexData[ELT(3*numVertices, 3 * v101 + 1, j)], vertexData[ELT(3*numVertices, 3 * v101 + 2, j)]);
      Vec3d u111 = Vec3d(vertexData[ELT(3*numVertices, 3 * v111 + 0, j)], vertexData[ELT(3*numVertices, 3 * v111 + 1, j)], vertexData[ELT(3*numVertices, 3 * v111 + 2, j)]);
      Vec3d u011 = Vec3d(vertexData[ELT(3*numVertices, 3 * v011 + 0, j)], vertexData[ELT(3*numVertices, 3 * v011 + 1, j)], vertexData[ELT(3*numVertices, 3 * v011 + 2, j)]);

      Vec3d coef(0,0,0);
      coef += dot(gradf000,normal) * u000;
      coef += dot(gradf100,normal) * u100;
      coef += dot(gradf110,normal) * u110;
      coef += dot(gradf010,normal) * u010;
      coef += dot(gradf001,normal) * u001;
      coef += dot(gradf101,normal) * u101;
      coef += dot(gradf111,normal) * u111;
      coef += dot(gradf011,normal) * u011;

      normalCorrection[ELT(3*numInterpolationLocations,3*i+0,j)] = coef[0];
      normalCorrection[ELT(3*numInterpolationLocations,3*i+1,j)] = coef[1];
      normalCorrection[ELT(3*numInterpolationLocations,3*i+2,j)] = coef[2];
    }
  }

  printf("\n");

  return numExternalVertices;
}

void CubicMesh::interpolateGradient(int element, const double * U, int numFields, Vec3d pos, double * grad) const
{
  // compute barycentric coordinates
  Vec3d w = pos - *(getVertex(element, 0));
  double alpha = w[0] / cubeSize;
  double beta = w[1] / cubeSize;
  double gamma = w[2] / cubeSize;

  //double f000 = (1-alpha)*(1-beta)*(1-gamma);
  //double f100 = (alpha)*(1-beta)*(1-gamma);
  //double f110 = (alpha)*(beta)*(1-gamma);
  //double f010 = (1-alpha)*(beta)*(1-gamma);

  //double f001 = (1-alpha)*(1-beta)*(gamma);
  //double f101 = (alpha)*(1-beta)*(gamma);
  //double f111 = (alpha)*(beta)*(gamma);
  //double f011 = (1-alpha)*(beta)*(gamma);

  int v000 = getVertexIndex(element, 0);
  int v100 = getVertexIndex(element, 1);
  int v110 = getVertexIndex(element, 2);
  int v010 = getVertexIndex(element, 3);
  int v001 = getVertexIndex(element, 4);
  int v101 = getVertexIndex(element, 5);
  int v111 = getVertexIndex(element, 6);
  int v011 = getVertexIndex(element, 7);

  Vec3d gradf000(1.0 / cubeSize * -(1-beta)*(1-gamma), 1.0 / cubeSize * -(1-alpha)*(1-gamma), 1.0 / cubeSize * -(1-alpha)*(1-beta));
  Vec3d gradf100(1.0 / cubeSize * (1-beta)*(1-gamma), 1.0 / cubeSize * -alpha*(1-gamma), 1.0 / cubeSize * -alpha*(1-beta));
  Vec3d gradf110(1.0 / cubeSize * beta*(1-gamma), 1.0 / cubeSize * alpha*(1-gamma), 1.0 / cubeSize * -alpha*beta);
  Vec3d gradf010(1.0 / cubeSize * -beta*(1-gamma), 1.0 / cubeSize * (1-alpha)*(1-gamma), 1.0 / cubeSize * (1-alpha)*-beta);

  Vec3d gradf001(1.0 / cubeSize * -(1-beta)*gamma, 1.0 / cubeSize * -(1-alpha)*gamma, 1.0 / cubeSize * (1-alpha)*(1-beta));
  Vec3d gradf101(1.0 / cubeSize * (1-beta)*gamma, 1.0 / cubeSize * -alpha*gamma, 1.0 / cubeSize * alpha*(1-beta));
  Vec3d gradf111(1.0 / cubeSize * beta*gamma, 1.0 / cubeSize * alpha*gamma, 1.0 / cubeSize * alpha*beta);
  Vec3d gradf011(1.0 / cubeSize * -beta*gamma, 1.0 / cubeSize * (1-alpha)*gamma, 1.0 / cubeSize * (1-alpha)*beta);

  for(int j=0; j<numFields; j++)
  {
    Vec3d u000 = Vec3d(U[ELT(3*numVertices, 3 * v000 + 0, j)], U[ELT(3*numVertices, 3 * v000 + 1, j)], U[ELT(3*numVertices, 3 * v000 + 2, j)]);
    Vec3d u100 = Vec3d(U[ELT(3*numVertices, 3 * v100 + 0, j)], U[ELT(3*numVertices, 3 * v100 + 1, j)], U[ELT(3*numVertices, 3 * v100 + 2, j)]);
    Vec3d u110 = Vec3d(U[ELT(3*numVertices, 3 * v110 + 0, j)], U[ELT(3*numVertices, 3 * v110 + 1, j)], U[ELT(3*numVertices, 3 * v110 + 2, j)]);
    Vec3d u010 = Vec3d(U[ELT(3*numVertices, 3 * v010 + 0, j)], U[ELT(3*numVertices, 3 * v010 + 1, j)], U[ELT(3*numVertices, 3 * v010 + 2, j)]);

    Vec3d u001 = Vec3d(U[ELT(3*numVertices, 3 * v001 + 0, j)], U[ELT(3*numVertices, 3 * v001 + 1, j)], U[ELT(3*numVertices, 3 * v001 + 2, j)]);
    Vec3d u101 = Vec3d(U[ELT(3*numVertices, 3 * v101 + 0, j)], U[ELT(3*numVertices, 3 * v101 + 1, j)], U[ELT(3*numVertices, 3 * v101 + 2, j)]);
    Vec3d u111 = Vec3d(U[ELT(3*numVertices, 3 * v111 + 0, j)], U[ELT(3*numVertices, 3 * v111 + 1, j)], U[ELT(3*numVertices, 3 * v111 + 2, j)]);
    Vec3d u011 = Vec3d(U[ELT(3*numVertices, 3 * v011 + 0, j)], U[ELT(3*numVertices, 3 * v011 + 1, j)], U[ELT(3*numVertices, 3 * v011 + 2, j)]);

    Mat3d FMode(0.0);
    FMode += tensorProduct(u000, gradf000);
    FMode += tensorProduct(u100, gradf100);
    FMode += tensorProduct(u110, gradf110);
    FMode += tensorProduct(u010, gradf010);
    FMode += tensorProduct(u001, gradf001);
    FMode += tensorProduct(u101, gradf101);
    FMode += tensorProduct(u111, gradf111);
    FMode += tensorProduct(u011, gradf011);

    for(int ii=0; ii<3; ii++)
      for(int jj=0; jj<3; jj++)
      {
        // 9 x r matrix
        grad[ELT(9, 3*ii+jj, j)] = FMode[ii][jj];
      }
  }
}

void CubicMesh::computeBarycentricWeights(int el, Vec3d pos, double * weights) const
{
  // compute barycentric coordinates
  Vec3d w = pos - *(getVertex(el, 0));
  double alpha = w[0] / cubeSize;
  double beta = w[1] / cubeSize;
  double gamma = w[2] / cubeSize;

  weights[0] = (1-alpha)*(1-beta)*(1-gamma); // f000
  weights[1] = (alpha)*(1-beta)*(1-gamma); // f100
  weights[2] = (alpha)*(beta)*(1-gamma); // f110
  weights[3] = (1-alpha)*(beta)*(1-gamma); // f010

  weights[4] = (1-alpha)*(1-beta)*(gamma); // f001
  weights[5] = (alpha)*(1-beta)*(gamma); // f101
  weights[6] = (alpha)*(beta)*(gamma); // f111
  weights[7] = (1-alpha)*(beta)*(gamma); // f011
}

double CubicMesh::getElementVolume(int el) const
{
  return cubeSize * cubeSize * cubeSize;
}

void CubicMesh::getElementInertiaTensor(int el, Mat3d & inertiaTensor) const
{
  // inertia tensor of a cube with side a, and mass m is spherical, with
  // diagonal elements 1.0 / 6 * m * a * a
  double cubeSize5 = cubeSize * cubeSize * cubeSize * cubeSize * cubeSize;

  inertiaTensor = cubeSize5 * Mat3d( 1.0/6 , 0 , 0, 0 , 1.0/6 , 0, 0 , 0 , 1.0/6 );
}

int CubicMesh::getNumElementEdges() const
{
  return 12;
}

void CubicMesh::getElementEdges(int el, int * edgeBuffer) const
{
  int v[8];
  for(int i=0; i<8; i++)
    v[i] = getVertexIndex(el,i);

  int edgeMask[12][2] = {
   { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
   { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
   { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 } } ;

  for(int edge=0; edge<12; edge++)
  {
    edgeBuffer[2*edge+0] = v[edgeMask[edge][0]];
    edgeBuffer[2*edge+1] = v[edgeMask[edge][1]];
  }
}

void CubicMesh::computeElementMassMatrix(int el, double * massMatrix) const
{
  double singleMassMatrix[64] =
  { 0.0370370335876942, 0.0185185167938471, 0.00925925839692354, 0.0185185167938471,
    0.0185185167938471, 0.00925925839692354, 0.00462962919846177, 0.00925925839692354,
    0.0185185167938471, 0.0370370335876942, 0.0185185167938471, 0.00925925839692354,
    0.00925925839692354, 0.0185185167938471, 0.00925925839692354, 0.00462962919846177,
    0.00925925839692354, 0.0185185167938471, 0.0370370335876942, 0.0185185167938471,
    0.00462962919846177, 0.00925925839692354, 0.0185185167938471, 0.00925925839692354,
    0.0185185167938471, 0.00925925839692354, 0.0185185167938471, 0.0370370335876942,
    0.00925925839692354, 0.00462962919846177, 0.00925925839692354, 0.0185185167938471,
    0.0185185167938471, 0.00925925839692354, 0.00462962919846177, 0.00925925839692354,
    0.0370370335876942, 0.0185185167938471, 0.00925925839692354, 0.0185185167938471,
    0.00925925839692354, 0.0185185167938471, 0.00925925839692354, 0.00462962919846177,
    0.0185185167938471, 0.0370370335876942, 0.0185185167938471, 0.00925925839692354,
    0.00462962919846177, 0.00925925839692354, 0.0185185167938471, 0.00925925839692354,
    0.00925925839692354, 0.0185185167938471, 0.0370370335876942, 0.0185185167938471,
    0.00925925839692354, 0.00462962919846177, 0.00925925839692354, 0.0185185167938471,
    0.0185185167938471, 0.00925925839692354, 0.0185185167938471, 0.0370370335876942
  };

  double density = getElementDensity(el);
  double voxelSpacing = getCubeSize();
  double voxelSpacing3 = voxelSpacing * voxelSpacing * voxelSpacing;
  double factor = density * voxelSpacing3;

  for(int i=0; i<64; i++)
    massMatrix[i] = factor * singleMassMatrix[i];
}

void CubicMesh::subdivide()
{
  int numNewElements = 8 * numElements; 
  int ** newElements = (int**) malloc (sizeof(int*) * numNewElements);

  int parentMask[8][3] = {
      { 0, 0, 0 },
      { 1, 0, 0 },
      { 1, 1, 0 },
      { 0, 1, 0 },
      { 0, 0, 1 },
      { 1, 0, 1 },
      { 1, 1, 1 },
      { 0, 1, 1 } };
  
  int mask[8][8][3];
  for(int el=0; el<8; el++)
    for(int vtx=0; vtx<8; vtx++)
      for(int dim=0; dim<3; dim++)
      {
        mask[el][vtx][dim] = parentMask[el][dim] + parentMask[vtx][dim];
        //printf("%d\n", mask[el][vtx][dim]);
      }

  vector<Vec3d> newVertices;
  for(int el=0; el<numElements; el++)
  {
    Vec3d * v0 = getVertex(el, 0);   

    // create the 8 children cubes
    for(int child=0; child<8; child++)
    {
      newElements[8 * el + child] = (int*) malloc (sizeof(int) * 8);

      int childVtx[8];
      for(int vtx=0; vtx<8; vtx++)
      {
        Vec3d pos = (*v0) + 0.5 * cubeSize * Vec3d(mask[child][vtx][0], mask[child][vtx][1], mask[child][vtx][2]);
        //printf("%G %G %G\n", pos[0], pos[1], pos[2]);
        // search for vertex
        int found = -1;
        for(int i=0; i<(int)newVertices.size(); i++)
        {
          if (len(pos - newVertices[i]) < 0.25 * cubeSize)
          {
            found = i;
            break;
          }
        }

        if (found == -1)
        {
          // new vertex
          newVertices.push_back(pos);
          found = (int)newVertices.size() - 1;
        }

        childVtx[vtx] = found;
        newElements[8 * el + child][vtx] = childVtx[vtx];
      }
    }
  }

  cubeSize *= 0.5;

  // deallocate old vertices
  for(int i=0; i<numVertices; i++)
    delete(vertices[i]);
  free(vertices);

  // copy new vertices into place
  numVertices = (int)newVertices.size();
  vertices = (Vec3d**) malloc (sizeof(Vec3d*) * numVertices);
  for(int i=0; i<numVertices; i++)
    vertices[i] = new Vec3d(newVertices[i]);

  // deallocate old elements
  for(int i=0; i<numElements; i++)
    free(elements[i]);
  free(elements);

  // copy new elements into place
  numElements = numNewElements;
  elements = newElements;

  // update sets (expand each entry in each set into 8 new entries)
  for(int setIndex=0; setIndex<numSets; setIndex++)
  {
    set<int> oldElements;
    sets[setIndex]->getElements(oldElements);
    Set * newSet = new Set(sets[setIndex]->getName());
    for(set<int> :: iterator iter = oldElements.begin(); iter != oldElements.end(); iter++)
    {
      for(int i=0; i<8; i++)
        newSet->insert(8 * *iter + i);
    }

    delete(sets[setIndex]);
    sets[setIndex] = newSet;
  }

  PropagateRegionsToElements();
}

