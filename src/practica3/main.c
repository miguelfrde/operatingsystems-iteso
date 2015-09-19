#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define DIF 16

// File top process
char filename[] = "/home/usuario/imagen.bmp";

#pragma pack(2)  // 2 bytes packaging
typedef struct {
  unsigned char magic1;
  unsigned char magic2;
  unsigned int size;
  unsigned short int reserved1, reserved2;
  unsigned int pixel_offset; // image offset
} Header;

#pragma pack() // Default packaging
typedef struct {
  unsigned int size; // Size of this header
  int cols, rows; // Image columns and rows
  unsigned short int planes;
  unsigned short int bits_per_pixel; // Bits per pixel
  unsigned int compression;
  unsigned int cmp_size;
  int x_scale, y_scale;
  unsigned int num_colors;
  unsigned int important_colors;
} InfoHeader;

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} Pixel;

typedef struct {
  Header header;
  InfoHeader infoheader;
  Pixel *pixel;
} Image;

Image source_image, dest_image;

int load_bmp(char* filename, Image* image) {
  int totpixs = 0;
  FILE* fp = fopen(filename, "rb+");

  if (fp == NULL) {
    return -1;
  }

  // Read header
  fread(&image->header, sizeof(Header), 1, fp);

  // Check if its a BMP file
  if (!(image->header.magic1 == 'B' && image->header.magic2 == 'M')) {
    return -1;
  }

  fread(&image->infoheader, sizeof(InfoHeader), 1, fp);

  // Check if it's a BMP file 24 bits uncompressed
  if (image->infoheader.bits_per_pixel != 24 || image->infoheader.compression) {
    return -1;
  }

  image->pixel = (Pixel*)malloc(sizeof(Pixel) * image->infoheader.cols * image->infoheader.rows);
  totpixs = image->infoheader.rows * image->infoheader.cols;

  for (int i = 0; i < totpixs; i += 512) {
    fread(image->pixel + i, sizeof(Pixel), 512, fp);
  }

  fclose(fp);

  return 0;
}

int save_bmp(char *filename, Image *image) {
  int totpixs;
  FILE* fp = fopen(filename,"wb");

  if (fp == NULL) {
    return -1;
  }

  // Write header
  fwrite(&image->header, sizeof(Header), 1, fp);

  // Write header info
  fwrite(&image->infoheader, sizeof(InfoHeader), 1, fp);

  totpixs = image->infoheader.rows*image->infoheader.cols;
  for (int i = 0; i < totpixs; i += 512) {
    fwrite(image->pixel + i, sizeof(Pixel), 512, fp);
  }

  fclose(fp);

  return 0;
}

unsigned char black_and_white(Pixel p) {
  float red = (float) p.red,
        green = (float) p.green,
        blue = (float) p.blue;
  return (unsigned char) (0.3*red + 0.59*green + 0.11*blue);
}

void process_bmp(Image *imagefte, Image *imagedst) {
  int image_rows, image_cols;
  Pixel *pfte, *pdst, *v0, *v1, *v2, *v3, *v4, *v5, *v6, *v7;

  memcpy(imagedst, imagefte, sizeof(Image) - sizeof(Pixel*));
  image_rows = imagefte->infoheader.rows;
  image_cols = imagefte->infoheader.cols;
  imagedst->pixel = (Pixel *)malloc(sizeof(Pixel) * image_rows * image_cols);

  for (int i = 1; i < image_rows - 1; i++) {
    for (int j = 1; j < image_cols - 1; j++) {
      pfte = imagefte->pixel + image_cols*i + j;
      v0 = pfte - image_cols - 1;
      v1 = pfte - image_cols;
      v2 = pfte - image_cols + 1;
      v3 = pfte - 1;
      v4 = pfte + 1;
      v5 = pfte + image_cols - 1;
      v6 = pfte + image_cols;
      v7 = pfte + image_cols + 1;

      pdst = imagedst->pixel + image_cols*i + j;
      if (abs(black_and_white(*pfte) - black_and_white(*v0)) > DIF ||
          abs(black_and_white(*pfte) - black_and_white(*v1)) > DIF ||
          abs(black_and_white(*pfte) - black_and_white(*v2)) > DIF ||
          abs(black_and_white(*pfte) - black_and_white(*v3)) > DIF ||
          abs(black_and_white(*pfte) - black_and_white(*v4)) > DIF ||
          abs(black_and_white(*pfte) - black_and_white(*v5)) > DIF ||
          abs(black_and_white(*pfte) - black_and_white(*v6)) > DIF ||
          abs(black_and_white(*pfte) - black_and_white(*v7)) > DIF) {
        pdst->red = 0;
        pdst->green = 0;
        pdst->blue = 0;
      } else {
        pdst->red = 255;
        pdst->green = 255;
        pdst->blue = 255;
      }
    }
  }
}

int main() {
  int res;
  char namedest[80];
  long long start_ts;
  long long stop_ts;
  long long elapsed_time;
  struct timeval ts;

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Initial time

  strcpy(namedest, strtok(filename, "."));
  strcat(filename, ".bmp");
  strcat(namedest, "_P.bmp");
  printf("Source file: %s\n", filename);
  printf("Dest file: %s\n", namedest);

  res = load_bmp(filename, &source_image);

  if (res == -1) {
    fprintf(stderr,"Error when opening the image\n");
    exit(1);
  }

  printf("Processing image with %d rows and %d columns\n",
      source_image.infoheader.rows, source_image.infoheader.cols);

  process_bmp(&source_image, &dest_image);
  res=save_bmp(namedest, &dest_image);

  if (res == -1) {
    fprintf(stderr,"Error when writting the image\n");
    exit(1);
  }

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Final time
  elapsed_time = stop_ts - start_ts;
  printf("Time: %2.3f seconds\n", (float)elapsed_time/1000000.0);

  return 0;
}
