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
#include "volumetricMeshParser.h"
#include "volumetricMesh.h"
#include "volumetricMeshENuMaterial.h"
#include "volumetricMeshMooneyRivlinMaterial.h"
using namespace std;

double VolumetricMesh::E_default = 1E9;
double VolumetricMesh::nu_default = 0.45;
double VolumetricMesh::density_default = 1000;

// parses the mesh, and returns the string corresponding to the element type
VolumetricMesh::VolumetricMesh(char * filename, int numElementVertices_, int verbose, elementType * elementType_): numElementVertices(numElementVertices_)
{
  if (verbose)
    printf("Opening file %s.\n", filename); fflush(NULL);

  // create buffer for element vertices
  int * v = (int*) malloc (sizeof(int) * numElementVertices);

  // parse the .veg file
  VolumetricMeshParser volumetricMeshParser;

  if (volumetricMeshParser.open(filename) != 0)
  {
    printf("Error: could not open file %s.\n",filename);
    free(v);
    throw 1;
  }

  // === First pass: parse vertices and elements, and count the number of materials, sets and regions  ===

  int countNumVertices = 0;
  int countNumElements = 0;

  numElements = -1;
  numMaterials = 0;
  numSets = 1; // set 0 is "allElements"
  numRegions = 0;
  *elementType_ = INVALID;
  int parseState = 0;
  char lineBuffer[1024];

  int oneIndexedVertices = 1;
  int oneIndexedElements = 1;
  while (volumetricMeshParser.getNextLine(lineBuffer, 0, 0) != NULL)
  {
    //lineBuffer now contains the next line
    //printf("%s\n", lineBuffer);

    // find *VERTICES
    if ((parseState == 0) && (strncmp(lineBuffer, "*VERTICES", 9) == 0))
    {
      parseState = 1;

      if (volumetricMeshParser.getNextLine(lineBuffer, 0, 0) != NULL)
      {
        // format is numVertices, 3, 0, 0
        sscanf(lineBuffer, "%d", &numVertices);  // ignore 3, 0, 0
        vertices = (Vec3d**) malloc (sizeof(Vec3d*) * numVertices);
      }
      else
      {
        printf("Error: file %s is not in the .veg format. Offending line:\n%s\n", filename, lineBuffer);
        free(v);
        throw 2;
      }
  
      continue;
    }

    // find *ELEMENTS
    if ((parseState == 1) && (strncmp(lineBuffer, "*ELEMENTS", 9) == 0))
    {
      parseState = 2;

      // parse element type
      if (volumetricMeshParser.getNextLine(lineBuffer) != NULL)
      {
        volumetricMeshParser.removeWhitespace(lineBuffer);

        if (strncmp(lineBuffer, "TET", 3) == 0)
          *elementType_ = TET;
        else if (strncmp(lineBuffer, "CUBIC", 5) == 0)
          *elementType_ = CUBIC;
        else
        {
          printf("Error: unknown mesh type %s in file %s\n", lineBuffer, filename);
          free(v);
          throw 3;
        }
      }
      else
      {
        printf("Error: file %s is not in the .veg format. Offending line:\n%s\n", filename, lineBuffer);
        free(v);
        throw 4;
      }

      // parse the number of elements
      if (volumetricMeshParser.getNextLine(lineBuffer, 0, 0) != NULL)
      {
        // format is: numElements, numElementVertices, 0
        sscanf(lineBuffer, "%d", &numElements);  // only use numElements; ignore numElementVertices, 0
        elements = (int**) malloc (sizeof(int*) * numElements);
      }
      else
      {
        printf("Error: file %s is not in the .veg format. Offending line:\n%s\n", filename, lineBuffer);
        free(v);
        throw 5;
      }

      continue;
    }

    if ((parseState == 2) && (lineBuffer[0] == '*'))
    {
      parseState = 3; // end of elements
    }

    if (parseState == 1)
    {
      // read the vertex position
      if (countNumVertices >= numVertices)
      {
        printf("Error: mismatch in the number of vertices in %s.\n", filename);
        free(v);
        throw 6;
      }

      // ignore space, comma or tab
      char * ch = lineBuffer;
      while((*ch == ' ') || (*ch == ',') || (*ch == '\t'))
        ch++;

      int index;
      sscanf(ch, "%d", &index);
      // seek next separator
      while((*ch != ' ') && (*ch != ',') && (*ch != '\t') && (*ch != 0))
        ch++;

      if (index == 0)
        oneIndexedVertices = 0; // input mesh has 0-indexed vertices

      double pos[3];
      for(int i=0; i<3; i++)
      {
        // ignore space, comma or tab
        while((*ch == ' ') || (*ch == ',') || (*ch == '\t'))
          ch++;

        if (*ch == 0)
        {
          printf("Error parsing line %s in file %s.\n", lineBuffer, filename);
          free(v);
          throw 7;
        }

        sscanf(ch, "%lf", &pos[i]);
 
        // seek next separator
        while((*ch != ' ') && (*ch != ',') && (*ch != '\t') && (*ch != 0))
          ch++;
      }

      vertices[countNumVertices] = new Vec3d(pos);
      countNumVertices++;
    }

    if (parseState == 2)
    {
      // read the element vertices
      if (countNumElements >= numElements)
      {
        printf("Error: mismatch in the number of elements in %s.\n", filename);
        throw 8;
      }

      // ignore space, comma or tab
      char * ch = lineBuffer;
      while((*ch == ' ') || (*ch == ',') || (*ch == '\t'))
        ch++;

      int index;
      sscanf(ch, "%d", &index);

      if (index == 0)
        oneIndexedElements = 0; // input mesh has 0-indexed elements

      // seek next separator
      while((*ch != ' ') && (*ch != ',') && (*ch != '\t') && (*ch != 0))
        ch++;

      for(int i=0; i<numElementVertices; i++)
      {
        // ignore space, comma or tab
        while((*ch == ' ') || (*ch == ',') || (*ch == '\t'))
          ch++;

        if (*ch == 0)
        {
          printf("Error parsing line %s in file %s.\n", lineBuffer, filename);
          free(v);
          throw 9;
        }

        sscanf(ch, "%d", &v[i]);

        // seek next separator
        while((*ch != ' ') && (*ch != ',') && (*ch != '\t') && (*ch != 0))
          ch++;
      }
      
      // if vertices were 1-numbered in the .veg file, convert to 0-numbered
      for (int k=0; k<numElementVertices; k++)
        v[k] -= oneIndexedVertices;

      elements[countNumElements] = (int*) malloc (sizeof(int) * numElementVertices);
      for(int j=0; j<numElementVertices; j++)
        elements[countNumElements][j] = v[j];

      countNumElements++;
    }

    if (strncmp(lineBuffer, "*MATERIAL", 9) == 0)
    {
      numMaterials++;
      continue;
    }

    if (strncmp(lineBuffer, "*SET", 4) == 0)
    {
      numSets++;
      continue;
    }

    if (strncmp(lineBuffer, "*REGION", 7) == 0)
    {
      numRegions++;
      continue;
    }
  }

  if (numElements < 0)
  {
    printf("Error: incorrect number of elements.  File %s may not be in the .veg format.\n", filename);
    throw 10;
  }

  // === Second pass: parse materials, sets and regions ===

  volumetricMeshParser.rewindToStart();

  if (verbose)
  {
    if (numMaterials == 0)
      printf("Warning: no materials encountered in %s.\n", filename);

    if (numRegions == 0)
      printf("Warning: no regions encountered in %s.\n", filename);
  }

  materials = (Material**) malloc (sizeof(Material*) * numMaterials);
  sets = (Set**) malloc (sizeof(Set*) * numSets);
  regions = (Region**) malloc (sizeof(Region*) * numRegions);

  // create the "allElements" set, containing all the elements
  sets[0] = new Set("allElements");
  for(int el=0; el<numElements; el++)
    sets[0]->insert(el);

  int countNumMaterials = 0;
  int countNumSets = 1; // set 0 is "allElements"
  int countNumRegions = 0;
  parseState = 0;

  while (volumetricMeshParser.getNextLine(lineBuffer, 0, 0) != NULL)
  {
    //printf("%s\n", lineBuffer);

     // exit comma separated set parseState upon the new command
    if ((parseState == 11) && (lineBuffer[0] == '*'))
      parseState = 0;

    // parse material

    if ((parseState == 0) && (strncmp(lineBuffer, "*MATERIAL", 9) == 0))
    {
      volumetricMeshParser.removeWhitespace(lineBuffer);

      // read material name
      char materialNameC[4096];
      strcpy(materialNameC, &lineBuffer[9]);

      // read the material type
      char materialType[4096];
      if (volumetricMeshParser.getNextLine(lineBuffer) != NULL)
      {
        volumetricMeshParser.removeWhitespace(lineBuffer);
        sscanf(lineBuffer, "%s", materialType);
      }
      else
      {
        printf("Error: incorrect material in file %s. Offending line:\n%s\n", filename, lineBuffer);
        free(v);
        throw 11;
      }

      if (strncmp(materialType, "ENU", 3) == 0)
      {
        // seek for first comma
        char * ch = lineBuffer;
        while((*ch != ',') && (*ch != 0))
          ch++;

        if (*ch == 0)
        {
          printf("Error parsing file %s. Offending line: %s.\n", filename, lineBuffer);
          free(v);
          throw 12;
        }

        ch++;

        // material specified by E, nu, density
        double density, E, nu;
        sscanf(ch, "%lf,%lf,%lf", &density, &E, &nu);

        if ((E > 0) && (nu > -1.0) && (nu < 0.5) && (density > 0))
        {
          // create new material
          materials[countNumMaterials] = new ENuMaterial(string(materialNameC), density, E, nu);
        }
        else
        {
          printf("Error: incorrect material specification in file %s. Offending line: %s\n", filename, lineBuffer);
          free(v);
          throw 13;
        }
      }
      else if (strncmp(materialType, "MOONEYRIVLIN", 12) == 0)
      {
        // seek for first comma
        char * ch = lineBuffer;
        while((*ch != ',') && (*ch != 0))
          ch++;

        if (*ch == 0)
        {
          printf("Error parsing file %s. Offending line: %s.\n", filename, lineBuffer);
          free(v);
          throw 14;
        }

        ch++;

        // mu01, m10, v1, density
        double density, mu01, mu10, v1;
        sscanf(ch, "%lf,%lf,%lf,%lf", &density, &mu01, &mu10, &v1);

        if (density > 0)
        {
          // create new material
          materials[countNumMaterials] = new MooneyRivlinMaterial(string(materialNameC), density, mu01, mu10, v1);
        }
        else
        {
          printf("Error: incorrect material specification in file %s. Offending line:\n%s\n", filename, lineBuffer);
          free(v);
          throw 15;
        }
      }
      else
      {
        printf("Error: incorrect material specification in file %s. Offending line:\n%s\n", filename, lineBuffer);
        free(v);
        throw 16;
      }

      countNumMaterials++;
    }

    // parse region

    if ((parseState == 0) && (strncmp(lineBuffer, "*REGION,", 7) == 0))
    {
      volumetricMeshParser.removeWhitespace(lineBuffer);

      char setNameC[4096];
      char materialNameC[4096];

      if (volumetricMeshParser.getNextLine(lineBuffer) != NULL)
      {
        volumetricMeshParser.removeWhitespace(lineBuffer);

        // format is set, material
        sscanf(lineBuffer, "%s,%s", setNameC, materialNameC);

        // seek for first comma
        char * ch = lineBuffer;
        while((*ch != ',') && (*ch != 0))
          ch++;

        if (*ch == 0)
        {
          printf("Error parsing file %s. Offending line: %s.\n", filename, lineBuffer);
          free(v);
          throw 17;
        }

        *ch = 0;
        strcpy(setNameC, lineBuffer);
        ch++;
        strcpy(materialNameC, ch);
      }
      else
      {
        printf("Error: file %s is not in the .veg format. Offending line:\n%s\n", filename, lineBuffer);
        free(v);
        throw 18;
      }

      // seek for setName
      int setNum = -1;
      for(int set=0; set < numSets; set++)
      {
        string name = sets[set]->getName();
        if (name == string(setNameC))
        {
          setNum = set;
          break;
        }
      }
      if (setNum == -1)
      {
        printf("Error: set %s not found among the sets.\n", setNameC);
        free(v);
        throw 19;
      }

      // seek for materialName
      int materialNum = -1;
      for(int material=0; material < numMaterials; material++)
      {
        string name = materials[material]->getName();
        if (name == string(materialNameC))
        {
          materialNum = material;
          break;
        }
      }
      if (materialNum == -1)
      {
        printf("Error: material %s not found among the materials.\n", materialNameC);
        free(v);
        throw 20;
      }

      regions[countNumRegions] = new Region(materialNum, setNum);
      countNumRegions++;
    }
   
    // parse set

    if ((parseState == 0) && (strncmp(lineBuffer, "*SET", 4) == 0))
    {
      volumetricMeshParser.removeWhitespace(lineBuffer);

      char setNameC[4096];
      strcpy(setNameC, &lineBuffer[4]);
      sets[countNumSets] = new Set(string(setNameC));
      countNumSets++;
      parseState = 11;
    }

    if (parseState == 11)
    {
      // we know that lineBuffer[0] != '*' (i.e., not end of the list) as that case was already previously handled

      volumetricMeshParser.removeWhitespace(lineBuffer);
      //printf("%s\n", lineBuffer);

      // parse the comma separated line
      char * pch;
      pch = strtok(lineBuffer, ",");
      while ((pch != NULL) && (isdigit(*pch)))
      {
        int newElement = atoi(pch);
        sets[countNumSets-1]->insert(newElement-oneIndexedElements); // sets are 0-indexed, but .veg files may be 1-indexed (oneIndexedElements == 1)
        pch = strtok (NULL, ",");
      }
    }
  }

  // === assign materials to elements ===

  elementMaterial = (int*) malloc (sizeof(int) * numElements);
  for(int el=0; el<numElements; el++)
    elementMaterial[el] = numMaterials;

  PropagateRegionsToElements();

  // seek for unassigned elements
  set<int> unassignedElements;
  for(int el=0; el<numElements; el++)
  {
    if (elementMaterial[el] == numMaterials)
      unassignedElements.insert(el);
  }

  if (unassignedElements.size() > 0)
  {
    // assign set and region for the unnassigned elements

    // create a material if none exists
    if (numMaterials == 0)
    {
      numMaterials++;
      materials = (Material**) realloc (materials, sizeof(Material*) * numMaterials);
      materials[numMaterials - 1] = new ENuMaterial("defaultMaterial", E_default, nu_default, density_default);
    }

    numSets++;
    sets = (Set**) realloc (sets, sizeof(Set*) * numSets);
    sets[numSets-1] = new Set("defaultSet"); 
    for(set<int>::iterator iter = unassignedElements.begin(); iter != unassignedElements.end(); iter++)
      sets[numSets-1]->insert(*iter); 

    numRegions++;
    regions = (Region**) realloc (regions, sizeof(Region*) * numRegions);
    regions[numRegions - 1] = new Region(numMaterials - 1, numSets - 1);

    if (verbose)
      printf("Warning: %d elements were not found in any of the regions. Using default material parameters for these elements.\n", (int)unassignedElements.size());
  }

  volumetricMeshParser.close();

  free(v);
}

