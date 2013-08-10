//
//  print.c
//  trabCGDraw3d
//
//  Created by Carolina Zamith Cunha on 8/10/13.
//  Copyright (c) 2013 Carolina Zamith Cunha. All rights reserved.
//


#include "screencasts.h"

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
