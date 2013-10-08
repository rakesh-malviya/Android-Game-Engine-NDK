/*
 * OFFviewer.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: malvir
 */

#include "OFFviewer.h"
#include <ctype.h>

bool isFloatEqual(float x,float y)
{
	if(fabs(x-y)>0.00001f)
		return false;
	else
		return true;
}

bool isVertexEqual(Vertex *v1,Vertex *v2)
{
	if(isFloatEqual(v1->x,v2->x)&&isFloatEqual(v1->y,v2->y)&&isFloatEqual(v1->z,v2->z))
		return true;
	else
		return false;
}

void calculateNormals(Mesh *m)
{

	m->normalArray=new float[(m->nverts)*3];
	m->vertexArray=new float[(m->nverts)*3];
	m->indices=new unsigned short[(m->nfaces)*3];

	for(int k=0; k < m->nverts; k++)
	{
		Vertex &currVert=m->verts[k];
		m->vertexArray[3*k+0]=currVert.x;
		m->vertexArray[3*k+1]=currVert.y;
		m->vertexArray[3*k+2]=currVert.z;
		currVert.index=k;
	}

	for (int i = 0; i < m->nfaces; i++)
	{
			Face& face = m->faces[i];
			for (int j = 0; j < 3; j++)
			{
				Vertex *v = face.verts[j];
				m->indices[3*i+j]=v->index;
			}
	}



	for(int k=0; k < m->nverts; k++)
	{
		Vertex &currVert=m->verts[k];

		for (int i = 0; i < m->nfaces; i++)
		{
			bool found=false;
			Face& face = m->faces[i];
			float vertexAngle=0;
			for (int j = 0; j < 3; j++)
			{
				Vertex *v = face.verts[j];
				if(isVertexEqual(&currVert,v))
				{
					Vertex *v1,*v2;
					found=true;
					/*if(j==0)
					{
						v1=face.verts[face.nverts-1];
						v2=face.verts[j+1];
					}
					else if(j==(face.nverts-1))
					{
						v1=face.verts[j-1];
						v2=face.verts[0];
					}
					else
					{
						v1=face.verts[j-1];
						v2=face.verts[j+1];
					}


					float dotProduct=((v1->x - v->x)*(v2->x - v->x))+((v1->y - v->y)*(v2->y - v->y))+((v1->z - v->z)*(v2->z - v->z));
					float magnitude=sqrt((v1->x - v->x)*(v1->x - v->x)+(v1->y - v->y)*(v1->y - v->y)+(v1->z - v->z)*(v1->z - v->z))*sqrt((v2->x - v->x)*(v2->x - v->x)+(v2->y - v->y)*(v2->y - v->y)+(v2->z - v->z)*(v2->z - v->z));
					vertexAngle=acosf(dotProduct/magnitude);*/

					break;//Vertex found
				}
			}

			if(found)
			{
				m->normalArray[3*k+0]+=face.normal[0];
				m->normalArray[3*k+1]+=face.normal[1];
				m->normalArray[3*k+2]+=face.normal[2];
				/*currVert.nx+=face.normal[0]*vertexAngle;
				currVert.ny+=face.normal[1]*vertexAngle;
				currVert.nz+=face.normal[2]*vertexAngle;*/
			}
		}

		//normalize

		float nl=sqrtf((m->normalArray[3*k+0]*m->normalArray[3*k+0])+(m->normalArray[3*k+1]*m->normalArray[3*k+1])+(m->normalArray[3*k+2]*m->normalArray[3*k+2]));
		if(nl > 1.0E-6)
		{
			m->normalArray[3*k+0]=m->normalArray[3*k+0]/nl;
			m->normalArray[3*k+1]=m->normalArray[3*k+1]/nl;
			m->normalArray[3*k+2]=m->normalArray[3*k+2]/nl;
		}
	}


}

