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

#include "volumetricMeshLoader.h"
#include "cubicMesh.h"
#include "tetMesh.h"

VolumetricMesh * VolumetricMeshLoader::load(char * filename, int verbose)
{
  VolumetricMesh::elementType elementType_ = VolumetricMesh::getElementType(filename);
  if (elementType_ == VolumetricMesh::INVALID)
  {
    return NULL;
  }

  VolumetricMesh * volumetricMesh = NULL;

  if (elementType_ == TetMesh::elementType())
    volumetricMesh = new TetMesh(filename, verbose); 

  if (elementType_ == CubicMesh::elementType())
    volumetricMesh = new CubicMesh(filename, verbose); 

  return volumetricMesh;
}

