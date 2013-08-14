//
//  main.c
//  cgTrab2
//
//  Created by Marcelle Guine e Carolina Zamith on 11/08/13.
//  Copyright (c) 2013 Marcelle Guine. All rights reserved.
//


// EXTRAS DO TRABALHO //
// 1) INSERÇÃO DA OPCÃO DE PROJEÇÃO POR PERSPECTIVA OU ORTOGONAL//
// 2) POSSIBILIDADE DE REDIMENSIONAR O OBJETO
// 3) 
#ifdef WIN32
    #include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include "print.c"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <OpenGL/glut.h>
#endif


#include "primitives.h"
#include "image.h"

#define PI 3.1415926535898
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
int toggleMode = 1; /* modo da projeção */
int toggleAxes = 1;   /* Aciona ou não os eixos */
int toggleValues = 1;/* Aciona ou não valores */

float X = -20;
float delta = 0.4;



float width = 0;
float height = 0;


double dimension=5.0; /* dim da caixa ortogonal */

enum STATE {
	NO_STATE,
    M_HEAD,
    M_TRUNK,
    M_LEFT_ARM,
    M_RIGHT_ARM,
    M_LEFT_FOREARM,
    M_RIGHT_FOREARM,
    M_LEFT_LEG,
    M_RIGHT_LEG,
    M_LEFT_THIGH,
    M_RIGHT_THIGH

} current_state = NO_STATE;

int menu_option;

/*  Variáveis globais */
int th = 0;  /* azimute do angulo de visão */
int ph = 0;  /* elevação do angulo de visão */
int filed_of_vision = 30;   /* campo de visão */
int aspect_relation = 1;    /* relação de aspecto */

float a_world [2] = {0, -35};
float a_head [4] = {0, 0, 0, 0};
float a_trunk [2] = {0, 0};
float a_larm [4] = {15, 0, 1, 0};
float a_rarm [4] = {0, 0, 0, 0};
float a_lforearm [2] = {20, 1};
float a_rforearm [2] = {0, 0};
float a_lleg [2] = {20, 1};
float a_rleg [2] = {-20, 1};
float a_lthigh [2] = {20, 1};
float a_rthigh [2] = {10, 1};

#define FORE_ARM_ANGLE_FRONT 30
#define FORE_ARM_ANGLE_BACK 120

#define ARM_ANGLE_LEFT 20
#define ARM_ANGLE_RIGHT 160

#define ARM_ANGLE_FRONT 120
#define ARM_ANGLE_BACK 140

#define TIGHT_ANGLE_FRONT 30
#define TIGHT_ANGLE_BACK 120

#define TRUNK_ANGLE_BACK 40
#define TRUNK_ANGLE_FRONT 80

#define LEG_ANGLE_FRONT 140
#define LEG_ANGLE_BACK 20

tpImage * bobfront1, * bobfront2, * bobback,
* bobleft,   * bobright,
* bobtop,    * bobbottom;

GLuint    texHandle[6];

GLubyte v_colors[][3]   = {
    {  0,  0,  0}, /* black  */
    {255,  0,  0}, /* red    */
    {255,255,  0}, /* yellow */
    {  0,255,  0}, /* green  */
    {  0,  0,255}, /* blue   */
    {255,  0,255}, /* violet */
    {255,255,255}, /* white  */
    {  0,255,255}};/* cyan   */