VolumetricMesh::VolumetricMesh(int numVertices_, double * vertices_,
               int numElements_, int numElementVertices_, int * elements_,
               double E, double nu, double density): numElementVertices(numElementVertices_)
{
  numElements = numElements_;
  numVertices = numVertices_;

  numMaterials = 1;
  numSets = 1;
  numRegions = 1;

  vertices = (Vec3d**) malloc (sizeof(Vec3d*) * numVertices);
  elements = (int**) malloc (sizeof(int*) * numElements);
  elementMaterial = (int*) malloc (sizeof(int) * numElements);
  materials = (Material**) malloc (sizeof(Material*) * numMaterials);
  sets = (Set**) malloc (sizeof(Set*) * numSets);
  regions = (Region**) malloc (sizeof(Region*) * numRegions);

  for(int i=0; i<numVertices; i++)
    vertices[i] = new Vec3d(vertices_[3*i+0], vertices_[3*i+1], vertices_[3*i+2]);

  Material * material = new ENuMaterial("defaultMaterial", density, E, nu);
  materials[0] = material;

  Set * set = new Set("allElements");

  int * v = (int*) malloc (sizeof(int) * numElementVertices);
  for(int i=0; i<numElements; i++)
  {
    set->insert(i);
    elements[i] = (int*) malloc (sizeof(int) * numElementVertices);
    elementMaterial[i] = 0;
    for(int j=0; j<numElementVertices; j++)
    {
      v[j] = elements_[numElementVertices * i + j];
      elements[i][j] = v[j];
    }
  }
  free(v);

  sets[0] = set;
  Region * region = new Region(0, 0);
  regions[0] = region;
}

