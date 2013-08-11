//
//  main.c
//  cgTrab2
//
//  Created by Marcelle Guine on 11/08/13.
//  Copyright (c) 2013 Marcelle Guine. All rights reserved.
//

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <math.h>
#include "print.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1415926535898
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))

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
int toggleAxes = 0;   /* eixos on/off */
int th = 0;  /* azimute do angulo de visão */
int ph = 0;  /* elevação do angulo de visão */
int fov = 60;   /* campo de visão */
//int fov = 30;   /* campo de visão */
int asp = 1.0;    /* relação de aspecto */

GLubyte v_colors[][3]   = {
    {  0,  0,  0}, /* black  */
    {255,  0,  0}, /* red    */
    {255,255,  0}, /* yellow */
    {  0,255,  0}, /* green  */
    {  0,  0,255}, /* blue   */
    {255,  0,255}, /* violet */
    {255,255,255}, /* white  */
    {  0,255,255}};/* cyan   */


/* Desenha os eixos */
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
        /*  Labels */
        glRasterPos3d(len,0,0);
        print("X");
        glRasterPos3d(0,len,0);
        print("Y");
        glRasterPos3d(0,0,len);
        print("Z");
    }
}

void project()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(fov, asp, 1.25, 20);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setEye()
{
    double Ex = -10*Sin(th)*Cos(ph);
    double Ey = +10        *Sin(ph);
    double Ez = +10*Cos(th)*Cos(ph);
    gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
}

void drawKnuckle(){
    glPushMatrix();
    
    glColor3ubv(v_colors[5]);
    glScalef(0.15, 0.15, 0.15);
    glutSolidSphere(1, 16, 16);
    
    glPopMatrix();
}

void drawHead()
{
    glPushMatrix();
    
    //chapéu
    glColor3ubv(v_colors[6]);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.4);
    glutSolidCone(0.5, 1.2, 10, 1);
    glPopMatrix();

    //cabeça
    glColor3ubv(v_colors[2]);
    glScalef(0.8, 0.8, 0.8);
    glutSolidCube(1);
    
    //pescoço
    glTranslatef(0.0, -0.65, 0.0);
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

void drawArm()
{    
    glPushMatrix();
    
    //ombro
    drawKnuckle();
    
    //antebraço
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.55, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glutSolidCube(1);
    glPopMatrix();

    //cotovelo
    glTranslatef(0.0, -0.55, 0.0);
    drawKnuckle();
    
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

void drawLeg()
{
    //anti coxa
    drawKnuckle();
    
    //coxa
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.65, 0.0);
    glPushMatrix();
    glScalef(0.3, 1, 0.3);
    glutSolidCube(1);
    glPopMatrix();
    
    //joelho
    glTranslatef(0.0, -0.65, 0.0);
    drawKnuckle();
    
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
    
}

void Display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    drawAxes();
//    setEye();
    
    glTranslatef (0.0, 0.0, -8.0);
    glRotated(ph, 1, 0, 0);
    glRotated(th, 0, 1, 0);
    
    glPushMatrix();

    //tronco
    drawTrunk();
    
    //cabeça
    glPushMatrix();
    glTranslatef (0.0, 1.45, 0.0);
    drawHead();
    glPopMatrix();
    
    //braço esquerdo
    glPushMatrix();
    glTranslatef(-0.75, 0.4, 0.0);
    glRotatef(-15.0, 0.0, 0.0, 1.0);
    drawArm();
    glPopMatrix();
    
    //braço direito
    glPushMatrix();
    glTranslatef(0.75, 0.4, 0.0);
    glRotatef(15.0, 0.0, 0.0, 1.0);
    drawArm();
    glPopMatrix();
    
    glPopMatrix(); 
    
    //perna esquerda
    glPushMatrix();
    glTranslatef(-0.3, -1.0, 0.0);
    drawLeg();
    glPopMatrix();
    
    //perna direita
    glPushMatrix();
    glTranslatef(0.3, -1.0, 0.0);
    drawLeg();
    glPopMatrix();
    
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
	glutAddMenuEntry("Movimentar coxa esquerda", 9);
	glutAddMenuEntry("Movimentar coxa direita", 10);
	glutAddMenuEntry("Movimentar mundo", 11);
	glutAddMenuEntry("Sair", 0);
    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Callback para tratamento de eventos de teclado