/* Projeção */
void project()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
   
    if (toggleMode) {
        /* perspectiva */
        gluPerspective(filed_of_vision,aspect_relation,dimension/4,4*dimension);    }
    else {
        /* projeção ortogonal*/
//        glOrtho(-dimension*aspect_relation,+dimension*aspect_relation, -dimension,+dimension, -dimension,+dimension);
        glOrtho(-dimension*aspect_relation,+dimension*aspect_relation, -dimension,+dimension, -dimension,+dimension);
       
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


/* Camera */
void setEye()
{
    if (toggleMode) {
        double Ex = -2*dimension*Sin(th)*Cos(ph);
        double Ey = +2*dimension        *Sin(ph);
        double Ez = +2*dimension*Cos(th)*Cos(ph);
        /* posiçao da camera*/
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
    /*  Ortogonal - seta a orientação do mundo */
    else {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }
}

void drawAxes()
{
    if (toggleAxes) {
        /*  Tamanho dos eixos */
        double len = 2.0;
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_LINES);
        glVertex3d(0,0,0);
        glVertex3d(len,0,0);
        glVertex3d(0,0,0);
        glVertex3d(0,len,0);
        glVertex3d(0,0,0);
        glVertex3d(0,0,len);
        glEnd();
        /*  Label dos eixos */
        glRasterPos3d(len,0,0);
        print("X");
        glRasterPos3d(0,len,0);
        print("Y");
        glRasterPos3d(0,0,len);
        print("Z");
    }
}

void drawValues()
{
    if (toggleValues) {
        glColor3f(0.8,0.8,0.8);
        printAt(5,5,"Angulo de visao (th, ph) =(%d, %d)", th, ph);
        printAt(5,25,"Modo de projecao =(%s)", toggleMode ? "Perspectiva" : "Ortogonal");
    }
}

void drawKnuckle()
{
    glPushMatrix();
    
    glColor3ubv(v_colors[5]);
    glScalef(0.15, 0.15, 0.15);
    glutSolidSphere(1, 16, 16);
    
    glPopMatrix();
}

void drawHead()
{
    glPushMatrix();
    
    glRotatef(a_head[0], a_head[2], 0, 0);
    glRotatef(a_head[1], 0, a_head[3], 0);
    
    glTranslatef (0.0, 1.8, 0.0);
    
    //chapéu
    glColor3ubv(v_colors[6]);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.4);
    glutSolidCone(0.5, 1.2, 10, 1);
    glPopMatrix();

    //cabeça
//    glColor3ubv(v_colors[2]);
    glScalef(0.8, 0.8, 0.8);
    ColorCubeTexturized(texHandle);
//    glutSolidCube(1);
    
    //pescoço
    glTranslatef(0.0, -1.1, 0.0);
    drawKnuckle();
    
    glPopMatrix();
}

void drawTrunk()
{
    glPushMatrix();
    
    //tronco
    glColor3ubv(v_colors[2]);
    glScalef(1.2, 1.7, 0.5);
    glutSolidCube(1);
    
    glPopMatrix();
}

void drawForeArm()
{
    glPushMatrix();
    
    //antebraço
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.55, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
}

void drawArm()
{    
    glPushMatrix();
    
    //braço
    glColor3ubv(v_colors[2]);
    glRotatef(-30.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.55, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    
    //dedos
    glColor3ubv(v_colors[6]);
    glTranslatef(0.0, -0.5, 0.0);
    glScalef(0.15, 0.25, 0.15);
    glutSolidCube(1);
    
    glPopMatrix();
}

void drawLeftArm()
{
    glTranslatef(-0.75, 0.4, 0.0);
    glRotatef(-15.0, 0.0, 0.0, 1.0);
    
    glPushMatrix();
    
    //ombro
    drawKnuckle();
    
    glRotatef(a_larm[0], a_larm[2], 0, 0);
    glRotatef(a_larm[1], 0, 0, a_larm[3]);
    
    drawForeArm();
    
    //cotovelo
    glTranslatef(0.0, -1.1, 0.0);
    drawKnuckle();
    
    glRotatef(a_lforearm[0], a_lforearm[1], 0, 0);
    
    drawArm();
    
    glPopMatrix();    
}

void drawRightArm()
{    
    glTranslatef(1.4, 0.4, 0.0);
    glRotatef(30.0, 0.0, 0.0, 1.0);
    
    glPushMatrix();
    
    //ombro
    drawKnuckle();
    
    glRotatef(a_rarm[0], a_rarm[2], 0, 0);
    glRotatef(a_rarm[1], 0, 0, a_rarm[3]);
    
    drawForeArm();
    
    //cotovelo
    glTranslatef(0.0, -1.1, 0.0);
    drawKnuckle();
    
    glRotatef(a_rforearm[0], a_rforearm[1], 0, 0);
    
    drawArm();
    
    glPopMatrix();
}

void drawThigh()
{
    glPushMatrix();
    
    //coxa
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.65, 0.0);
    glPushMatrix();
    glScalef(0.3, 1, 0.3);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
}