VolumetricMesh::VolumetricMesh(int numVertices_, double * vertices_,
         int numElements_, int numElementVertices_, int * elements_,
         int numMaterials_, Material ** materials_,
         int numSets_, Set ** sets_,
         int numRegions_, Region ** regions_): numElementVertices(numElementVertices_)
{
  numElements = numElements_;
  numVertices = numVertices_;

  numMaterials = numMaterials_;
  numSets = numSets_;
  numRegions = numRegions_;

  vertices = (Vec3d**) malloc (sizeof(Vec3d*) * numVertices);
  elements = (int**) malloc (sizeof(int*) * numElements);
  elementMaterial = (int*) malloc (sizeof(int) * numElements);
  materials = (Material**) malloc (sizeof(Material*) * numMaterials);
  sets = (Set**) malloc (sizeof(Set*) * numSets);
  regions = (Region**) malloc (sizeof(Region*) * numRegions);

  for(int i=0; i<numVertices; i++)
    vertices[i] = new Vec3d(vertices_[3*i+0], vertices_[3*i+1], vertices_[3*i+2]);

  int * v = (int*) malloc (sizeof(int) * numElementVertices);
  for(int i=0; i<numElements; i++)
  {
    elements[i] = (int*) malloc (sizeof(int) * numElementVertices);
    for(int j=0; j<numElementVertices; j++)
    {
      v[j] = elements_[numElementVertices * i + j];
      elements[i][j] = v[j];
    }
  }
  free(v);

  for(int i=0; i<numMaterials; i++)
    materials[i] = materials_[i]->clone();

  for(int i=0; i<numSets; i++)
    sets[i] = new Set(*(sets_[i]));

  for(int i=0; i<numRegions; i++)
    regions[i] = new Region(*(regions_[i]));

  // set elementMaterial:
  PropagateRegionsToElements();
}

VolumetricMesh::VolumetricMesh(const VolumetricMesh & volumetricMesh)
{
  numVertices = volumetricMesh.numVertices;
  vertices = (Vec3d**) malloc (sizeof(Vec3d*) * numVertices);
  for(int i=0; i<numVertices; i++)
    vertices[i] = new Vec3d(*(volumetricMesh.vertices[i]));

  numElementVertices = volumetricMesh.numElementVertices;
  numElements = volumetricMesh.numElements;
  elements = (int**) malloc (sizeof(int*) * numElements);
  for(int i=0; i<numElements; i++)
  {
    elements[i] = (int*) malloc (sizeof(int) * numElementVertices);
    for(int j=0; j<numElementVertices; j++)
      elements[i][j] = (volumetricMesh.elements)[i][j];
  }

  numMaterials = volumetricMesh.numMaterials;
  numSets = volumetricMesh.numSets;
  numRegions = volumetricMesh.numRegions;

  materials = (Material**) malloc (sizeof(Material*) * numMaterials);
  for(int i=0; i<numMaterials; i++)
    materials[i] = (volumetricMesh.materials)[i]->clone();

  sets = (Set**) malloc (sizeof(Set*) * numSets);
  for(int i=0; i<numSets; i++)
    sets[i] = new Set(*((volumetricMesh.sets)[i]));

  regions = (Region**) malloc (sizeof(Region*) * numRegions);
  for(int i=0; i<numRegions; i++)
    regions[i] = new Region((*(volumetricMesh.regions)[i]));

  elementMaterial = (int*) malloc (sizeof(int) * numElements);
  for(int i=0; i<numElements; i++)
    elementMaterial[i] = (volumetricMesh.elementMaterial)[i];
}

