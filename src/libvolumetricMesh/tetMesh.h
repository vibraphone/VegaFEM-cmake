/*************************************************************************
 *                                                                       *
 * Vega FEM Simulation Library Version 1.1                               *
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

/*
  This class is a container for a tetrahedral volumetric 3D mesh. See 
  also volumetricMesh.h. The tetrahedra can take arbitrary shapes (not 
  limited to only a few shapes).
*/

#ifndef _TETMESH_H_
#define _TETMESH_H_

#include "volumetricMesh.h"

// see also volumetricMesh.h for a description of the routines

class TetMesh : public VolumetricMesh
{
public:
  // loads the mesh from a text file 
  // (.veg input formut, see documentation and the provided examples)
  TetMesh(char * filename);

  // constructs a tet mesh from the given vertices and elements, 
  // with a single region and material ("E, nu" material)
  // "vertices" is double-precision array of length 3 x numVertices .
  // "elements" is an integer array of length 4 x numElements
  TetMesh(int numVertices, double * vertices,
         int numElements, int * elements,
         double E=1E6, double nu=0.45, double density=1000);

  // loads a file of a "special" (not .veg) type
  // currently one such special format is supported:
  // specialFileType=0: 
  //   the ".ele" and ".node" format, used by TetGen, 
  //   "filename" is the basename, e.g., passing "mesh" will load the mesh from "mesh.ele" and "mesh.node" 
  // default material parameters will be used
  TetMesh(char * filename, int specialFileType); 

  // creates a mesh consisting of the specified element subset of the given TetMesh
  TetMesh(const TetMesh & mesh, int numElements, int * elements, std::map<int,int> * vertexMap = NULL);

  TetMesh(const TetMesh & tetMesh);
  virtual VolumetricMesh * clone();
  virtual ~TetMesh();

  virtual int save(char * filename) const;

 // === misc queries ===

  static const VolumetricMesh::elementType elementType() { return elementType_; }
  virtual VolumetricMesh::elementType getElementType() const { return elementType(); }

  static double getTetVolume(Vec3d * a, Vec3d * b, Vec3d * c, Vec3d * d);
  static double getTetDeterminant(Vec3d * a, Vec3d * b, Vec3d * c, Vec3d * d);
  virtual double getElementVolume(int el) const;
  virtual void getElementInertiaTensor(int el, Mat3d & inertiaTensor) const;
  virtual void computeElementMassMatrix(int element, double * massMatrix) const;

  virtual bool containsVertex(int element, Vec3d pos) const; // true if given element contain given position, false otherwise

  // edge queries
  virtual int getNumElementEdges() const;
  virtual void getElementEdges(int el, int * edgeBuffer) const;

 // === interpolation ===

  virtual void computeBarycentricWeights(int el, Vec3d pos, double * weights) const;
  void computeGradient(int element, const double * U, int numFields, double * grad) const; // for tet meshes, gradient is constant inside each element, hence no need to specify position
  virtual void interpolateGradient(int element, const double * U, int numFields, Vec3d pos, double * grad) const; // conforms to the virtual function in the base class, "pos" does not affect the computation

protected:
  void computeElementMassMatrixHelper(Vec3d a, Vec3d b, Vec3d c, Vec3d d, double * buffer);
  static const VolumetricMesh::elementType elementType_;
  TetMesh(int numElementVertices): VolumetricMesh(numElementVertices) {}

  friend class VolumetricMeshExtensions;
};

#endif

