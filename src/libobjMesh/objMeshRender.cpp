/*************************************************************************
 *                                                                       *
 * Vega FEM Simulation Library Version 1.1                               *
 *                                                                       *
 * "objMesh" library , Copyright (C) 2007 CMU, 2009 MIT, 2012 USC        *
 * All rights reserved.                                                  *
 *                                                                       *
 * Code authors: Jernej Barbic, Christopher Twigg, Daniel Schroeder      *
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

#include <string.h>
using namespace std;
#include "objMeshRender.h"

// Renders the obj mesh.
// Written by Daniel Schroeder and Jernej Barbic, 2011

void ObjMeshRender::Texture::loadTexture(string fullPath, int textureMode_)
{
  printf("loading texture %s.\n", fullPath.c_str());
  int width, height;
  unsigned char * texData = ObjMeshRender::loadPPM(fullPath, &width, &height);
  if(texData == NULL)
  {
    printf("Warning: unable to load texture %s.\n", fullPath.c_str());
    return;
  }

  glEnable(GL_TEXTURE_2D);
  textureMode = textureMode_;
  texture.first = true;
  glGenTextures(1, &(texture.second));
  glBindTexture(GL_TEXTURE_2D, texture.second);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  if((textureMode & OBJMESHRENDER_MIPMAPBIT) == OBJMESHRENDER_GL_USEMIPMAP)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if((textureMode & OBJMESHRENDER_LIGHTINGMODULATIONBIT) == OBJMESHRENDER_GL_REPLACE)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  else
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  if((textureMode & OBJMESHRENDER_MIPMAPBIT) == OBJMESHRENDER_GL_NOMIPMAP)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
  else
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, texData);

  glDisable(GL_TEXTURE_2D);

  free(texData);
}

ObjMeshRender::ObjMeshRender(ObjMesh * mesh_) : mesh(mesh_) 
{
}

ObjMeshRender::~ObjMeshRender()
{
}

void ObjMeshRender::render(int geometryMode, int renderMode)
{
  bool warnMissingNormals = false;
  bool warnMissingFaceNormals = false;
  bool warnMissingTextureCoordinates = false;
  bool warnMissingTextures = false;
  //bool warnUnimplementedFlat = false;

  GLboolean lightingInitiallyEnabled = false;
  glGetBooleanv(GL_LIGHTING, &lightingInitiallyEnabled);

  // resolve conflicts in render mode settings and/or mesh data
  if((renderMode & OBJMESHRENDER_FLAT) && (renderMode & OBJMESHRENDER_SMOOTH))
  {
    printf("Requested both FLAT and SMOOTH rendering; SMOOTH used\n");
    renderMode &= ~OBJMESHRENDER_FLAT;
  }

  if((renderMode & OBJMESHRENDER_COLOR) && (renderMode & OBJMESHRENDER_MATERIAL))
  {
    printf("Requested both COLOR and MATERIAL rendering; MATERIAL used\n");
    renderMode &= ~OBJMESHRENDER_COLOR;
  }

  if(renderMode & OBJMESHRENDER_COLOR)
    glEnable(GL_COLOR_MATERIAL);
  else if(renderMode & OBJMESHRENDER_MATERIAL)
    glDisable(GL_COLOR_MATERIAL);

  if(renderMode & OBJMESHRENDER_CUSTOMCOLOR)
    glDisable(GL_LIGHTING);

  // render triangles
  if(geometryMode & OBJMESHRENDER_TRIANGLES)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(geometryMode & (OBJMESHRENDER_EDGES | OBJMESHRENDER_VERTICES))
    {
      //glEnable(GL_POLYGON_OFFSET_FILL);
      //glPolygonOffset(2.0, 2.0);
    }
    for(unsigned int i=0; i < mesh->getNumGroups(); i++)
    {
      const ObjMesh::Group * groupHandle = mesh->getGroupHandle(i);
      // set material
      const ObjMesh::Material * materialHandle = mesh->getMaterialHandle(groupHandle->getMaterialIndex());
      //printf("Material: %d\n",group.materialIndex());
      Vec3d Ka = materialHandle->getKa();
      Vec3d Kd = materialHandle->getKd();
      Vec3d Ks = materialHandle->getKs();
      float shininess = materialHandle->getShininess();
      float alpha = materialHandle->getAlpha();
      float ambient[4] = { Ka[0], Ka[1], Ka[2], alpha };
      float diffuse[4] = { Kd[0], Kd[1], Kd[2], alpha };
      float specular[4] = { Ks[0], Ks[1], Ks[2], alpha };
      if(renderMode & OBJMESHRENDER_MATERIAL)
      {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
      }
      else if(renderMode & OBJMESHRENDER_COLOR)
      {
        glColor3fv(diffuse);
      }

      if((renderMode & OBJMESHRENDER_TEXTURE) && materialHandle->hasTextureFilename())
      {
        if(groupHandle->getMaterialIndex() >= textures.size())
        {
          //textures are out of date
          warnMissingTextures = true;
        }
        else if (!textures[groupHandle->getMaterialIndex()].hasTexture())
          warnMissingTextures = true;
        else
        {
          Texture * textureHandle = &(textures[groupHandle->getMaterialIndex()]);
          glBindTexture(GL_TEXTURE_2D, textureHandle->getTexture());
          glEnable(GL_TEXTURE_2D);
          if((textureHandle->getTextureMode() & OBJMESHRENDER_LIGHTINGMODULATIONBIT) == OBJMESHRENDER_GL_REPLACE)
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
          else
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          glEnable(GL_TEXTURE_2D);
        }
      }
  
      //printf("amb: %G %G %G\n", Ka[0], Ka[1], Ka[2]);
      //printf("dif: %G %G %G\n", Kd[0], Kd[1], Kd[2]);
      //printf("spe: %G %G %G\n", Ks[0], Ks[1], Ks[2]);
  
      for( unsigned int iFace = 0; iFace < groupHandle->getNumFaces(); iFace++ )
      {
        const ObjMesh::Face * faceHandle = groupHandle->getFaceHandle(iFace);
  
        glBegin(GL_POLYGON);
        if(renderMode & OBJMESHRENDER_FLAT)
        {
          if(faceHandle->hasFaceNormal())
          {
  	    Vec3d fnormal = faceHandle->getFaceNormal();
  	    glNormal3d(fnormal[0],fnormal[1],fnormal[2]);
          }
          else
            warnMissingFaceNormals = true;
        }
  
        for( unsigned int iVertex = 0; iVertex < faceHandle->getNumVertices(); iVertex++ )
        {
          const ObjMesh::Vertex * vertexHandle = faceHandle->getVertexHandle(iVertex);
          Vec3d v = mesh->getPosition(*vertexHandle);
          int vertexPositionIndex = vertexHandle->getPositionIndex();
  
          // set normal
          if(renderMode & OBJMESHRENDER_SMOOTH)
          {
            if(vertexHandle->hasNormalIndex())
            {
              Vec3d normal = mesh->getNormal(*vertexHandle);
              glNormal3d(normal[0], normal[1], normal[2]);
            }
            else
              warnMissingNormals = true;
          }
  
          // set texture coordinate
          if(renderMode & OBJMESHRENDER_TEXTURE)
          {
            if(vertexHandle->hasTextureCoordinateIndex())
            {
              Vec3d vtexture = mesh->getTextureCoordinate(*vertexHandle);
              glTexCoord2d(vtexture[0], vtexture[1]);
            }
            else
              warnMissingTextureCoordinates = true;
          }

          if(renderMode & OBJMESHRENDER_CUSTOMCOLOR)
          {
            if (customColors.size() == 0)
              glColor3f(0,0,1);
            else
              glColor3f(customColors[vertexPositionIndex][0], customColors[vertexPositionIndex][1], customColors[vertexPositionIndex][2]);
          }
  
          // set position
          glVertex3d(v[0],v[1],v[2]);
        }
  
        glEnd();
      }

      if((renderMode & OBJMESHRENDER_TEXTURE))
        glDisable(GL_TEXTURE_2D);
    }
    if(geometryMode & (OBJMESHRENDER_EDGES | OBJMESHRENDER_VERTICES))
    {
      //glDisable(GL_POLYGON_OFFSET_FILL);
    }
  }

  // render vertices 
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);

  if(geometryMode & OBJMESHRENDER_VERTICES)
  {
    int numVertices = mesh->getNumVertices();

    for(int i = 0; i < numVertices; i++)
    {
      if(renderMode & OBJMESHRENDER_SELECTION)
        glLoadName(i);  
      glBegin(GL_POINTS);
      Vec3d pos = mesh->getPosition(i);
      glVertex3f(pos[0], pos[1], pos[2]);
      glEnd();
    }
  }

  // render edges 
  if (geometryMode & OBJMESHRENDER_EDGES)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_POLYGON_OFFSET_LINE);
    //glPolygonOffset(1.0, 1.0);
    glPolygonOffset(-1.0, -1.0);
    for(unsigned int i=0; i < mesh->getNumGroups(); i++)
    {
      const ObjMesh::Group * groupHandle = mesh->getGroupHandle(i);
      for(unsigned int iFace = 0; iFace < groupHandle->getNumFaces(); ++iFace)
      {
        const ObjMesh::Face * faceHandle = groupHandle->getFaceHandle(iFace);
        if(geometryMode & OBJMESHRENDER_EDGES)
        {
          glBegin(GL_POLYGON);
          for(unsigned int iVertex = 0; iVertex < faceHandle->getNumVertices(); ++iVertex)
          {
  	    const ObjMesh::Vertex * vertexHandle = faceHandle->getVertexHandle(iVertex);
  	    Vec3d v = mesh->getPosition(*vertexHandle);
  	    glVertex3d(v[0], v[1], v[2]);
          }
          glEnd();
        }
      }
    }
    glDisable(GL_POLYGON_OFFSET_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (lightingInitiallyEnabled)
    glEnable(GL_LIGHTING);

  //print warnings
  if(warnMissingNormals)
    printf("Warning: used SMOOTH rendering with missing vertex normal(s).\n");
  if(warnMissingFaceNormals)
    printf("Warning: used FLAT rendering with missing face normal(s).\n");
  if(warnMissingTextureCoordinates)
    printf("Warning: used TEXTURE rendering with missing texture coordinate(s).\n");
  if(warnMissingTextures)
    printf("Warning: used TEXTURE rendering with un-setup texture(s).\n");
}

unsigned int ObjMeshRender::createDisplayList(int geometryMode, int renderMode)
{
  unsigned int list = glGenLists(1);
  glNewList(list, GL_COMPILE);
    render(geometryMode, renderMode);
  glEndList();
  return list;
}

void ObjMeshRender::renderSpecifiedVertices(int * specifiedVertices, int numSpecifiedVertices)
{
  glBegin(GL_POINTS);

  for(int i = 0; i < numSpecifiedVertices; i++)
  {
    renderVertex(specifiedVertices[i]);
  }

  glEnd();
}

void ObjMeshRender::renderVertex(int index)
{
  Vec3d pos = mesh->getPosition(index);
  glVertex3f(pos[0], pos[1], pos[2]);
}

void ObjMeshRender::renderGroupEdges(char * groupName)
{
  //get the group
  string name(groupName);
  ObjMesh::Group group = mesh->getGroup(name);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_POLYGON_OFFSET_LINE);
  glPolygonOffset(1.0, 1.0);

  for( unsigned int iFace = 0; iFace < group.getNumFaces(); ++iFace )
  {
    ObjMesh::Face face = group.getFace(iFace);

    glBegin(GL_POLYGON);
    for( unsigned int iVertex = 0; iVertex < face.getNumVertices(); ++iVertex )
    {
      const ObjMesh::Vertex * vertexHandle = face.getVertexHandle(iVertex);
      Vec3d v = mesh->getPosition(*vertexHandle);
      glVertex3d(v[0], v[1], v[2]);
    }
    glEnd();
  }

  glDisable(GL_POLYGON_OFFSET_LINE);
}

int ObjMeshRender::numTextures()
{
  int numTextures = 0;
  int numMaterials = mesh->getNumMaterials();
  for(int i = 0; i < numMaterials; i++)
  {
    const ObjMesh::Material * material = mesh->getMaterialHandle(i);
    if(material->hasTextureFilename())
      numTextures++;
  }
  return numTextures;
}

void ObjMeshRender::loadTextures(int textureMode)
{
  //clear old

  textures.clear();

  int numMaterials = mesh->getNumMaterials();
  textures.resize(numMaterials);

  const ObjMesh::Material * material;
  char path[4096];
  string objName = mesh->getFilename();
  mesh->dirname(objName.c_str(), path);
  string pathStr = string(path);

  for(int i = 0; i < numMaterials; i++)
  {
    material = mesh->getMaterialHandle(i);
    if(!material->hasTextureFilename())
      continue;

    textures[i].loadTexture(pathStr + string("/") + material->getTextureFilename(), textureMode);
  }
}

ObjMeshRender::Texture * ObjMeshRender::getTextureHandle(int textureIndex)
{
  return &textures[textureIndex];
}

unsigned char * ObjMeshRender::loadPPM(string filename, int * width, int * height)
{
  char buf[4096];
  FILE * file;

  unsigned char * data;

  int maxval;

  file = fopen(filename.c_str(), "rb");
  if(!file)
    return NULL;

  char * result = fgets(buf, 4096, file);
  result = result; // to avoid compiler warnings
  if(strncmp(buf, "P6", 2))
  {
    printf("file is not raw RGB ppm\n");
    fclose(file);
    return NULL;
  }

  //read file dimensions

  int i = 0;
  while(i < 3)
  {
    result = fgets(buf, 4096, file);
    if(buf[0] == '#')
      continue;
    if(i == 0)
      i += sscanf(buf, "%d %d %d", width, height, &maxval);
    else if(i == 1)
      i += sscanf(buf, "%d %d", height, &maxval);
    else if(i == 2)
      i += sscanf(buf, "%d", &maxval);
  }

  //read
  data = (unsigned char*) malloc (sizeof(unsigned char) * 3 * *width * *height);
  if((int)fread(data, sizeof(unsigned char), 3 * *width * *height, file) < 3 * *width * *height)
  {
    printf("error reading ppm image data\n");
    free(data);
    fclose(file);
    return NULL;
  }

  // must flip image: PPM gives pixels top-to-bottom, but glTexImage2D expects bottom-to-top
  unsigned char * rowBuffer = (unsigned char*) malloc (sizeof(unsigned char) * 3 * *width);
  for(int row=0; row < *height / 2; row++)
  {
    int otherRow = *height - 1 - row;

    // swap row and otherRow
 
    unsigned char * rowPixels = &data[3 * *width * row];
    unsigned char * otherRowPixels = &data[3 * *width * otherRow];

    // copy row to rowBuffer
    for(int i=0; i<3 * *width; i++)
      rowBuffer[i] = rowPixels[i];

    // copy otherRow to row
    for(int i=0; i<3 * *width; i++)
      rowPixels[i] = otherRowPixels[i];

    // copy rowBuffer to otherRow
    for(int i=0; i<3 * *width; i++)
      otherRowPixels[i] = rowBuffer[i];
  }
  free(rowBuffer);

  fclose(file);
  return data;
}

void ObjMeshRender::outputOpenGLRenderCode()
{
  for(unsigned int i=0; i < mesh->getNumGroups(); i++)
  {
    const ObjMesh::Group * groupHandle = mesh->getGroupHandle(i);

    // set material
    ObjMesh::Material material = mesh->getMaterial(groupHandle->getMaterialIndex());

    Vec3d Ka = material.getKa();
    Vec3d Kd = material.getKd();
    Vec3d Ks = material.getKs();
    float shininess = material.getShininess();

    printf("float ambient[4] = { %f, %f, %f, 1.0 };\n", Ka[0], Ka[1], Ka[2]);
    printf("float diffuse[4] = { %f, %f, %f, 1.0 };\n", Kd[0], Kd[1], Kd[2]);
    printf("float specular[4] = { %f, %f, %f, 1.0 };\n", Ks[0], Ks[1], Ks[2]);

    printf("glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);\n");
    printf("glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);\n");
    printf("glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);\n");
    printf("glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, %f);\n", shininess);

    for( unsigned int iFace = 0; iFace < groupHandle->getNumFaces(); ++iFace )
    {
      ObjMesh::Face face = groupHandle->getFace(iFace);

      printf("glBegin(GL_POLYGON);\n");

      for( unsigned int iVertex = 0; iVertex < face.getNumVertices(); ++iVertex )
      {
        const ObjMesh::Vertex * vertexHandle = face.getVertexHandle(iVertex);
        Vec3d v = mesh->getPosition(*vertexHandle);

        // set normal
        if( vertexHandle->hasNormalIndex() )
        {
          Vec3d normal = mesh->getNormal(*vertexHandle);
          printf("glNormal3d(%f,%f,%f);\n", normal[0], normal[1], normal[2]);
        }

        // set texture coordinate
        if(vertexHandle->hasTextureCoordinateIndex())
        {
          Vec3d textureCoordinate = mesh->getTextureCoordinate(*vertexHandle);
          printf("glTexCoord2d(%f,%f);\n", textureCoordinate[0], textureCoordinate[1]);
        }

        printf("glVertex3d(%f,%f,%f);\n", v[0],v[1],v[2]);
      }
      printf("glEnd();\n");
    }
  }
}

void ObjMeshRender::renderNormals(double normalLength)
{
  double diameter = mesh->getDiameter();

  glBegin(GL_LINES);

  for(unsigned int i=0; i < mesh->getNumGroups(); i++)
  {
    const ObjMesh::Group * groupHandle = mesh->getGroupHandle(i);
    for(unsigned int iFace = 0; iFace < groupHandle->getNumFaces(); ++iFace)
    {
      ObjMesh::Face face = groupHandle->getFace(iFace);

      for(unsigned int iVertex = 0; iVertex < face.getNumVertices(); ++iVertex)
      {
        const ObjMesh::Vertex * vertexHandle = face.getVertexHandle(iVertex);
        Vec3d v = mesh->getPosition(*vertexHandle);

        glVertex3d(v[0], v[1], v[2]);

        // compute endpoint
        Vec3d vnormalOffset;
        if(vertexHandle->hasNormalIndex())
          vnormalOffset = v + normalLength * diameter * mesh->getNormal(*vertexHandle);
        else
          vnormalOffset = v;

        glVertex3d(vnormalOffset[0], vnormalOffset[1], vnormalOffset[2]);
      }
    }
  }

  glEnd();
}

void ObjMeshRender::setCustomColors(Vec3d color)
{
  int numVertices = (int)mesh->getNumVertices();
  customColors.clear();
  for(int i=0; i<numVertices; i++)
    customColors.push_back(color);
}

void ObjMeshRender::setCustomColors(vector<Vec3d> colors)
{
  int numVertices = (int)mesh->getNumVertices();
  customColors.clear();
  for(int i=0; i<numVertices; i++)
    customColors.push_back(colors[i]);
}