VolumetricMesh::~VolumetricMesh()
{
  for(int i=0; i< numVertices; i++)
    delete(vertices[i]);
  free(vertices);

  for(int i=0; i< numElements; i++)
    free(elements[i]);
  free(elements);

  for(int i=0; i< numMaterials; i++)
    delete(materials[i]);
  free(materials);
  
  for(int i=0; i< numSets; i++)
    delete(sets[i]);
  free(sets);

  for(int i=0; i< numRegions; i++)
    delete(regions[i]);
  free(regions);
}

int VolumetricMesh::save(char * filename, elementType elementType_) const // saves the mesh to a .veg file
{       
  FILE * fout = fopen(filename, "w");
  if (!fout)
  {       
    printf("Error: could not write to %s.\n",filename);
    return 1;
  }         

  fprintf(fout, "# Vega mesh file.\n");
  fprintf(fout, "# %d vertices, %d elements\n", numVertices, numElements);
  fprintf(fout, "\n");
          
  // write vertices
  fprintf(fout,"*VERTICES\n");
  fprintf(fout,"%d 3 0 0\n", numVertices);
          
  for(int i=0; i < numVertices; i++)  
  {
    Vec3d v = *getVertex(i);
    fprintf(fout,"%d %.15G %.15G %.15G\n", i+1, v[0], v[1], v[2]);
  }   
  fprintf(fout, "\n");

  // write elements
  fprintf(fout,"*ELEMENTS\n");

  char elementName[4096] = "INVALID";
  if (elementType_ == TET)
    strcpy(elementName, "TET");
  if (elementType_ == CUBIC)
    strcpy(elementName, "CUBIC");
  fprintf(fout,"%s\n", elementName);

  fprintf(fout,"%d %d 0\n", numElements, numElementVertices);

  for(int el=0; el < numElements; el++)
  {   
    fprintf(fout,"%d ", el+1);
    for(int j=0; j < numElementVertices; j++)
    {   
      fprintf(fout, "%d", getVertexIndex(el, j) + 1);
      if (j != numElementVertices - 1)
        fprintf(fout," ");
    } 
    fprintf(fout,"\n");
  }     
  fprintf(fout, "\n");

  // write materials
  for(int materialIndex=0; materialIndex < numMaterials; materialIndex++)
  {
    string name = materials[materialIndex]->getName();
    fprintf(fout, "*MATERIAL %s\n", name.c_str());

    if (materials[materialIndex]->getType() == Material::ENU)
    {
      ENuMaterial * material = downcastENuMaterial(materials[materialIndex]);
      double density = material->getDensity();
      double E = material->getE();
      double nu = material->getNu();
      fprintf(fout, "ENU, %.15G, %.15G, %.15G\n", density, E, nu);
    }

    if (materials[materialIndex]->getType() == Material::MOONEYRIVLIN)
    {
      MooneyRivlinMaterial * material = downcastMooneyRivlinMaterial(materials[materialIndex]);
      double density = material->getDensity();
      double mu01 = material->getmu01();
      double mu10 = material->getmu10();
      double v1 = material->getv1();
      fprintf(fout, "MOONEYRIVLIN, %.15G, %.15G, %.20G %.15G\n", density, mu01, mu10, v1);
    }
    fprintf(fout, "\n");
  }

  // write sets (skip the allElements set)
  for(int setIndex=1; setIndex < numSets; setIndex++)
  {
    string name = sets[setIndex]->getName();
    fprintf(fout, "*SET %s\n", name.c_str());
    set<int> setElements;
    sets[setIndex]->getElements(setElements);
    int count = 0;
    for(set<int>::iterator iter = setElements.begin(); iter != setElements.end(); iter++)
    {
      fprintf(fout, "%d, ", *iter + 1); // .veg files are 1-indexed
      count++;
      if (count == 8)
      {
        fprintf(fout, "\n");
        count = 0;
      }
    }
    if (count != 0)
      fprintf(fout, "\n");
    fprintf(fout, "\n");
  }

  // write regions
  for(int regionIndex=0; regionIndex < numRegions; regionIndex++)
  {
    int materialIndex = regions[regionIndex]->getMaterialIndex();
    int setIndex = regions[regionIndex]->getSetIndex();

    fprintf(fout, "*REGION\n");
    fprintf(fout, "%s, %s\n", sets[setIndex]->getName().c_str(), materials[materialIndex]->getName().c_str());
    fprintf(fout, "\n");
  }
        
  fclose(fout);
  return 0;
}   

VolumetricMesh::elementType VolumetricMesh::getElementType(char * filename) 
{
  //printf("Parsing %s... (for element type determination)\n",filename);fflush(NULL);
  elementType elementType_;

  // parse the .veg file
  VolumetricMeshParser volumetricMeshParser;
  elementType_ = INVALID;

  if (volumetricMeshParser.open(filename) != 0)
  {
    printf("Error: could not open file %s.\n",filename);
    return elementType_;
  }

  char lineBuffer[1024];
  while (volumetricMeshParser.getNextLine(lineBuffer, 0, 0) != NULL)
  {
    //printf("%s\n", lineBuffer);

    // seek for *ELEMENTS
    if (strncmp(lineBuffer, "*ELEMENTS", 9) == 0)
    {
      // parse element type
      if (volumetricMeshParser.getNextLine(lineBuffer) != NULL)
      {
        volumetricMeshParser.removeWhitespace(lineBuffer);

        if (strncmp(lineBuffer, "TET", 3) == 0)
          elementType_ = TET;
        else if (strncmp(lineBuffer, "CUBIC", 5) == 0)
          elementType_ = CUBIC;
        else
        {
          printf("Error: unknown mesh type %s in file %s\n", lineBuffer, filename);
          return elementType_;
        }
      }
      else
      {
        printf("Error (getElementType): file %s is not in the .veg format. Offending line:\n%s\n", filename, lineBuffer);
        return elementType_;
      }
    }
  }

  volumetricMeshParser.close();

  if (elementType_ == INVALID)
    printf("Error: could not determine the mesh type in file %s. File may not be in .veg format.\n", filename);

  return elementType_;
}

double VolumetricMesh::getVolume() const
{
  double vol = 0.0;
  for(int el=0; el<numElements; el++)
    vol += getElementVolume(el);
  return vol;
}

void VolumetricMesh::getVertexVolumes(double * vertexVolumes) const
{
  memset(vertexVolumes, 0, sizeof(double) * numVertices);
  double factor = 1.0 / numElementVertices;
  for(int el=0; el<numElements; el++)
  {
    double volume = getElementVolume(el);
    for(int j=0; j<numElementVertices; j++)
      vertexVolumes[getVertexIndex(el, j)] += factor * volume;
  }
}

Vec3d VolumetricMesh::getElementCenter(int el) const
{
  Vec3d pos(0,0,0);
  for(int i=0; i<numElementVertices; i++)
    pos += *getVertex(el,i);

  pos *= 1.0 / numElementVertices;

  return pos;
}

