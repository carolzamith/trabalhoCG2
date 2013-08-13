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

               int             numComp;      /* número de componentes                                       */       

               int             type;         /* tipo BITMAP - imagem monocromática                          */
                                             /*      RGB    - imagem tricromática                           */
                                             /*      RGBA   - imagem tricromática + canal alfa              */    

               unsigned char * bits;         /* vetor que armazena os bytes que representam a cor do pixel  */
                                             /* BITMAT c0c1c2...cn               , tamanho = width*height   */
                                             /* RGB    r0g0b0r1g1b1..rngnbn      , tamanho = 3*width*height */
                                             /* RGBA   r0g0b0a0r1g1b1a1..rngnbnan, tamanho = 4*width*height */

               GLuint          textureHandle;/* ponteiro para o mapa de textura associado a imagem          */
                                             /* necessário para o desenho da imagem na tela                 */ 

              }tpImage;

/* Lê e retorna uma imagem descrita em um arquivo em formato ppm */
tpImage * ReadPpmImage2RGBA(char * fileName);
/* Cria uma imagem com tipo (BITMAP,RGB ou RGBA) de tamanho dado por width e height */
tpImage * CreateImage(int type, int width, int height);
/* Destrói uma imagem */
void      DestroyImage(tpImage * image);
/* Cria uma textura especificando os modos de filtragem para ampliação e redução */
GLuint    CreateTexture(tpImage * image,GLuint minFilter, GLuint magFilter);
/* Desenha uma imagem ou uma sub-região retangular da mesma na tela*/
void      DrawImage(tpImage * image, int posx, int posy, int sizex, int sizey, int xmin, int ymin , int xmax, int ymax);

#endif
