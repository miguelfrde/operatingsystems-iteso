#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define DIF 16

// NOMBRE DEL ARCHIVO A PROCESAR
char filename[]="/home/usuario/imagen.bmp";

#pragma pack(2)  // Empaquetado de 2 bytes
typedef struct {
  unsigned char magic1;
  unsigned char magic2;
  unsigned int size;
  unsigned short int reserved1, reserved2;
  unsigned int pixelOffset; // offset a la imagen
} HEADER;

#pragma pack() // Empaquetamiento por default
typedef struct {
  unsigned int size; // Tamaño de este encabezado INFOHEADER
  int cols, rows; // Renglones y columnas de la imagen
  unsigned short int planes;
  unsigned short int bitsPerPixel; // Bits por pixel
  unsigned int compression;
  unsigned int cmpSize;
  int xScale, yScale;
  unsigned int numColors;
  unsigned int importantColors;
} INFOHEADER;

typedef struct {
      unsigned char red;
      unsigned char green;
      unsigned char blue;
} PIXEL;

typedef struct {
      HEADER header;
      INFOHEADER infoheader;
      PIXEL *pixel;
} IMAGE;

IMAGE imagenfte,imagendst;

int loadBMP(char *filename, IMAGE *image) {
  FILE *fin;
  int i=0;
  int totpixs=0;
  fin = fopen(filename, "rb+");

  // Si el archivo no existe
  if (fin == NULL)
    return(-1);

  // Leer encabezado
  fread(&image->header, sizeof(HEADER), 1, fin);

  // Probar si es un archivo BMP
  if (!((image->header.magic1 == 'B') && (image->header.magic2 == 'M')))
    return(-1);

  fread(&image->infoheader, sizeof(INFOHEADER), 1, fin);
  // Probar si es un archivo BMP 24 bits no compactado

  if (!((image->infoheader.bitsPerPixel == 24) && !image- >infoheader.compression))
    return(-1);

  image->pixel=(PIXEL *)malloc(sizeof(PIXEL)*image->infoheader.cols*image->infoheader.rows);
  totpixs=image->infoheader.rows*image->infoheader.cols;

  while(i<totpixs) {
    fread(image->pixel+i, sizeof(PIXEL),512, fin); i+=512;
  }

  fclose(fin);
}

int saveBMP(char *filename, IMAGE *image) {
  FILE *fout;
  int i,totpixs;
  fout=fopen(filename,"wb");
  if (fout == NULL)
    return(-1); // Error

  // Escribe encabezado
  fwrite(&image->header, sizeof(HEADER), 1, fout);

  // Escribe información del encabezado
  fwrite(&image->infoheader, sizeof(INFOHEADER), 1, fout);

  i=0;
  totpixs=image->infoheader.rows*image->infoheader.cols;
  while(i<totpixs) {
    fwrite(image->pixel+i,sizeof(PIXEL),512,fout); i+=512;
  }

  fclose(fout);
}

unsigned char blackandwhite(PIXEL p) {
  return((unsigned char) (0.3*((float)p.red)+ 0.59*((float)p.green)+0.11*((float)p.blue)));
}

void processBMP(IMAGE *imagefte, IMAGE *imagedst) {
  int i,j;
  int count=0;
  PIXEL *pfte,*pdst;
  PIXEL *v0,*v1,*v2,*v3,*v4,*v5,*v6,*v7; int imageRows,imageCols;
  memcpy(imagedst,imagefte,sizeof(IMAGE)-sizeof(PIXEL *)); imageRows = imagefte->infoheader.rows;
  imageCols = imagefte->infoheader.cols;
  imagedst->pixel=(PIXEL *)malloc(sizeof(PIXEL)*imageRows*imageCols);
  for(i=1;i<imageRows-1;i++)
    for(j=1;j<imageCols-1;j++)
    {
      pfte=imagefte->pixel+imageCols*i+j;
      v0=pfte-imageCols-1;
      v1=pfte-imageCols;
      v2=pfte-imageCols+1;
      v3=pfte-1;
      v4=pfte+1;
      v5=pfte+imageCols-1;
      v6=pfte+imageCols;
      v7=pfte+imageCols+1;

      pdst=imagedst->pixel+imageCols*i+j;
      if(abs(blackandwhite(*pfte)-blackandwhite(*v0))>DIF ||
         abs(blackandwhite(*pfte)-blackandwhite(*v1))>DIF ||
         abs(blackandwhite(*pfte)-blackandwhite(*v2))>DIF ||
         abs(blackandwhite(*pfte)-blackandwhite(*v3))>DIF ||
         abs(blackandwhite(*pfte)-blackandwhite(*v4))>DIF ||
         abs(blackandwhite(*pfte)-blackandwhite(*v5))>DIF ||
         abs(blackandwhite(*pfte)-blackandwhite(*v6))>DIF ||
         abs(blackandwhite(*pfte)-blackandwhite(*v7))>DIF) {
        pdst->red=0;
        pdst->green=0;
        pdst->blue=0;
      }
      else
      {
        pdst->red=255;
        pdst->green=255;
        pdst->blue=255;
      }
    }
}

int main() {
  int res;
  char namedest[80];
  long long start_ts;
  long long stop_ts;
  long long elapsed_time;
  long lElapsedTime;
  struct timeval ts;

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

  strcpy(namedest,strtok(filename,"."));
  strcat(filename,".bmp");
  strcat(namedest,"_P.bmp");
  printf("Archivo fuente %s\n",filename);
  printf("Archivo destino %s\n",namedest);

  res=loadBMP(filename,&imagenfte);

  if(res==-1) {
    fprintf(stderr,"Error al abrir imagen\n");
    exit(1);
  }
  printf("Procesando imagen de: Renglones = %d, Columnas = %d\n",imagenfte.infoheader.rows,imagenfte.infoheader.cols);

  processBMP(&imagenfte,&imagendst);
  res=saveBMP(namedest,&imagendst);
  if(res==-1) {
    fprintf(stderr,"Error al escribir imagen\n");
    exit(1);
  }
  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
  elapsed_time = stop_ts - start_ts;
  printf("Tiempo %2.3f segundos\n", (float)elapsed_time/1000000.0);
}