void VolumetricMesh::getVerticesInElements(vector<int> & elements_, vector<int> & vertices_) const
{
  set<int> ver;
  for(unsigned int i=0; i< elements_.size(); i++)
    for(int j=0; j< numElementVertices; j++)
      ver.insert(getVertexIndex(elements_[i],j));

  vertices_.clear();
  set<int>::iterator iter;
  for(iter = ver.begin(); iter != ver.end(); iter++)
    vertices_.push_back(*iter);
}

void VolumetricMesh::getElementsTouchingVertices(vector<int> & vertices_, vector<int> & elements_) const
{
  set<int> ver;
  for(unsigned int i=0; i<vertices_.size(); i++)
    ver.insert(vertices_[i]);

  elements_.clear();
  for(int i=0; i< numElements; i++)
  {
    set<int> :: iterator iter;
    for(int j=0; j<numElementVertices; j++)
    {
      iter = ver.find(getVertexIndex(i,j));
      if (iter != ver.end())
      {
        elements_.push_back(i);
        break;
      }
    }
  }
}

void VolumetricMesh::getVertexNeighborhood(vector<int> & vertices_, vector<int> & neighborhood) const
{
  vector<int> elements_;
  getElementsTouchingVertices(vertices_, elements_);
  getVerticesInElements(elements_, neighborhood);
}

void VolumetricMesh::getInertiaParameters(double & mass, Vec3d & centerOfMass, Mat3d & inertiaTensor) const
{
  mass = 0.0;
  centerOfMass[0] = centerOfMass[1] = centerOfMass[2] = 0;
  inertiaTensor[0][0] = inertiaTensor[0][1] = inertiaTensor[0][2] = 0;
  inertiaTensor[1][0] = inertiaTensor[1][1] = inertiaTensor[1][2] = 0;
  inertiaTensor[2][0] = inertiaTensor[2][1] = inertiaTensor[2][2] = 0;

  // compute mass, center of mass, inertia tensor
  for(int i=0; i< getNumRegions(); i++)
  {
    Region * region = getRegion(i);
    double density = getMaterial(region->getMaterialIndex())->getDensity();
    set<int> setElements; // elements in the region
    getSet(region->getSetIndex())->getElements(setElements);

    // over all elements in the region
    for(set<int> :: iterator iter = setElements.begin(); iter != setElements.end(); iter++)
    {
      int element = *iter;
      double elementVolume = getElementVolume(element);
      double elementMass = elementVolume * density;

      mass += elementMass;
      Vec3d elementCenter = getElementCenter(element);
      centerOfMass += elementMass * elementCenter;

      Mat3d elementITUnitDensity;
      getElementInertiaTensor(element, elementITUnitDensity);

      double a = elementCenter[0];
      double b = elementCenter[1];
      double c = elementCenter[2];

      Mat3d elementITCorrection
       (  b*b + c*c, -a*b, -a*c,
         -a*b, a*a + c*c, -b*c,
         -a*c, -b*c, a*a + b*b );

      Mat3d elementIT = density * elementITUnitDensity + elementMass * elementITCorrection;

      inertiaTensor += elementIT;
    }
  }

  //printf("final mass: %G\n",mass);
  centerOfMass /= mass;

  // correct inertia tensor so it's around the center of mass
  double a = centerOfMass[0];
  double b = centerOfMass[1];
  double c = centerOfMass[2];

  Mat3d correction
       ( b*b + c*c, -a*b, -a*c,
         -a*b, a*a + c*c, -b*c,
         -a*c, -b*c, a*a + b*b );

  inertiaTensor -= mass * correction;
}

void VolumetricMesh::getMeshGeometricParameters(Vec3d & centroid, double * radius) const
{
  // compute centroid
  centroid = Vec3d(0, 0, 0);
  for(int i=0; i < numVertices ; i++)
  {
    Vec3d * vertex = getVertex(i);
    centroid += *vertex;
  }

  centroid /= numVertices;

  // compute radius
  *radius = 0;
  for(int i=0; i < numVertices; i++)
  {
    Vec3d * vertex = getVertex(i);
    double dist = len(*vertex - centroid);
    if (dist > *radius)
      *radius = dist;
  }
}

int VolumetricMesh::getClosestVertex(Vec3d pos) const
{
  // linear scan
  double closestDist = DBL_MAX;
  int closestVertex = -1;

  for(int i=0; i<numVertices; i++)
  {
    Vec3d * vertexPosition = vertices[i];
    double dist = len(pos - *vertexPosition);
    if (dist < closestDist)
    {
      closestDist = dist;
      closestVertex = i;
    }
  }

  return closestVertex;
}

int VolumetricMesh::getClosestElement(Vec3d pos) const
{
  // linear scan
  double closestDist = DBL_MAX;
  int closestElement = 0;
  for(int element=0; element < numElements; element++)
  {
    Vec3d center = getElementCenter(element);
    double dist = len(pos - center);
    if (dist < closestDist)
    {
      closestDist = dist;
      closestElement = element;
    }
  }

  return closestElement;
}

int VolumetricMesh::getContainingElement(Vec3d pos) const
{
  // linear scan
  for(int element=0; element < numElements; element++)
  {
    if (containsVertex(element, pos))
      return element;
  }

  return -1;
}

void VolumetricMesh::setSingleMaterial(double E, double nu, double density)
{
  // erase previous materials
  for(int i=0; i<numMaterials; i++)
    delete(materials[i]);
  free(materials);

  for(int i=0; i<numSets; i++)
    delete(sets[i]);
  free(sets);

  for(int i=0; i<numRegions; i++)
    delete(regions[i]);
  free(regions);

  // add a single material
  numMaterials = 1;
  numSets = 1;
  numRegions = 1;

  materials = (Material**) malloc (sizeof(Material*) * numMaterials);
  sets = (Set**) malloc (sizeof(Set*) * numSets);
  regions = (Region**) malloc (sizeof(Region*) * numRegions);

  Material * material = new ENuMaterial("defaultMaterial", density, E, nu);
  materials[0] = material;

  Set * set = new Set("allElements");
  for(int i=0; i<numElements; i++)
  {
    set->insert(i);
    elementMaterial[i] = 0;
  }
  sets[0] = set;

  Region * region = new Region(0, 0);
  regions[0] = region;
}

void VolumetricMesh::getDefaultMaterial(double * E, double * nu, double * density)
{
  *E = E_default;
  *nu = nu_default;
  *density = density_default;
}

void VolumetricMesh::PropagateRegionsToElements()
{
  for(int regionIndex=0; regionIndex < numRegions; regionIndex++)
  {
    Region * region = regions[regionIndex];
    int materialIndex = region->getMaterialIndex();

    set<int> setElements;
    sets[region->getSetIndex()]->getElements(setElements);

    for(set<int> :: iterator iter = setElements.begin(); iter != setElements.end(); iter++)
    {
      int elt = *iter;
      elementMaterial[elt] = materialIndex;
    }
  }
}