void Keyboard (unsigned char key, int x, int y)
{
    //sair
    if (key == 27)
    {
        exit(EXIT_SUCCESS);
    }
    //exibir eixos
    else if (key == 'a' || key == 'A')
    {
        toggleAxes = 1 - toggleAxes;
    }
    
    project();
    glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
    if (current_state == NO_STATE)
    {
        //move mundo para esquerda
        if (key == GLUT_KEY_LEFT)
        {
            th -= 5;
        }
        //move mundo para direita
        else if (key == GLUT_KEY_RIGHT)
        {
            th += 5;
        }
        //move mundo para cima
        else if (key == GLUT_KEY_UP)
        {
             ph += 5;
        }
        //move mundo para baixo
        else if (key == GLUT_KEY_DOWN)
        {
             ph -= 5;
        }
    }
    
    else if (current_state == M_HEAD)
    {
        //move cabeça para direita
        if (key == GLUT_KEY_RIGHT)
        {
            
        }
        //move cabeça para esquerda
        else if (key == GLUT_KEY_LEFT)
        {
        
        }
        //move cabeça para cima
        else if (key == GLUT_KEY_UP)
        {
            
        }
        //move cabeça para baixo
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if ( current_state == M_TRUNK)
    {
        //move tronco para cima
        if (key == GLUT_KEY_UP)
        {
            
        }
        //move tronco para baixo
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_LEFT_ARM)
    {
        //move braço esquedo para direita
        if (key == GLUT_KEY_RIGHT)
        {
            
        }
        //move braço esquerdo para esquerda
        else if (key == GLUT_KEY_LEFT)
        {
            
        }
        //move braço esquerdo para frente
        else if (key == GLUT_KEY_UP)
        {
            
        }
        //move braço esquerdo para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_RIGHT_ARM)
    {
        //move braço direito para direita
        if (key == GLUT_KEY_RIGHT)
        {
            
        }
        //move braço direito para esquerda
        else if (key == GLUT_KEY_LEFT)
        {
            
        }
        //move braço direito para frente
        else if (key == GLUT_KEY_UP)
        {
            
        }
        //move braço direito para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_LEFT_FOREARM)
    {
        //move antebraço esquerdo para frente
        if (key == GLUT_KEY_UP)
        {
            
        }
        //move antebraço esquerdo para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_RIGHT_FOREARM)
    {
        //move antebraço direito para frente
        if (key == GLUT_KEY_UP)
        {
            
        }
        //move antebraço direito para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_LEFT_LEG)
    {
        //move perna esquerda para frente
        if (key == GLUT_KEY_UP)
        {
            
        }
        //move perna esquerda para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_RIGHT_LEG)
    {
        //move perna direita para frente
        if (key == GLUT_KEY_UP)
        {
            
        }
        //move perna direita para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_LEFT_THIGH)
    {
        //move coxa esquerda para frente
        if (key == GLUT_KEY_UP)
        {
            
        }
        //move coxa esquerda para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    else if (current_state == M_RIGHT_THIGH)
    {
        //move coxa direita para frente
        if (key == GLUT_KEY_UP)
        {
            
        }
        //move coxa direita para trás
        else if (key == GLUT_KEY_DOWN)
        {
            
        }
    }
    
    /*  Mantém angulos em +/-360 graus */
    th %= 360;
    ph %= 360;
    
    project();
    glutPostRedisplay();
}

void InitLighting();

void Init(void)
{
    /* Seleciona a cor negra como cor de fundo                 */
    glClearColor (0.0, 0.0, 0.0, 0.0);
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
    
    /* Configura a iluminação                                  */
    InitLighting();
    
    /* Cria menu */
    createMenu();
}

void InitLighting()
{
    GLfloat mat_specular[]         = {1.0,1.0,1.0,1.0};
    GLfloat mat_shininess[]        = {100.0};
    GLfloat light_position1[]      = {200.0,200.0,200.0,1.0};
    GLfloat light_position2[]      = {-500.0,500.0,0.0,1.0};
    
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
    
    /* Habilita iluminação */
    glEnable(GL_LIGHTING);
    /* Habilita fonte de luz 0 */
    //glEnable(GL_LIGHT0);
    /* Habilita fonte de luz 1 */
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
    
    asp = (h>0) ? (double)w/h : 1;
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