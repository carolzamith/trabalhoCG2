#include "screencasts.h"

#define PI 3.1415926535898
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
/*  D graus de rotação */
#define DEF_D 5

/*  Globais */
double dim = 5.0; /* dimensões caixa ortogonal */
char *windowName = "Desenha Robô";
int windowWidth = 500;
int windowHeight = 450;

/*  Estados */
int toggleAxes = 0;   /* alterna on/off eixos */
int toggleValues = 1; /* alterna on/off escrita na tela */
int toggleMode = 1; /* habilita modo de projeção (perspectiva) */
int th = 340;   /* azimute relacionado ao angulo */
int ph = 30;    /* elevação relaticionado ao angulo */
int fov = 55;   /* campo de visão para modo de projeção em perpectiva */
int asp = 1;    /* proporção */

/*  Vértices do cubo */
GLfloat vertA[3] = { 0.5, 0.5, 0.5};
GLfloat vertB[3] = {-0.5, 0.5, 0.5};
GLfloat vertC[3] = {-0.5,-0.5, 0.5};
GLfloat vertD[3] = { 0.5,-0.5, 0.5};
GLfloat vertE[3] = { 0.5, 0.5,-0.5};
GLfloat vertF[3] = {-0.5, 0.5,-0.5};
GLfloat vertG[3] = {-0.5,-0.5,-0.5};
GLfloat vertH[3] = { 0.5,-0.5,-0.5};

/* Projeção */
void project()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (toggleMode) {
        /* perspectiva */
        gluPerspective(fov,asp,dim/4,4*dim);
    }
    else {
        /* ortogonal */
        glOrtho(-dim*asp,+dim*asp, -dim,+dim, -dim,+dim);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* Posição da câmera */
void setEye()
{
    /* perspectiva */
    if (toggleMode) {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
    /*  orthogonal */
    else {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }
}

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

/* Desenha os valores no canto inferior esquerdo */
void drawValues()
{
    if (toggleValues) {
        glColor3f(0.8,0.8,0.8);
        printAt(5,5,"Angulos (th, ph) = (%d, %d)", th, ph);
        printAt(5,25,"Modo da projecao = (%s)", toggleMode?"Perspectiva":"Ortogonal");
    }
}

void cube(double x,double y,double z,
          double dx,double dy,double dz,
          double th)
{
    glPushMatrix();
    
    /*  Transformação do cubo */
    glTranslated(x, y, z);
    glRotated(th, 0, 1, 0);
    glScaled(dx, dy, dz);
    
    glColor3f(1.0,1.0,0.0);
    
    /* Cubo */
    glBegin(GL_QUADS);
        /* frente */
        glVertex3fv(vertA);
        glVertex3fv(vertB);
        glVertex3fv(vertC);
        glVertex3fv(vertD);
    
        /* costas */
        glVertex3fv(vertF);
        glVertex3fv(vertE);
        glVertex3fv(vertH);
        glVertex3fv(vertG);
    
        /* direita */
        glVertex3fv(vertE);
        glVertex3fv(vertA);
        glVertex3fv(vertD);
        glVertex3fv(vertH);
    
        /* esquerda */
        glVertex3fv(vertB);
        glVertex3fv(vertF);
        glVertex3fv(vertG);
        glVertex3fv(vertC);
    
        /* topo */
        glVertex3fv(vertE);
        glVertex3fv(vertF);
        glVertex3fv(vertB);
        glVertex3fv(vertA);

        /* base */
        glVertex3fv(vertD);
        glVertex3fv(vertC);
        glVertex3fv(vertG);
        glVertex3fv(vertH);
    glEnd();
    
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    
    /* setup functions */
    setEye();
    
    /* draw */
    drawAxes();
    drawValues();
    
    //desenha cabeça
    cube(1, 0, 1, 1, 1, 1, 0);
    //desenha tronco
    cube(-1 ,0, 1, 3, 2, 1, 0);
    
    glFlush();
    glutSwapBuffers();
}


void reshape(int width,int height)
{
    asp = (height>0) ? (double)width/height : 1;
    glViewport(0,0, width,height);
    project();
}


void windowKey(unsigned char key,int x,int y)
{
    /*  Sair com ESC */
    if (key == 27) exit(0);
    else if (key == 'a' || key == 'A') toggleAxes = 1-toggleAxes;
    else if (key == 'v' || key == 'V') toggleValues = 1-toggleValues;
    else if (key == 'm' || key == 'M') toggleMode = 1-toggleMode;
    /*  Muda o ângulo de visão */
    else if (key == '-' && key>1) fov--;
    else if (key == '+' && key<179) fov++;
    /*  Altera as dimensões */
    else if (key == 'D') dim += 0.1;
    else if (key == 'd' && dim>1) dim -= 0.1;
    
    project();
    glutPostRedisplay();
}

/* windowSpecial() */
void windowSpecial(int key,int x,int y)
{
    /*  Seta direita - aumenta azimute em 5 graus */
    if (key == GLUT_KEY_RIGHT) th += 5;
    /*  Seta esquerda - diminui azimute em 5 graus */
    else if (key == GLUT_KEY_LEFT) th -= 5;
    /*  Seta para cima - aumenta elevacao em 5 graus */
    else if (key == GLUT_KEY_UP) ph += 5;
    /*  Seta para baixo - diminui elevacao em 5 graus */
    else if (key == GLUT_KEY_DOWN) ph -= 5;
    
    /*  Mantem ângulos entre +/-360 graus */
    th %= 360;
    ph %= 360;
    
    project();
    glutPostRedisplay();
}

void windowMenu(int value)
{
    windowKey((unsigned char)value, 0, 0);
}


int main(int argc,char* argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow(windowName);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(windowKey);
    glutSpecialFunc(windowSpecial);
    
    glutCreateMenu(windowMenu);
    glutAddMenuEntry("Alternar eixos [a]",'a');
    glutAddMenuEntry("Alternar valores [v]",'v');
    glutAddMenuEntry("Alterar modo [m]",'m');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutMainLoop();
    return 0;
}