void drawLeg()
{
    glPushMatrix();
    
    //panturrilha
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.65, 0.0);
    glPushMatrix();
    glScalef(0.3, 1, 0.3);
    glutSolidCube(1);
    glPopMatrix();
    
    //dedos
    glColor3ubv(v_colors[6]);
    glTranslatef(0.0, -0.55, 0.05);
    glScalef(0.35, 0.15, 0.4);
    glutSolidCube(1);
    
    glPopMatrix();
}

void drawLeftLeg()
{
    glTranslatef(-0.3, -1.0, 0.0);
    glPushMatrix();
    
    //antecoxa
    drawKnuckle();
    
    glRotatef(a_lleg[0], a_lleg[1], 0, 0);
    
    drawThigh();
    
    //joelho
    glTranslatef(0.0, -1.3, 0.0);
    drawKnuckle();
    
    glRotatef(a_lthigh[0], a_lthigh[1], 0, 0);
    
    drawLeg();
    
    glPopMatrix();
}

void drawRightLeg()
{
    glTranslatef(0.65, 0.0, 0.0);

    glPushMatrix();
    
    //antecoxa
    drawKnuckle();
    
    glRotatef(a_rleg[0], a_rleg[1], 0, 0);
    
    drawThigh();
    
    //joelho
    glTranslatef(0.0, -1.3, 0.0);
    drawKnuckle();
    
    glRotatef(a_rthigh[0], a_rthigh[1], 0, 0);
    
    drawLeg();
    
    glPopMatrix();
}

void fullTrunk()
{
    
    glPushMatrix();
    
    glRotatef(a_trunk[0], a_trunk[1], 0, 0);
    drawTrunk();
    drawHead();
    drawLeftArm();
    drawRightArm();
 
    glPopMatrix();
}

void drawRobot()
{
    glPushMatrix();
    
    fullTrunk();
    drawAxes();
    drawValues();
    drawLeftLeg();
    drawRightLeg();
  
    
    glPopMatrix();
}

void Display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    
    setEye();
    
    // Se colocar o X aqui, o boneco so aparece uma vez na tela
    
    glTranslatef (0.0, 0.0, -8.0);
    glRotated(a_world[0], 1, 0, 0);
    glRotated(a_world[1], 0, 1, 0);
    
    drawRobot();
        
    glFlush();
    glutSwapBuffers();
}

void menu(int menu_option) {
    
	switch(menu_option) {
            
        /* Movimentar cabeça */
        case 1:
            current_state = M_HEAD;
            break;
            
        /* Movimentar tronco */
        case 2:
            current_state = M_TRUNK;
            break;
            
        /* Movimentar braço esquerdo */
        case 3:
            current_state = M_LEFT_ARM;
            break;
            
        /* Movimentar braço direito */
        case 4:
            current_state = M_RIGHT_ARM;
            break;
            
        /* Movimentar antebraço esquerdo */
        case 5:
            current_state = M_LEFT_FOREARM;
            break;
            
        /* Movimentar antebraço direito */
        case 6:
            current_state = M_RIGHT_FOREARM;
            break;
            
        /* Movimentar perna esquerda */
        case 7:
            current_state = M_LEFT_LEG;
            break;
            
        /* Movimentar perna direita */
        case 8:
            current_state = M_RIGHT_LEG;
            break;
            
        /* Movimentar coxa esquerda */
        case 9:
            current_state = M_LEFT_THIGH;
            break;
            
        /* Movimentar coxa direita */
        case 10:
            current_state = M_RIGHT_THIGH;
            break;
            
        /* Movimentar mundo */
        case 11:
            current_state = NO_STATE;
            break;
            
        /* Sair */
        case 0:
            exit(EXIT_SUCCESS);
            
        default:
            exit(EXIT_SUCCESS);
            
	}
    
	glutPostRedisplay();
}

