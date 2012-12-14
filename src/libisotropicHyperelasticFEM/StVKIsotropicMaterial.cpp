/*************************************************************************
 *                                                                       *
 * Vega FEM Simulation Library Version 1.1                               *
 *                                                                       *
 * "isotropic hyperelastic FEM" library , Copyright (C) 2012 USC         *
 * All rights reserved.                                                  *
 *                                                                       *
 * Code authors: Jernej Barbic, Fun Shing Sin                            *
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

#include <math.h>
#include "StVKIsotropicMaterial.h"
#include "volumetricMeshENuMaterial.h"

StVKIsotropicMaterial::StVKIsotropicMaterial(TetMesh * tetMesh) 
{
  int numElements = tetMesh->getNumElements();
  lambdaLame = (double*) malloc (sizeof(double) * numElements);
  muLame = (double*) malloc (sizeof(double) * numElements);

  for(int el=0; el<numElements; el++)
  {
    VolumetricMesh::Material * material = tetMesh->getElementMaterial(el);
    VolumetricMesh::ENuMaterial * eNuMaterial = downcastENuMaterial(material);
    if (eNuMaterial == NULL)
    {
      printf("Error: mesh does not consist of E, nu materials.\n");
      throw 1;
    }

    lambdaLame[el] = eNuMaterial->getLambda();
    muLame[el] = eNuMaterial->getMu();
  }
}

StVKIsotropicMaterial::~StVKIsotropicMaterial() 
{
  free(lambdaLame);
  free(muLame);
}

double StVKIsotropicMaterial::ComputeEnergy(int elementIndex, double * invariants)
{
  double IC = invariants[0];
  double IIC = invariants[1];
  //double IIIC = invariants[2]; // not needed for StVK

  double energy = 0.125 * lambdaLame[elementIndex] * (IC - 3.0) * (IC - 3.0) + 0.25 * muLame[elementIndex] * (IIC - 2.0 * IC + 3.0);
  return energy;
}

void StVKIsotropicMaterial::ComputeEnergyGradient(int elementIndex, double * invariants, double * gradient) // invariants and gradient are 3-vectors
{
  double IC = invariants[0];
  gradient[0] = 0.25 * lambdaLame[elementIndex] * (IC - 3.0) - 0.5 * muLame[elementIndex];
  gradient[1] = 0.25 * muLame[elementIndex];
  gradient[2] = 0.0;
}

void StVKIsotropicMaterial::ComputeEnergyHessian(int elementIndex, double * invariants, double * hessian) // invariants is a 3-vector, hessian is a 3x3 symmetric matrix, unrolled into a 6-vector, in the following order: (11, 12, 13, 22, 23, 33).
{
  // 11
  hessian[0] = 0.25 * lambdaLame[elementIndex];
  // 12
  hessian[1] = 0.0;
  // 13
  hessian[2] = 0.0;
  // 22
  hessian[3] = 0.0;
  // 23
  hessian[4] = 0.0;
  // 33
  hessian[5] = 0.0;
}

