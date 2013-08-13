#ifdef WIN32
  #include<windows.h>
#endif 

#include<stdio.h>
#include<OpenGL/glu.h>
#include"image.h"

GLint GetImageGLFormat(tpImage * image)
{
 switch(image->type)
   {
    case BITMAP: return GL_LUMINANCE;
    break;
    case RGBIM:  return GL_RGB;
    break;
    case RGBAIM: return GL_RGBA;
    break;
   }
 return -1;
}

tpImage * CreateImage(int type, int width, int height)
{
 int       numComp;
 tpImage * im;
 
 /* Aloca a image */
 im         = (tpImage*) malloc(sizeof(tpImage));
 im->width  = width;
 im->height = height;
 im->type   = type;
 
 switch (type)
   {
    case BITMAP: numComp = 1;
    break;
    case RGBIM:  numComp = 3;
    break;
    case RGBAIM: numComp = 4;
    break;
   }

 im->numComp   = numComp;

 // Aloca o vetor que armazena os bytes da image, cujo tamanho depende da resolução 
 // da imagem e do número de componentes de cor.
 //
 // Em uma imagem monocromática temos apenas uma componente de cor e, por este motivo,
 // utilizamos somente 1 byte para representar a cor de cada pixel. 
 //
 // Em uma imagem RGB utilizamos 3 bytes (1 para cada componente para representar a cor
 // de um pixel). O vetor de bits armazena as cores no seguinte formato:
 // r0b0g0r1g1b1r2g2b2r3g3b3...
 //
 // Em uma imagem RGBA utilizamos 4 bytes (1 para cada componente e 1 para o canal alfa)
 // O vetor de bits armazena as cores no seguinte formato:
 // r0b0g0a0r1g1b1a1r2g2b2a2r3g3b3a3...


 im->bits   = (unsigned char*)malloc(numComp*width*height*sizeof(unsigned char));

 return im;

}

/* Desaloca a imagem criada */
void DestroyImage(tpImage * image)
{
 if (image!=NULL)
   {
    if (image->bits!=NULL) free(image->bits);
    free(image);
   }
}

/* Lê uma linha do arquivo ppm que descreve a imagem*/
static ReadLine ( FILE *infile, char *s )
{
  char *news, *sptr;
  static char line[80]="";

  if ((strlen (line) == 0) || ((sptr = (char *)strtok (NULL, " \n\t")) == NULL))
    {
     while ((news = fgets (line, 80, infile)) != NULL)
        if ( (line[0] != '#') && ((sptr = (char*)strtok (line, " \n\t")) != NULL) )
           break;

     if (news == NULL)
       {
        printf("\nError: ReadLine - reading line\n");
        return 0;
       }
    }

  strcpy ( s, sptr );
  return 1;
}

/* Lê e cria uma imagem a partir de um arquivo no formato ppm*/
tpImage * ReadPpmImage2RGBA(char * fileName)
{
   char            s[80]="";
   int             i,j,width, height,result=1,numBytesRead;
   tpImage       * image=NULL;
   FILE          * infile;

   if ((infile=fopen(fileName,"rb"))==NULL)
     {
      printf("\nErro:Arquivo nao encontrado\n");
      return NULL;
     }
 
   fgets ( s, 4, infile );
   if ( strcmp ( s, "P6\n" ) != 0 )
     {
      printf( "\nErro: ReadPpmImage2RGBA - (P6 nao encontrado)\n");
      return NULL;
     }

   result = (result & ReadLine ( infile, s )); width  = atoi(s);
   result = (result & ReadLine ( infile, s )); height = atoi(s);
   result = (result & ReadLine ( infile, s )); /*maxval = atoi(s);*/

   /* Checa se o header está corrompido */
   if ( !result )
     {
      printf("\nErro:ReadPpm2RGBA - header corrompido\n");

      return NULL;
     }

   if ( image == NULL )
      image = CreateImage(RGBAIM,width,height);
     

   if ( image == NULL )
     {
      printf("\nErro: ReadPpm2RGBA - Imagem nao pode ser alocada\n");      
      return NULL;
     }
   
   /* Se a imagem tem tamanho zero então termine */
   if ( width == 0 || height == 0 ) return image;

   result = 1;
   for (j=0;j<height;j++)
     for (i=0;i<width;i++)
       {
        numBytesRead = 0;
        numBytesRead = fread(&(image->bits[image->numComp*(i+(height-j-1)*width)+0]),1,1,infile)+    
                       fread(&(image->bits[image->numComp*(i+(height-j-1)*width)+1]),1,1,infile)+    
                       fread(&(image->bits[image->numComp*(i+(height-j-1)*width)+2]),1,1,infile); 
        if (numBytesRead!=3)
           {
            result = 0;
            break;
           }
        /* O canal alfa fica com valor 255(1.0) o que significa que a imagem é completamente opaca.  */
        /* Deve ser alterado posteriormente para que as regiões transparentes tenham valor 0.        */
        /* Não alterar aqui. Alterar através de uma rotina sua no programa principal que varre os    */
        /* que percorre os pixels da imagem.                                                         */                              
        /* ATENÇÃO: A modificação deve ser feita antes de se criar a textura senão não haverá efeito.*/

        image->bits[image->numComp*(i+(height-j-1)*width)+3]=255;  
       }

   if (!result)
     {
      printf("\nErro: ReadPpm2RGBA - Arquivo ppm truncado\n");
      DestroyImage(image);      
      return NULL;
     }

   /* sucesso */
   return image;
}