int VolumetricMesh::generateInterpolationWeights(int numTargetLocations, 
	double * targetLocations, int ** vertices_, double ** weights, 
	double zeroThreshold, vector<int> * closestElementList, int verbose) const
{  
  // allocate interpolation arrays  
  *vertices_ = (int*) malloc (sizeof(int) * numElementVertices * numTargetLocations);
  *weights = (double*) malloc (sizeof(double) * numElementVertices * numTargetLocations);

  double * baryWeights = (double*) malloc (sizeof(double) * numElementVertices);

  int numExternalVertices = 0;

  if (closestElementList != NULL)
    closestElementList->clear();

  for (int i=0; i < numTargetLocations; i++) // over all interpolation locations
  {
    if ((verbose) && (i % 100 == 0))
    {
      printf("%d ", i); fflush(NULL);
    }

    Vec3d pos = Vec3d(targetLocations[3*i+0],
                      targetLocations[3*i+1],
                      targetLocations[3*i+2]);

    // find element containing pos
    int element = getContainingElement(pos);

    if (element < 0)
    {
      element = getClosestElement(pos);
      numExternalVertices++;
    }

    if (closestElementList != NULL)
      closestElementList->push_back(element);

    computeBarycentricWeights(element, pos, baryWeights);

    if (zeroThreshold > 0)
    {
      // check whether vertex is close enough to the mesh
      double minDistance = DBL_MAX;
      int numElementVertices = getNumElementVertices();
      int assignedZero = 0;
      for(int ii=0; ii< numElementVertices; ii++)
      {
        Vec3d * vpos = getVertex(element, ii);
        if (len(*vpos-pos) < minDistance)
        {
          minDistance = len(*vpos-pos);
        }
      }

      if (minDistance > zeroThreshold)
      {
        // assign zero weights
        for(int ii=0; ii < numElementVertices; ii++)
          baryWeights[ii] = 0.0;
        assignedZero++;
        continue;
      }
    }

    for(int ii=0; ii<numElementVertices; ii++)
    {
      (*vertices_)[numElementVertices * i + ii] = getVertexIndex(element, ii);
      (*weights)[numElementVertices * i + ii] = baryWeights[ii];
    }
  }

  free(baryWeights);

  return numExternalVertices;
}

int VolumetricMesh::getNumInterpolationElementVertices(char * filename)
{
  FILE * fin = fopen(filename, "r");
  if (!fin)
  {
    printf("Error: unable to open file %s.\n", filename);
    return -1;
  }

  char s[1024];
  if (fgets(s, 1024, fin) == NULL)
  {
    printf("Error: incorrect first line of file %s.\n", filename);
    return -2;
  }
  fclose(fin);

  VolumetricMeshParser::beautifyLine(s, 1);

  int slen = strlen(s);
  int count = 0;
  for(int i=0; i<slen; i++)
    if (s[i] == ' ')
      count++;

  if (count % 2 == 1)
  {
    printf("Error: odd number of whitespaces in the first line of file %s.\n", filename);
    return -3;
  }

  return count / 2;
}

int VolumetricMesh::loadInterpolationWeights(char * filename, int numTargetLocations, int numElementVertices_, int ** vertices_, double ** weights)
{
  FILE * fin = fopen(filename, "r");
  if (!fin)
  {
    printf("Error: unable to open file %s.\n", filename);
    return 2;
  }

  // allocate interpolation arrays
  *vertices_ = (int*) malloc (sizeof(int) * numElementVertices_ * numTargetLocations);
  *weights = (double*) malloc (sizeof(double) * numElementVertices_ * numTargetLocations);

  int numReadTargetLocations = -1;
  int currentVertex;

  // read the elements one by one and accumulate entries
  while (numReadTargetLocations < numTargetLocations-1)
  {
    numReadTargetLocations++;

    if (feof(fin))
    {
      printf("Error: interpolation file is too short. Num vertices in interp file: %d . Should be: %d .\n", numReadTargetLocations, numTargetLocations);
      return 1;
    }

    if (fscanf(fin, "%d", &currentVertex) < 1)
      printf("Warning: bad file syntax. Unable to read interpolation info.\n");

    if (currentVertex != numReadTargetLocations)
    {
      printf("Error: consecutive vertex index at position %d mismatch.\n", currentVertex);
      return 1;
    }

    for(int j=0; j<numElementVertices_; j++)
    {
      if (fscanf(fin,"%d %lf", &((*vertices_)[currentVertex * numElementVertices_ + j]), &((*weights)[currentVertex * numElementVertices_ + j]) ) < 2)
        printf("Warning: bad file syntax. Unable to read interpolation info.\n");
    }

    if (fscanf(fin,"\n") < 0)
    {
      //printf("Warning: bad file syntax. Missing end of line in the interpolation file.\n");
      //do nothing
      nop();
    }
  }

  fclose(fin);

  return 0;
}

int VolumetricMesh::saveInterpolationWeights(char * filename, int numTargetLocations, int numElementVertices_, int * vertices_, double * weights)
{
  FILE * fin = fopen(filename, "w");
  if (!fin)
  {
    printf("Error: unable to open file %s.\n", filename);
    return 1;
  }

  // read the elements one by one and accumulate entries
  for(int currentVertex=0; currentVertex < numTargetLocations; currentVertex++)
  {
    fprintf(fin, "%d", currentVertex);

    for(int j=0; j<numElementVertices_; j++)
      fprintf(fin," %d %lf", vertices_[currentVertex * numElementVertices_ + j], 
        weights[currentVertex * numElementVertices_ + j]);

    fprintf(fin,"\n");
  }

  fclose(fin);

  return 0;
}

void VolumetricMesh::interpolate(double * u, double * uTarget, 
  int numTargetLocations, int numElementVertices_, int * vertices_, double * weights)
{
  for(int i=0; i< numTargetLocations; i++)
  {
    Vec3d defo(0,0,0);
    for(int j=0; j<numElementVertices_; j++)
    {
      int volumetricMeshVertexIndex = vertices_[numElementVertices_ * i + j];
      Vec3d volumetricMeshVertexDefo = Vec3d(u[3*volumetricMeshVertexIndex+0], u[3*volumetricMeshVertexIndex+1], u[3*volumetricMeshVertexIndex+2]);
      defo += weights[numElementVertices_ * i + j] * volumetricMeshVertexDefo;
    }
    uTarget[3*i+0] = defo[0];
    uTarget[3*i+1] = defo[1];
    uTarget[3*i+2] = defo[2];
  }
}

int VolumetricMesh::interpolateGradient(const double * U, int numFields, Vec3d pos, double * grad) const
{
  // find the element containing "pos"
  int externalVertex = 0;
  int element = getContainingElement(pos);
  if (element < 0)
  {
    element = getClosestElement(pos);
    externalVertex = 1;
  }

  interpolateGradient(element, U, numFields, pos, grad);

  return externalVertex;
}

