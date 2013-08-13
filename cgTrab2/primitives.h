#ifndef PRIMITIVES_h

#define PRIMITIVES_h

#ifdef WIN32
  #include <windows.h>
#endif

#include"OpenGL/gl.h"

void InitQuadrics();
void PolygonFill(int a, int b, int c, int d, GLfloat normal[3]);
void PolygonBorder(int a, int b, int c, int d);
void InitCubesDisplayLists();
void Cube();
void CubeBorder();
void ColorCube();
void ColorCubeTexturized(GLuint texHandle[6]);
void Sphere();
void SphereTexturized(GLuint texHandle);
void Cylinder();
void ClosedCylinder();
void Torus(GLfloat innerRadius, GLfloat outerRadius, GLfloat nsides, GLfloat rings);
void Teapot(GLdouble size);
void TeapotTexturized(GLdouble size, GLuint texHandle);
void RenderBitmapString2D(float x, float y, void *font, char *string);
void RenderBitmapString(float x, float y, float z, void *font, char *string);

#endif