/* Verifica se um número é potência de 2. Necessário porque a OpenGL */
/* somente aceita texturas com tamanho que seja potência de 2        */

int is2PowerMSB(int num,int * msbp)
{
 int msb,nlshifts,mask = 0x80000000;
 
 nlshifts = 0;
 msb      = 0; 

 while ((num!=0)&&(msb==0))
  {
   msb = num&mask;
   num = num<<1;
   nlshifts ++;
  }

 *msbp = 32 - nlshifts + 1;

 if (num==0)
   {
    return 1;
   }

 return 0;
}

/* Associa uma imagem a uma textura criada na OpenGL com indentificador dado por textureHandle */
void SetImageToTexture(tpImage * image,GLuint textureHandle)
{
 int format;
 
 /* Faz conexão com a textura com indentificador textureHandle        */
 glBindTexture(GL_TEXTURE_2D,textureHandle);
 /* Obtém o formato da image                                          */
 format = GetImageGLFormat(image);
 /* armazena os bits da imagem na região correspondente a textura     */
 glTexSubImage2D(GL_TEXTURE_2D,0,0,0,image->width,image->height,format,GL_UNSIGNED_BYTE,image->bits); 

}

/* Seleciona os parametros para o mapeamento de textura               */
void SetTextureParameters(GLuint textureHandle, GLuint minFilter, GLuint magFilter)
{
// glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);       
// glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);   
 
 /* Escolhe o filtro para amplicação(magnification) da textura.       */   
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,minFilter);
 /* Escolhe o filtro para redução (minification) da textura.          */          
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,magFilter);    

 /* Escolhe o modo como a textura será mapeada sobre a região.        */
 /* No caso estamos escolhendo o modo GL_REPLACE, o que significa que */
 /* a textura substituirá qualquer cor que exista na região mapeada   */
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


GLuint CreateTexture(tpImage * image,GLuint minFilter, GLuint magFilter)
{ 
 unsigned char * texels  = NULL;
 int             msbw,msbh,
                 width,height,
                 numComp,format;
 GLuint          textureHandle;

 width          = image->width;
 height         = image->height;
 numComp        = image->numComp;

 if (!is2PowerMSB(width, &msbw)) width  = 1<<msbw;    
 if (!is2PowerMSB(height,&msbh)) height = 1<<msbh;    

 width = 256;
 height = 512;

 texels = (unsigned char *)malloc(numComp*width*height*sizeof(unsigned char));
 memset(texels,0,numComp*width*height);

 glGenTextures(1,&(textureHandle));
 glBindTexture(GL_TEXTURE_2D,textureHandle);       
 SetTextureParameters(textureHandle,minFilter,magFilter);
 format = GetImageGLFormat(image);

 gluScaleImage(format, image->width, image->height, GL_UNSIGNED_BYTE, image->bits, width, height, GL_UNSIGNED_BYTE, texels);
 glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,texels);
 image->textureHandle = textureHandle;
 free(texels);

 return textureHandle;
}

/* Destrói uma textura*/
void DestroyTexture(GLuint textureHandle)
{
 glDeleteTextures(1,&(textureHandle));
}

/* Desenha uma imagem ou uma sub-região retangular da mesma na tela*/

