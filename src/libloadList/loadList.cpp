/*************************************************************************
 *                                                                       *
 * Vega FEM Simulation Library Version 2.0                               *
 *                                                                       *
 * "loadList" library , Copyright (C) 2007 CMU, 2009 MIT                 *
 * All rights reserved.                                                  *
 *                                                                       *
 * Code author: Jernej Barbic                                            *
 * http://www.jernejbarbic.com/code                                      *
 * Research: Jernej Barbic, Doug L. James, Jovan Popovic                 *
 * Funding: NSF, Link Foundation, Singapore-MIT GAMBIT Game Lab          *
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

#include "loadList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// removes all whitespace characters from string s
void LoadList::stripBlanks(char * s)
{
  char * w = s;
  while (*w != '\0')
  {
    while (*w == ' ') // erase blank
    {
      char * u = w;
      while (*u != '\0') // shift everything left one char
      {
        *u = *(u+1);
        u++;
      }
    }
    w++;
  }
}

int compareLoadList(const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int LoadList::load(char * filename, int * numListEntries, int ** listEntries, int offset)
{
   // comma-separated text file of fixed vertices
  FILE * fin;
  fin = fopen(filename,"r");
  if (!fin)
  {
    printf("Error: could not open file %s.\n",filename);
    return 1;
  }

  *numListEntries = 0;

  char s[4096];
  while (fgets(s,4096,fin) != NULL)
  { 
    stripBlanks(s);

    char * pch;
    pch = strtok (s,",");
    while ((pch != NULL) && (isdigit(*pch)))
    {
      (*numListEntries)++;
      pch = strtok (NULL, ",");
    } 
  }

  *listEntries = (int*) malloc (sizeof(int) * (*numListEntries));

  rewind(fin);

  (*numListEntries) = 0;

  while (fgets(s,4096,fin) != NULL)
  {
    stripBlanks(s);
    char * pch;
    pch = strtok (s,",");
    while ((pch != NULL) && (isdigit(*pch)))
    {
      (*listEntries)[*numListEntries] = atoi(pch) - offset;
      (*numListEntries)++;
      pch = strtok (NULL, ",");
    }
  }

  // sort the list entries
  qsort ((*listEntries), *numListEntries, sizeof(int), compareLoadList);

  fclose(fin);

  return 0;
}

int loadListComparator(const void * a, const void * b)
{
  if (*(int*)a < *(int*)b)
    return -1;

  if (*(int*)a == *(int*)b)
    return 0;

  return 1;
}

void LoadList::sort(int numListEntries, int * listEntries)
{
  qsort(listEntries,numListEntries,sizeof(int),loadListComparator);
}

int LoadList::save(char * filename, int numListEntries, int * listEntries, int offset)
{
  // comma-separated text file of fixed vertices
  FILE * fout;
  fout = fopen(filename, "w");
  if (!fout)
  {
    printf("Error: could not open boundary vertices specification file %s.\n",filename);
    return 1;
  }

  for(int nv=0; nv < numListEntries; nv++)
  {
    fprintf(fout, "%d,", listEntries[nv] + offset);
    if (nv % 8 == 7)
      fprintf(fout,"\n");
  }
  fprintf(fout,"\n");

  fclose(fout);

  return 0;
}

void LoadList::print(int numListEntries, int * listEntries)
{
  for(int nv=0; nv < numListEntries; nv++)
  {
    printf("%d,", listEntries[nv]);
    if (nv % 8 == 7)
      printf("\n");
  }
  printf("\n"); fflush(NULL);
}