void VolumetricMesh::exportMeshGeometry(int * numVertices_, double ** vertices_, int * numElements_, int * numElementVertices_, int ** elements_) const
{
  *numVertices_ = numVertices;
  *numElements_ = numElements;
  *numElementVertices_ = numElementVertices;

  *vertices_ = (double*) malloc (sizeof(double) * 3 * numVertices);
  *elements_ = (int*) malloc (sizeof(int) * numElementVertices * numElements);

  for(int i=0; i<numVertices; i++)
  {
    Vec3d v = *getVertex(i);
    (*vertices_)[3*i+0] = v[0];
    (*vertices_)[3*i+1] = v[1];
    (*vertices_)[3*i+2] = v[2];
  }

  for(int i=0; i<numElements; i++)
  {
    for(int j=0; j<numElementVertices; j++)
      (*elements_)[numElementVertices * i + j] = elements[i][j];
  }
}

void VolumetricMesh::computeGravity(double * gravityForce, double g, bool addForce) const
{
  if (!addForce)
    memset(gravityForce, 0, sizeof(double) * 3 * numVertices);

  double invNumElementVertices = 1.0 / getNumElementVertices();

  for(int el=0; el < numElements; el++)
  {
    double volume = getElementVolume(el);
    double density = getElementDensity(el);
    double mass = density * volume;
    for(int j=0; j<getNumElementVertices(); j++)
      gravityForce[3 * getVertexIndex(el,j) + 1] -= invNumElementVertices * mass * g; // gravity assumed to act in negative y-direction
  }  
}

void VolumetricMesh::applyDeformation(double * u)
{
  for(int i=0; i<numVertices; i++)
  {
    Vec3d * v = getVertex(i);
    (*v)[0] += u[3*i+0];
    (*v)[1] += u[3*i+1];
    (*v)[2] += u[3*i+2];
  }
}

// transforms every vertex as X |--> pos + R * X
void VolumetricMesh::applyLinearTransformation(double * pos, double * R)
{
  for(int i=0; i<numVertices; i++)
  {
    Vec3d * v = getVertex(i);
    
    double newPos[3];
    for(int j=0; j<3; j++)
    {
      newPos[j] = pos[j];
      for(int k=0; k<3; k++)
        newPos[j] += R[3*j+k] * (*v)[k];
    }

    (*v)[0] = newPos[0];
    (*v)[1] = newPos[1];
    (*v)[2] = newPos[2];
  }
}

void VolumetricMesh::setMaterial(int i, const Material * material)
{
  delete(materials[i]);
  materials[i] = material->clone();
}

VolumetricMesh::VolumetricMesh(const VolumetricMesh & volumetricMesh, int numElements_, int * elements_, map<int,int> * vertexMap_)
{
  // determine vertices in the submesh
  numElementVertices = volumetricMesh.getNumElementVertices();
  set<int> vertexSet;
  for(int i=0; i<numElements_; i++)
    for(int j=0; j < numElementVertices; j++)
      vertexSet.insert(volumetricMesh.getVertexIndex(elements_[i],j));

  // copy vertices into place and also into vertexMap
  numVertices = vertexSet.size();
  vertices = (Vec3d**) malloc (sizeof(Vec3d*) * numVertices);
  set<int> :: iterator iter;
  int vertexNo = 0;
  map<int, int> vertexMap;
  for(iter = vertexSet.begin(); iter != vertexSet.end(); iter++)
  {
    vertices[vertexNo] = new Vec3d(*(volumetricMesh.getVertex(*iter)));
    vertexMap.insert(make_pair(*iter,vertexNo));
    vertexNo++;
  }

  if (vertexMap_ != NULL)
    *vertexMap_ = vertexMap;

  // copy elements
  numElements = numElements_;
  elements = (int**) malloc (sizeof(int*) * numElements);
  elementMaterial = (int*) malloc (sizeof(int) * numElements);
  map<int,int> elementMap;
  for(int i=0; i<numElements; i++)
  {
    elements[i] = (int*) malloc (sizeof(int) * numElementVertices);
    for(int j=0; j< numElementVertices; j++)
    {
      map<int,int> :: iterator iter2 = vertexMap.find((volumetricMesh.elements)[elements_[i]][j]);
      if (iter2 == vertexMap.end())
      {
        printf("Internal error 1.\n");
        exit(1);
      }
      elements[i][j] = iter2->second;
    }

    elementMaterial[i] = (volumetricMesh.elementMaterial)[elements_[i]];
    elementMap.insert(make_pair(elements_[i], i)); 
  }

  // copy materials
  numMaterials = volumetricMesh.getNumMaterials();
  numSets = volumetricMesh.getNumSets();
  numRegions = volumetricMesh.getNumRegions();

  materials = (Material**) malloc (sizeof(Material*) * numMaterials);
  for(int i=0; i < numMaterials; i++)
    materials[i] = volumetricMesh.getMaterial(i)->clone();

  // copy element sets; restrict element sets to the new mesh, also rename vertices to reflect new vertex indices
  vector<Set*> newSets;
  map<int,int> oldToNewSetIndex;
  for(int oldSetIndex=0; oldSetIndex < volumetricMesh.getNumSets(); oldSetIndex++)
  {
    Set * oldSet = volumetricMesh.getSet(oldSetIndex);
    set<int> oldElements;
    oldSet->getElements(oldElements);

    for(set<int> :: iterator iter = oldElements.begin(); iter != oldElements.end(); iter++)
    {
      if(*iter < 0)
      {
        printf("Internal error 2.\n");
        exit(1);
      }
    }

    // construct the element list
    vector<int> newElements;
    for(set<int> :: iterator iter = oldElements.begin(); iter != oldElements.end(); iter++)
    {
      map<int,int> :: iterator iter2 = elementMap.find(*iter);
      if (iter2 != elementMap.end())
        newElements.push_back(iter2->second);
    }

    // if there is at least one element in the new set, create a set for it
    if (newElements.size() > 0)
    {
      Set * newSet = new Set(oldSet->getName());
      for(unsigned int j=0; j<newElements.size(); j++)
      {
        if(newElements[j] < 0)
        {
          printf("Internal error 3.\n");
          exit(1);
        }
        newSet->insert(newElements[j]);
      }
      newSets.push_back(newSet);
      oldToNewSetIndex.insert(make_pair(oldSetIndex, newSets.size() - 1));
    }
  }

  numSets = newSets.size();
  sets = (Set**) malloc (sizeof(Set*) * numSets);
  for(int i=0; i<numSets; i++)
    sets[i] = newSets[i];

  //printf("numSets: %d\n", numSets);

  // copy regions; remove empty ones
  vector<Region*> vregions;
  for(int i=0; i < numRegions; i++)
  {
    Region * sregion = volumetricMesh.getRegion(i);
    map<int,int> :: iterator iter = oldToNewSetIndex.find(sregion->getSetIndex());
    if (iter != oldToNewSetIndex.end())
    {
      Region * newRegion = new Region(sregion->getMaterialIndex(),iter->second);
      vregions.push_back(newRegion);
    }
  }

  numRegions = vregions.size();
  regions = (Region**) malloc (sizeof(Region*) * numRegions);
  for(int j=0; j<numRegions; j++)
    regions[j] = vregions[j];

  // sanity check
  // seek each element in all the regions
  for(int el=0; el<numElements; el++)
  {
    int found = 0;
    for(int region=0; region < numRegions; region++)
    {
      int elementSet = (regions[region])->getSetIndex();

      // seek for element in elementSet
      if (sets[elementSet]->isMember(el))
      {
        if (found != 0)
          printf("Warning: element %d (1-indexed) is in more than one region.\n",el+1);
        else
          found = 1;
      }
    }
    if (found == 0)
      printf("Warning: element %d (1-indexed) is not in any of the regions.\n",el+1);
  }

  // sanity check: make sure all elements are between bounds
  for(int i=0; i < numSets; i++)
  {
    set<int> elts;
    sets[i]->getElements(elts);
    for(set<int> :: iterator iter = elts.begin(); iter != elts.end(); iter++)
    {
      if (*iter < 0)
        printf("Warning: encountered negative element index in element set %d.\n",i);
      if (*iter >= numElements)
        printf("Warning: encountered too large element index in element set %d.\n",i);
    }
  }
}

