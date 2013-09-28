/*
 * OFFviewer.h
 *
 *  Created on: Sep 23, 2013
 *      Author: malvir
 */

#ifndef OFFVIEWER_H_
#define OFFVIEWER_H_



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

typedef struct Vertex {
  float x, y, z;
  int index;
} Vertex;

typedef struct Face {
  Face(void) : nverts(0), verts(0) {};
  int nverts;
  Vertex **verts;
  float normal[3];
} Face;

typedef struct Mesh {
  Mesh(void) : nverts(0), verts(0), nfaces(0), faces(0) {};
  int nverts;
  Vertex *verts;
  float *normalArray;
  float *vertexArray;
  unsigned short *indices;
  int nfaces;
  Face *faces;
} Mesh;

bool isFloatEqual(float x,float y);

bool isVertexEqual(Vertex *v1,Vertex *v2);
void calculateNormals(Mesh *m);
Mesh *
ReadOffFile(const char *filename);


#endif /* OFFVIEWER_H_ */