void createMenu(void) {
	menu_option = glutCreateMenu(menu);
  
	glutAddMenuEntry("Movimentar cabeça", 1);
	glutAddMenuEntry("Movimentar tronco", 2);
	glutAddMenuEntry("Movimentar braço esquerdo", 3);
	glutAddMenuEntry("Movimentar braço direito", 4);
	glutAddMenuEntry("Movimentar antebraço esquerdo", 5);
	glutAddMenuEntry("Movimentar antebraço direito", 6);
	glutAddMenuEntry("Movimentar perna esquerda", 7);
	glutAddMenuEntry("Movimentar perna direita", 8);
	glutAddMenuEntry("Movimentar panturrilha esquerda", 9);
	glutAddMenuEntry("Movimentar panturrilha direita", 10);
	glutAddMenuEntry("Movimentar mundo", 11);
	glutAddMenuEntry("Sair", 0);
    
    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


// Callback para tratamento de eventos de teclado
void Keyboard (unsigned char key, int x, int y)
{
    //sair
    if (key == 27)
        exit(EXIT_SUCCESS);
    else if (key == 'a' || key == 'A') toggleAxes = 1-toggleAxes;
    else if (key == 'v' || key == 'V') toggleValues = 1-toggleValues;
    else if (key == 'm' || key == 'M') toggleMode = 1-toggleMode;
    /*  Muda a dimensão do boneco */
    else if (key == 'D') dimension += 0.1;
    else if (key == 'd' && dimension>1) dimension -= 0.1;

    project();
    glutPostRedisplay();
}



void move_arm(float arm[4], int key)
{
    //move braço para direita
    if (key == GLUT_KEY_RIGHT)
    {
        if (arm[1] < ARM_ANGLE_LEFT)
        {
            arm[1] += 5;
            arm[3] = 1;
        }
    }
    //move braço para esquerda
    else if (key == GLUT_KEY_LEFT)
    {
        if (arm[1] > -ARM_ANGLE_RIGHT)
        {
            arm[1] -= 5;
            arm[3] = 1;
        }
    }
    //move braço para frente
    else if (key == GLUT_KEY_UP)
    {
        if (arm[0] < ARM_ANGLE_FRONT)
        {
            arm[0] +=5;
            arm[2] = 1;
        }
    }
    //move braço para trás
    else if (key == GLUT_KEY_DOWN)
    {
        if (arm[0] > -ARM_ANGLE_BACK)
        {
            arm[0] -= 5;
            arm[2] = 1;
        }
    }
}

void move_left_arm(int key)
{
    move_arm(a_larm, key);
}

void move_right_arm(int key)
{
   move_arm(a_rarm, key);}

void move_fore_arm(float forearm[2], int key)
{
    //move antebraço para frente
    if (key == GLUT_KEY_DOWN)
    {
        if (forearm[0] < FORE_ARM_ANGLE_FRONT)
        {
            forearm[0] += 5;
            forearm[1] = 1;
        }
    }
    //move antebraço para trás
    else if (key == GLUT_KEY_UP)
    {
        if (forearm[0] > -FORE_ARM_ANGLE_BACK)
        {
            forearm[0] -= 5;
            forearm[1] = 1;
        }
    }
}

void move_left_fore_arm(int key)
{
    move_fore_arm(a_lforearm, key);
}

void move_right_fore_arm(int key)
{
    move_fore_arm(a_rforearm, key);
}

void move_leg(float leg[2], int key)
{
    //move perna para frente
    if (key == GLUT_KEY_UP)
    {
        if (leg[0] < TIGHT_ANGLE_FRONT)
        {
            leg[0] += 5;
            leg[1] = 1;
        }
    }
    //move perna para trás
    else if (key == GLUT_KEY_DOWN)
    {
        if (leg[0] > -TIGHT_ANGLE_BACK)
        {
            leg[0] -= 5;
            leg[1] = 1;
        }
    }
}

void move_left_leg(int key)
{
    move_leg(a_lleg, key);
}

void move_right_leg(int key)
{
    move_leg(a_rleg, key);
}

void move_tigh(float tigh[2], int key)
{
    //move coxa para frente
    if (key == GLUT_KEY_UP)
    {
        if (tigh[0] < LEG_ANGLE_FRONT)
        {
            tigh[0] += 5;
            tigh[1] = 1;
        }
    }
    //move coxa para trás
    else if (key == GLUT_KEY_DOWN)
    {
        if (tigh[0] > -LEG_ANGLE_BACK)
        {
            tigh[0] -= 5;
            tigh[1] = 1;
        }
    }
}

void move_left_tigh(int key)
{
    move_tigh(a_lthigh, key);
}

void move_right_tigh(int key)
{
    move_tigh(a_rthigh, key);
}

void move_head(int key)
{
    //move cabeça para direita
    if (key == GLUT_KEY_RIGHT)
    {
        if (a_head[1] < 40)
        {
            a_head[1] += 10;
            a_head[3] = 1;
        }
    }
    //move cabeça para esquerda
    else if (key == GLUT_KEY_LEFT)
    {
        if (a_head[1] > -40)
        {
            a_head[1] -= 10;
            a_head[3] = 1;
        }
    }
    //move cabeça para cima
    else if (key == GLUT_KEY_UP)
    {
        if (a_head[0] < 10)
        {
            a_head[0] += 10;
            a_head[2] = 1;
        }
    }
    //move cabeça para baixo
    else if (key == GLUT_KEY_DOWN)
    {
        if (a_head[0] > -10)
        {
            a_head[0] -= 10;
            a_head[2] = 1;
        }
    }
}

void move_world(int key)
{
    //move mundo para esquerda
    if (key == GLUT_KEY_LEFT)
    {
        a_world[1] -= 5;
    }
    //move mundo para direita
    else if (key == GLUT_KEY_RIGHT)
    {
        a_world[1] += 5;
    }
    //move mundo para cima
    else if (key == GLUT_KEY_UP)
    {
        a_world[0] += 5;
    }
    //move mundo para baixo
    else if (key == GLUT_KEY_DOWN)
    {
        a_world[0] -= 5;
    }
}

void move_trunk(int key)
{
    //move tronco para baixo
    if (key == GLUT_KEY_DOWN)
    {
        if (a_trunk[0] < TRUNK_ANGLE_FRONT)
        {
            a_trunk[0] += 5;
            a_trunk[1] = 1;
        }
    }
    //move tronco para cima
    else if (key == GLUT_KEY_UP)
    {
        if (a_trunk[0] > -TRUNK_ANGLE_BACK)
        {
            a_trunk[0] -= 5;
            a_trunk[1] = 1;
        }
    }
}


void SpecialKeyboard(int key, int x, int y)
{
    
    if (current_state == NO_STATE)
    {
        move_world(key);
    }
    
    else if (current_state == M_HEAD)
    {
        move_head(key);
    }
    
    else if ( current_state == M_TRUNK)
    {
        move_trunk(key);
    }
    else if(current_state == M_LEFT_ARM)
    {
        move_left_arm(key);
    }
    
    else if (current_state == M_RIGHT_ARM)
    {
        move_right_arm(key);
    }
    
    else if (current_state == M_LEFT_FOREARM)
    {
        move_left_fore_arm(key);
    }
    
    else if (current_state == M_RIGHT_FOREARM)
    {
        move_right_fore_arm(key);
    }
    
    else if (current_state == M_LEFT_LEG)
    {
        move_left_leg(key);
    }
    
    else if (current_state == M_RIGHT_LEG)
    {
        move_right_leg(key);
    }
    
    else if (current_state == M_LEFT_THIGH)
    {
        move_left_tigh(key);
    }
    
    else if (current_state == M_RIGHT_THIGH)
    {
        move_right_tigh(key);
    }
    
    /*  Mantém angulos em +/-360 graus */
    th %= 360;
    ph %= 360;
    
    project();
    glutPostRedisplay();
}

void InitLighting();

void TimerFunction( int value ){
    
    if( X < 15 ) delta = 0.4;
    if( X > 10 ) delta = -0.4;
    
    X += delta;
    
    glutPostRedisplay();
    glutTimerFunc( 20, TimerFunction, 1);
}
void Init(void)
{
    /* Seleciona a cor de fundo                 */
    glClearColor(0.0, 0.7, 1.0, 1.0);
//    glClearColor(0.0, 0.0, 0.0, 1.0);
    /* Habilita o teste de profundidade (Zbuffer)              */
    /* Um pixel é desenhado com a cor da superfície se esta tem*/
    /* profundidade menor ou igual ao valor atual armazenado no*/
    /* map de profundidades                                    */
    glEnable(GL_DEPTH_TEST);
    /* Faz com que as normais transformadas sejam normalizadas */
    glEnable(GL_NORMALIZE);
    
    /* glHint é uma função que estabelece o comportamento      */
    /* da OpenGL em relação a aspectos especificados pelo      */
    /* primeiro parâmetro que é uma constante.                 */
    /* No caso, os parâmetros passados para a função indicam   */
    /* que a OpenGL deve usar correção de perspectiva na       */
    /* interpolação das cores e/ou coordenadas de textura dos  */
    /* fragmentos(pixels). GL_NICEST indica que o OpenGL deve  */
    /* interpolação com correção de perspectiva e não utilizar */
    /* aproximações                                            */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    
    bobfront1    = ReadPpmImage2RGBA("/Users/Carol/Documents/UFF/UFF-Computação/Comp.Graf/TrabalhoCGPARTE2/trabalhoCG2/cgTrab2/bobfront1.ppm");
    bobfront2    = ReadPpmImage2RGBA("/Users/Carol/Documents/UFF/UFF-Computação/Comp.Graf/TrabalhoCGPARTE2/trabalhoCG2/cgTrab2/bobfront2.ppm");
    bobback      = ReadPpmImage2RGBA("/Users/Carol/Documents/UFF/UFF-Computação/Comp.Graf/TrabalhoCGPARTE2/trabalhoCG2/cgTrab2/bobback.ppm");
    bobleft      = ReadPpmImage2RGBA("/Users/Carol/Documents/UFF/UFF-Computação/Comp.Graf/TrabalhoCGPARTE2/trabalhoCG2/cgTrab2/bobleft.ppm");
    bobright     = ReadPpmImage2RGBA("/Users/Carol/Documents/UFF/UFF-Computação/Comp.Graf/TrabalhoCGPARTE2/trabalhoCG2/cgTrab2/bobright.ppm");
    bobtop       = ReadPpmImage2RGBA("/Users/Carol/Documents/UFF/UFF-Computação/Comp.Graf/TrabalhoCGPARTE2/trabalhoCG2/cgTrab2/bobtop.ppm");
    bobbottom    = ReadPpmImage2RGBA("/Users/Carol/Documents/UFF/UFF-Computação/Comp.Graf/TrabalhoCGPARTE2/trabalhoCG2/cgTrab2/bobbottom.ppm");
    
    texHandle[0] = CreateTexture(bobback,    GL_LINEAR, GL_LINEAR);
    texHandle[1] = CreateTexture(bobright,   GL_LINEAR, GL_LINEAR);
    texHandle[2] = CreateTexture(bobtop,     GL_LINEAR, GL_LINEAR);
    texHandle[3] = CreateTexture(bobleft,    GL_LINEAR, GL_LINEAR);
    texHandle[4] = CreateTexture(bobbottom,  GL_LINEAR, GL_LINEAR);
    texHandle[5] = CreateTexture(bobfront1,  GL_LINEAR, GL_LINEAR);
    
    /* Configura a iluminação                                  */
    InitLighting();
    
    /* Anima o boneco*/
    glutTimerFunc(33, TimerFunction, 1);
    /* Cria menu */
    createMenu();
}

void InitLighting()
{
    GLfloat mat_specular[]         = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[]        = {100.0};
    GLfloat light_position1[]      = {200.0, 200.0, 200.0, 1.0};
    GLfloat light_position2[]      = {-500.0, 500.0, 0.0, 1.0};
    GLfloat light_position3[]      = {700.0, -700.0, 0.0, 1.0};
    
    GLfloat white_light_specular[] = {1.0,1.0,1.0,1.0};
    GLfloat white_light_diffuse[]  = {1.0,1.0,1.0,1.0};
    
    /* Define modelo de iluminação suave. A intensidade de cor nos pixels é calculada       */
    /* com base na interpolação dos atributos definidos nos vértices durante a rasterização */
    glShadeModel(GL_SMOOTH);
    /* Habilita materiais coloridos */
    glEnable(GL_COLOR_MATERIAL);
    
    /* Caracteriza o material da superfície com especular                                   */
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    /* Define a especularidade do material. Quanto maior a especularidade mais concentrado  */
    /* é o highlight                                                                        */
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    /* Define a posicao da fonte de luz 0 */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    /* Define a intensidade da componenente difusa   da fonte de luz 1 em cada componente   */
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light_specular);
    /* Define a intensidade da componenente specular da fonte de luz 1 em cada componente   */
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light_diffuse);
    
    /* Define a posicao da fonte de luz 1 */
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    /* Define a intensidade da componenente difusa   da fonte de luz 1 em cada componente   */
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  white_light_specular);
    /* Define a intensidade da componenente specular da fonte de luz 1 em cada componente   */
    glLightfv(GL_LIGHT1, GL_SPECULAR, white_light_diffuse);
    
    /* Define a posicao da fonte de luz 2 */
    glLightfv(GL_LIGHT2, GL_POSITION, light_position3);
    /* Define a intensidade da componenente difusa   da fonte de luz 1 em cada componente   */
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  white_light_specular);
    /* Define a intensidade da componenente specular da fonte de luz 1 em cada componente   */
    glLightfv(GL_LIGHT2, GL_SPECULAR, white_light_diffuse);
    
    /* Habilita iluminação */
    glEnable(GL_LIGHTING);
    /* Habilita fonte de luz 0 */
    glEnable(GL_LIGHT0);
    /* Habilita fonte de luz 1 */
    glEnable(GL_LIGHT1);
    /* Habilita fonte de luz 2 */
    glEnable(GL_LIGHT1);
}