Mesh * ReadOffFile(const char *filename)
{
  int i;

  // Open file
  FILE *fp;
  if (!(fp = fopen(filename, "r"))) {
    fprintf(stderr, "Unable to open file %s\n", filename);
    return 0;
  }

  // Allocate mesh structure
  Mesh *mesh = new Mesh();
  if (!mesh) {
    fprintf(stderr, "Unable to allocate memory for file %s\n", filename);
    fclose(fp);
    return 0;
  }

  // Read file
  int nverts = 0;
  int nfaces = 0;
  int nedges = 0;
  int line_count = 0;
  char buffer[1024];
  while (fgets(buffer, 1023, fp)) {
    // Increment line counter
    line_count++;

    // Skip white space
    char *bufferp = buffer;
    while (isspace(*bufferp)) bufferp++;

    // Skip blank lines and comments
    if (*bufferp == '#') continue;
    if (*bufferp == '\0') continue;

    // Check section
    if (nverts == 0) {
      // Read header
      if (!strstr(bufferp, "OFF")) {
        // Read mesh counts
        if ((sscanf(bufferp, "%d%d%d", &nverts, &nfaces, &nedges) != 3) || (nverts == 0)) {
          fprintf(stderr, "Syntax error reading header on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return NULL;
        }

        // Allocate memory for mesh
        mesh->verts = new Vertex [nverts];
        assert(mesh->verts);
        mesh->faces = new Face [nfaces];
        assert(mesh->faces);
      }
    }
    else if (mesh->nverts < nverts) {
      // Read vertex coordinates
      Vertex& vert = mesh->verts[mesh->nverts++];
      if (sscanf(bufferp, "%f%f%f", &(vert.x), &(vert.y), &(vert.z)) != 3) {
        fprintf(stderr, "Syntax error with vertex coordinates on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return NULL;
      }

      if(vert.x > mesh->maxX)
    	  mesh->maxX = vert.x;
      if(vert.x < mesh->minX)
    	  mesh->minX = vert.x;
      if(vert.y > mesh->maxY)
    	  mesh->maxY = vert.y;
      if(vert.y < mesh->minY)
    	  mesh->minY = vert.y;
      if(vert.z > mesh->maxZ)
    	  mesh->maxZ = vert.z;
      if(vert.z < mesh->minZ)
    	  mesh->minZ = vert.z;
    }
    else if (mesh->nfaces < nfaces) {
      // Get next face
      Face& face = mesh->faces[mesh->nfaces++];

      // Read number of vertices in face
      bufferp = strtok(bufferp, " \t");
      if (bufferp) face.nverts = atoi(bufferp);
      else {
        fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return NULL;
      }

      // Allocate memory for face vertices
      face.verts = new Vertex *[face.nverts];
      assert(face.verts);

      // Read vertex indices for face
      for (i = 0; i < face.nverts; i++) {
        bufferp = strtok(NULL, " \t");
        if (bufferp) face.verts[i] = &(mesh->verts[atoi(bufferp)]);
        else {
          fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return NULL;
        }
      }

      // Compute normal for face
      face.normal[0] = face.normal[1] = face.normal[2] = 0;
      Vertex *v1 = face.verts[face.nverts-1];
      for (i = 0; i < face.nverts; i++) {
        Vertex *v2 = face.verts[i];
        face.normal[0] += (v1->y - v2->y) * (v1->z + v2->z);
        face.normal[1] += (v1->z - v2->z) * (v1->x + v2->x);
        face.normal[2] += (v1->x - v2->x) * (v1->y + v2->y);
        v1 = v2;
      }

      // Normalize normal for face
      float squared_normal_length = 0.0;
      squared_normal_length += face.normal[0]*face.normal[0];
      squared_normal_length += face.normal[1]*face.normal[1];
      squared_normal_length += face.normal[2]*face.normal[2];
      float normal_length = sqrt(squared_normal_length);
      if (normal_length > 1.0E-6) {
        face.normal[0] /= normal_length;
        face.normal[1] /= normal_length;
        face.normal[2] /= normal_length;
      }
    }
    else {
      // Should never get here
      fprintf(stderr, "Found extra text starting at line %d in file %s\n", line_count, filename);
      break;
    }
  }

  // Check whether read all faces
  if (nfaces != mesh->nfaces) {
    fprintf(stderr, "Expected %d faces, but read only %d faces in file %s\n", nfaces, mesh->nfaces, filename);
  }

  // Close file
  fclose(fp);
  calculateNormals(mesh);

  // Return mesh
  return mesh;
}