void DrawImage(tpImage * image,       /* Ponteiro para uma imagem.                                 */
               int posx, int posy,    /* Posição da tela em que a imagem será desenhada.           */
               int sizex, int sizey,  /* Tamanho final da imagem desenhada.                        */
               int xmin, int ymin ,   /* Canto inferior esquerdo da região da imagem.              */
               int xmax, int ymax)    /* Canto superior direito  da região da imagem.              */
                                      /* Para desenhar toda a imagem basta passar os               */
                                      /* seguintes valores xmin=0,ymin=0,xmax=width, ymax = height */
{ 
  double textureCoord[4][2];
 
  /* Calcula as coordenadas de textura com base nas coordenadas da sub-região a ser desenhada e o  */
  /* tamanho da imagem. É uma simples normalização para o intervalo [0,1]                          */
  textureCoord[0][0] = (double)xmin/(double)image->width;
  textureCoord[0][1] = (double)ymin/(double)image->height;
  textureCoord[1][0] = (double)xmax/(double)image->width;
  textureCoord[1][1] = (double)ymin/(double)image->height; 
  textureCoord[2][0] = (double)xmax/(double)image->width;
  textureCoord[2][1] = (double)ymax/(double)image->height;
  textureCoord[3][0] = (double)xmin/(double)image->width;
  textureCoord[3][1] = (double)ymax/(double)image->height;

  /* Ativa o modo blending (para fazer composição dos valores que já estão no framebuffer  */
  /* com a nova imagem conforme visto em aula).                                            */
  glEnable(GL_BLEND);
  /* A cor do pixel resultante sera AI*CI+(1-AI)*CF, onde AI é o canal alfa dos pixels da  */
  /* imagem a ser desenhada, CI é a cor dos pixels da imagem e CF é a cor dos pixels que   */
  /* já estão no framebuffer (o fundo da tela ).                                           */
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  /* Habilita o modo de texturização. */
  glEnable(GL_TEXTURE_2D);
  /* Ativa a textura associada a imagem. */
  glBindTexture(GL_TEXTURE_2D,image->textureHandle);
  /* Seleciona a matriz de modelagem e visualização. */
  glMatrixMode(GL_MODELVIEW);
  /* Empilha uma nova matriz. Isso é feito para que as operações de translação              */
  /* e escala não alterem o estado original da OpenGL. Ao termino desempilharemos           */
  /* a matriz modificada e voltaremos ao estado original.                                   */
  glPushMatrix();
  /* Aplica uma translação na imagem. */
  glTranslated(posx,posy,0.0);
  /* Aplica uma escala na imagem.     */
  glScaled(sizex,sizey,0.0);
  /* Observe a ordem em que as operações são realizadas. Primeiro fazemos a escala e depois */
  /* a translação. No código OpenGL isso é especificado na ordem contrária porque reflete a */
  /* ordem em que as matrizes associadas as transformações são multiplicadas.               */  
  
  /* Desenha uma quadrilátero*/
  glBegin(GL_QUADS);
    /* Passa um vértice juntamente com suas coordenadas de textura. A OpenGL requer         */
    /* a passagem de 4 coordenadas de textura (prevê mapeamento de textura 3D projetiva.    */
    /* Aqui o mapeamento é 2D e as duas últimas coordenadas devem ser 0.0 e 1.0(será visto  */
    /* mais tarde no curso).                                                                */

    glTexCoord4d(textureCoord[0][0],textureCoord[0][1],0.0,1.0);
    glVertex2d(0.0,0.0);
    glTexCoord4d(textureCoord[1][0],textureCoord[1][1],0.0,1.0);
    glVertex2d(1.0,0.0);
    glTexCoord4d(textureCoord[2][0],textureCoord[2][1],0.0,1.0);
    glVertex2d(1.0,1.0);
    glTexCoord4d(textureCoord[3][0],textureCoord[3][1],0.0,1.0);
    glVertex2d(0.0,1.0);
  glEnd();
  /* Desempilha a matriz de modelagem e visualização que codificava as trasformações de    */
  /* escala e translação, fazendo com que voltemos ao estado original(matriz identidade    */
  /* que não aplica nenhuma transformação).                                                */
  glPopMatrix();
  /* Desabilita o modo de texturização */
  glDisable(GL_TEXTURE_2D);
  /* Desabilita o modo de composição (blending) */
  glDisable(GL_BLEND);
}
