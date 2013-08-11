//
//  print.c
//  cgTrab2
//
//  Created by Marcelle Guine on 11/08/13.
//  Copyright (c) 2013 Marcelle Guine. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* OpenGL */
#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "print.h"


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