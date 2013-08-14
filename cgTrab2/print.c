

#ifndef SCREENCASTS
#define SCREENCASTS

/* standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* OpenGL and friends */
#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



#endif

void printv(va_list args, const char* format);
void print(const char* format, ...);
void printAt(int x, int y, const char* format, ...);
void errCheck(char* where);

#define LEN 8192
void printv(va_list args, const char* format)
{
  char buf[LEN];
  char* ch=buf;
  vsnprintf(buf,LEN,format,args);
  while (*ch)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void print(const char* format, ...)
{
  va_list args;
  va_start(args,format);
  printv(args,format);
  va_end(args);
}

void printAt(int x, int y, const char* format, ...)
{
  va_list args;
  glWindowPos2i(x,y);
  va_start(args,format);
  printv(args,format);
  va_end(args);
}

/*
 *  Convenience method to print out OpenGL errors to stderr
 */
void errCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}