void Reshape (int w, int h)
{
   

    /* Configura o tamanho da janela de desenho como igual o tamanho total   */
    /* do canvas                                                             */
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    /* Seleciona a matriz de modelagem e visualização*/
    glMatrixMode (GL_PROJECTION);
    /* Carrega a matriz de modelagem e visualização com a matriz identidade  */
    glLoadIdentity ();
    /* Cria o volume de visualização(frustum). A especificação do volume de  */
    /* visualização permite a OpenGL inferir a matriz de projeção            */
    gluPerspective(65.0, (GLfloat) w/(GLfloat)h, 1.0, 500.0);
    //glOrtho(-75.0,75.0,-75.0,75.0,-500.0,500.0);
    /* Seleciona a matriz de modelagem e visualização*/
    glMatrixMode(GL_MODELVIEW);
    /* Carrega a matriz de modelagem e visualização com a matriz identidade  */
    glLoadIdentity();
    
    aspect_relation = (h>0) ? (double)w/h : 1;
    glViewport(0, 0, w, h);
    project();
}

void Idle(void) { }

int main(int argc, char** argv)
{
    /* Inicia a biblioteca Glut */
    glutInit(&argc, argv);
    /* Configura o modo de display para uso de double buffering e 4 */
    /* componentes de cor                                           */
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    /* Configura o tamanho da janela para 500x500 pixels            */
    glutInitWindowSize (500, 500);
    /* Posiciona a janela na posição (100,100) da tela              */
    glutInitWindowPosition (100, 100);
    /* Cria a janela */
    glutCreateWindow (argv[0]);
    /* Chama uma função para inicialização geral. Normalmente       */
    /* configura estados e atributos da OpenGL                      */
    Init();
    /* Registra a callback (função que é chamada para tratamento    */
    /* de eventos da interface) que cuida do desenho do canvas      */
    glutDisplayFunc(Display);
    /* Registra a callback para que trata do evento associado ao    */
    /* ao redimensionamento da janela                               */
    glutReshapeFunc(Reshape);
    /* Registra a callback para tratamento de eventos gerados pelo  */
    /* teclado                                                      */
    glutKeyboardFunc(Keyboard); 
    glutSpecialFunc(SpecialKeyboard);
    /* Entra no laço que captura e trata eventos da GLUT            */ 
    glutIdleFunc(Idle);
    glutMainLoop();
    return 0;
}