// if vertexMap is non-null, it also returns a renaming datastructure: vertexMap[big mesh vertex] is the vertex index in the subset mesh
void VolumetricMesh::setToSubsetMesh(std::set<int> & subsetElements, int removeIsolatedVertices, std::map<int,int> * vertexMap)
{
  int numRemovedElements = 0;
  for(int el=0; el<numElements; el++)
  {
    if (subsetElements.find(el) == subsetElements.end())
    {
      delete(elements[el]);
      elements[el] = NULL;
      numRemovedElements++;
    }
  }

  int head = 0;
  int tail = 0;

  int * lookupTable = (int *) malloc (sizeof(int) * numElements); 
  for(int i=0; i<numElements; i++)
    lookupTable[i] = i;

  while (tail < numElements)
  {
    if (elements[tail] != NULL)
    {
      elements[head] = elements[tail];
      lookupTable[tail] = head;  // update to new index 
      head++;
    }
    tail++;
  }
  numElements -= numRemovedElements;
  elements = (int**) realloc (elements, sizeof(int*) * numElements);

  for(int setIndex=0; setIndex < numSets; setIndex++)
  {
    set<int> setElements;
    sets[setIndex]->getElements(setElements);
    sets[setIndex]->clear();
    for(set<int>::iterator iter = setElements.begin(); iter != setElements.end(); iter++)
    {
      if (subsetElements.find(*iter) == subsetElements.end()) // not found!!
        continue;
      int newIndex = lookupTable[(*iter)];
      sets[setIndex]->insert(newIndex);
    }
  }
  free(lookupTable);

  if (removeIsolatedVertices)
  {
    set<int> retainedVertices;
    for(int el=0; el<numElements; el++)
      for(int j=0; j < numElementVertices; j++)
        retainedVertices.insert(getVertexIndex(el,j));

    int numRemovedVertices = 0;
    for(int v=0; v<numVertices; v++)
    {
      if (retainedVertices.find(v) == retainedVertices.end())
      {
        delete(vertices[v]);
        vertices[v] = NULL;
        numRemovedVertices++;
      }
    }
  
    int head = 0;
    int tail = 0;
  
    int * renamingFunction = (int*) malloc (sizeof(int) * numVertices);
    if (vertexMap != NULL)
      vertexMap->clear();
    while (tail < numVertices)
    {
      if (vertices[tail] != NULL)
      {
        renamingFunction[tail] = head;
        if (vertexMap != NULL)
          vertexMap->insert(make_pair(tail, head));
        vertices[head] = vertices[tail];
        head++;
      }
      tail++;
    }

    // rename vertices inside the elements
    for(int el=0; el<numElements; el++)
      for(int j=0; j < numElementVertices; j++)
        elements[el][j] = renamingFunction[getVertexIndex(el,j)];
   
    free(renamingFunction);
    numVertices -= numRemovedVertices;
    vertices = (Vec3d**) realloc (vertices, sizeof(Vec3d*) * numVertices );
  }
}

int VolumetricMesh::exportToEle(char * baseFilename, int includeRegions) const
{
  char s[1024];
  sprintf(s, "%s.ele", baseFilename);

  FILE * fout = fopen(s, "w");
  if (!fout)
  {       
    printf("Error: could not write to %s.\n",s);
    return 1;
  }         

  int * elementRegion = NULL;
  if (includeRegions)
  {
    elementRegion = (int*) malloc (sizeof(int) * getNumElements());
    for(int el=0; el<getNumElements(); el++)
    {
      int found = 0;
      for(int region=0; region < numRegions; region++)
      {
        int elementSet = (regions[region])->getSetIndex();

        // seek for element in elementSet
        if (sets[elementSet]->isMember(el))
        {
          if (found != 0)
            printf("Warning: element %d (1-indexed) is in more than one region.\n",el+1);
          else
            found = region+1;
        }
      }
      if (found == 0)
        printf("Warning: element %d (1-indexed) is not in any of the regions.\n",el+1);
      elementRegion[el] = found;
    }
  }

  if (includeRegions)
    fprintf(fout,"%d %d %d\n", numElements, numElementVertices, 1);
  else
    fprintf(fout,"%d %d %d\n", numElements, numElementVertices, 0);

  for(int el=0; el < numElements; el++)
  {   
    fprintf(fout,"%d ",el+1);
    for(int j=0; j < numElementVertices; j++)
    {   
      fprintf(fout,"%d", getVertexIndex(el,j)+1);
      if (j != numElementVertices - 1)
        fprintf(fout," ");
    } 
    if (includeRegions)
    {
      fprintf(fout," %d", elementRegion[el]);
    }
    fprintf(fout,"\n");
  }     
        
  fprintf(fout,"# generated by the volumetricMesh class\n");

  fclose(fout);

  if (includeRegions)
    free(elementRegion);

  sprintf(s, "%s.node", baseFilename);

  fout = fopen(s, "w");
  if (!fout)
  {       
    printf("Error: could not write to %s.\n",s);
    return 1;
  }         
          
  fprintf(fout,"%d %d %d %d\n", numVertices, 3, 0, 0);
  for(int v=0; v < numVertices; v++)
  {   
    fprintf(fout,"%d ",v+1);
    Vec3d vtx = *getVertex(v);
    fprintf(fout,"%.15f %.15f %.15f\n", vtx[0], vtx[1], vtx[2]);
  }     
        
  fprintf(fout,"# generated by the volumetricMesh class\n");

  fclose(fout);

  return 0;
}

void VolumetricMesh::nop()
{
}

