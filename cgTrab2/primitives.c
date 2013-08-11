//
//  primitives.c
//  cgTrab2
//
//  Created by Marcelle Guine on 11/08/13.
//  Copyright (c) 2013 Marcelle Guine. All rights reserved.
//

#ifdef WIN32
#include <windows.h>
#else
#define CALLBACK
#endif

#include <stdio.h>
#include "primitives.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


GLuint          cubeList,cubeBorderList,colorCubeList,textureCubeList,
sphereList,cylinderList,closedCylinderList,diskList,partialDiskList;
GLuint          quadricsStartList;
GLUquadricObj * qobj;


/* Coordenadas 3D dos oito vértices de um cubo canônico centrado na origem*/
GLfloat vertices[][3] = {
    {-1.0,-1.0,-1.0},
    { 1.0,-1.0,-1.0},
    { 1.0, 1.0,-1.0},
    {-1.0, 1.0,-1.0},
    {-1.0,-1.0, 1.0},
    { 1.0,-1.0, 1.0},
    { 1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0}};

/* Normais das 6 faces do cubo */
GLfloat normals[][3]  = {
    { 0.0, 0.0,-1.0},
    { 1.0, 0.0, 0.0},
    { 0.0, 1.0, 0.0},
    {-1.0, 0.0, 0.0},
    { 0.0,-1.0, 0.0},
    { 0.0, 0.0, 1.0}};

GLubyte colors[][3]   = {
    {  0,  0,  0}, /* black  */
    {255,  0,  0}, /* red    */
    {255,255,  0}, /* yellow */
    {  0,255,  0}, /* green  */
    {  0,  0,255}, /* blue   */
    {255,  0,255}, /* violet */
    {255,255,255}, /* white  */
    {  0,255,255}};/* cyan   */


/*Desenha polígono (modo GL_LINE) */
void PolygonBorder(int a, int b, int c, int d)
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_POLYGON);
    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

/*Desenha polígono preenchido (modo GL_FILL) */
void PolygonFill(int a, int b, int c, int d, GLfloat normal[3])
{
    glBegin(GL_POLYGON);
    glNormal3fv(normal);
    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
    glEnd();
}


/* Inicializa listas de visualização (Display Lists) para              */
/* desenho de cubos. As listas de visualização compilam na             */
/* placa de vídeo  conjuntos de chamadas para desenho de               */
/* primitivas, transformações geométricas e comandos de configuração   */
/* de atributos. Isto possibilita o desenho de um objeto gráfico com   */
/* maior eficiência já que não há a necessidade de realimentar a placa */
/* cada vez que for necessário o redesenho, uma vez que o objeto já    */
/* se encontra armazenado em memória de vídeo.                         */

void InitCubesDisplayLists()
{
    
    /* Gera uma lista de visualização e retorna seu identificador(handle)*/
    cubeList = glGenLists(1);
    /* Inicia a definição dos comandos associados a lista*/
    glNewList(cubeList,GL_COMPILE);
    PolygonFill(1,0,3,2,normals[0]);
    PolygonFill(5,1,2,6,normals[1]);
    PolygonFill(7,6,2,3,normals[2]);
    PolygonFill(0,4,7,3,normals[3]);
    PolygonFill(0,1,5,4,normals[4]);
    PolygonFill(4,5,6,7,normals[5]);
    /* Finaliza a definição da lista */
    glEndList();
    
    cubeBorderList = glGenLists(1);
    glNewList(cubeBorderList,GL_COMPILE);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glLineWidth(1.5f);
    PolygonBorder(1,0,3,2);
    PolygonBorder(5,1,2,6);
    PolygonBorder(7,6,2,3);
    PolygonBorder(0,4,7,3);
    PolygonBorder(0,1,5,4);
    PolygonBorder(4,5,6,7);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glLineWidth(1.0);
    glEndList();
    
    colorCubeList = glGenLists(1);
    glNewList(colorCubeList,GL_COMPILE);
    glColor3ubv(colors[6]);
    PolygonFill(1,0,3,2,normals[0]);
    glColor3ubv(colors[1]);
    PolygonFill(5,1,2,6,normals[1]);
    glColor3ubv(colors[2]);
    PolygonFill(7,6,2,3,normals[2]);
    glColor3ubv(colors[3]);
    PolygonFill(0,4,7,3,normals[3]);
    glColor3ubv(colors[4]);
    PolygonFill(0,1,5,4,normals[4]);
    glColor3ubv(colors[5]);
    PolygonFill(4,5,6,7,normals[5]);
    glEndList();
    
    
}

void Cube()
{
    glCallList(cubeList);
}

void CubeBorder()
{
    glCallList(cubeBorderList);
}

void ColorCube()
{
    glCallList(colorCubeList);
}


void CALLBACK errorCallback(GLenum errorCode)
{
    const GLubyte * estring;
    
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Quadric Error:%s\n",estring);
    exit(0);
}


void InitQuadrics()
{
    /* Cria uma superfície quadrica */
    qobj = gluNewQuadric();
    gluQuadricCallback(qobj, GLU_ERROR, errorCallback);
    
    /* Seleciona o modo de preenchimento */
    gluQuadricDrawStyle(qobj,GLU_FILL);
    /* Gera normais interpoladas         */
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluQuadricTexture(qobj,GL_TRUE);
    
    sphereList = glGenLists(1);
    glNewList(sphereList,GL_COMPILE);
    /* Define uma esfera de raio 1 como numero de fatias e seções igual a 200  */
    gluSphere(qobj, 1.0, 50, 50);
    glEndList();
    
    
    cylinderList = glGenLists(1);
    glNewList(cylinderList,GL_COMPILE);
    /* Define um cilindro com base e topo de raio unitário, altura unitária e */
    /* com numero de fatias e seções igual a 50                               */
    gluCylinder(qobj, 1.0, 1.0, 1.0, 50, 50);
    glEndList();
    
    
    closedCylinderList = glGenLists(1);
    glNewList(closedCylinderList,GL_COMPILE);
    /* Define um cilindro fechado */
    glPushMatrix();
    glTranslatef(0.0,0.0,1.0);
    gluDisk(qobj,0.0,1.0,50,50);
    glPopMatrix();
    gluCylinder(qobj, 1.0, 1.0, 1.0, 50, 50);
    gluDisk(qobj,0.0,1.0,50,50);
    glEndList();
    
    diskList = glGenLists(1);
    glNewList(diskList,GL_COMPILE);
    /* Define um disco de raio unitário com número de fatias e anéis igual a 50 */
    gluDisk(qobj, 0.0, 1.0, 50, 50);
    glEndList();
    
}

void Sphere()
{
    glCallList(sphereList);
}

void Cylinder()
{
    glCallList(closedCylinderList);
}

void ClosedCylinder()
{
    glCallList(closedCylinderList);
}

void RenderBitmapString2D(float x, float y, void *font,char *string)
{
    char *c;
    /* Posiciona o cursor na posição (x,y,z) */
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        /* Função da glut que escreve um caractere na posição corrente */
        glutBitmapCharacter(font, *c);
    }
}

/* Escreve uma cadeia de caracteres */
void RenderBitmapString(float x, float y, float z, void *font, char *string)
{
    char *c;
    /* Posiciona o cursor na posição (x,y,z) */
    glRasterPos3f(x, y, z);
    for (c=string; *c != '\0'; c++) {
        /* Função da glut que escreve um caractere na posição corrente */
        glutBitmapCharacter(font, *c);
    }
}