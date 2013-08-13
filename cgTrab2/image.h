#ifndef image_h

#define image_h

#ifdef WIN32
  #include <windows.h>
#endif

#include<OpenGL/gl.h>

#define BITMAP    0 
#define RGBIM     1
#define RGBAIM    2 


typedef struct{int             width,height; /* largura e altura da imagem                                  */       

               int             numComp;      /* n�mero de componentes                                       */       

               int             type;         /* tipo BITMAP - imagem monocrom�tica                          */
                                             /*      RGB    - imagem tricrom�tica                           */
                                             /*      RGBA   - imagem tricrom�tica + canal alfa              */    

               unsigned char * bits;         /* vetor que armazena os bytes que representam a cor do pixel  */
                                             /* BITMAT c0c1c2...cn               , tamanho = width*height   */
                                             /* RGB    r0g0b0r1g1b1..rngnbn      , tamanho = 3*width*height */
                                             /* RGBA   r0g0b0a0r1g1b1a1..rngnbnan, tamanho = 4*width*height */

               GLuint          textureHandle;/* ponteiro para o mapa de textura associado a imagem          */
                                             /* necess�rio para o desenho da imagem na tela                 */ 

              }tpImage;

/* L� e retorna uma imagem descrita em um arquivo em formato ppm */
tpImage * ReadPpmImage2RGBA(char * fileName);
/* Cria uma imagem com tipo (BITMAP,RGB ou RGBA) de tamanho dado por width e height */
tpImage * CreateImage(int type, int width, int height);
/* Destr�i uma imagem */
void      DestroyImage(tpImage * image);
/* Cria uma textura especificando os modos de filtragem para amplia��o e redu��o */
GLuint    CreateTexture(tpImage * image,GLuint minFilter, GLuint magFilter);
/* Desenha uma imagem ou uma sub-regi�o retangular da mesma na tela*/
void      DrawImage(tpImage * image, int posx, int posy, int sizex, int sizey, int xmin, int ymin , int xmax, int ymax);

#endif
