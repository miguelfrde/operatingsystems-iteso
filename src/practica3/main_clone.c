#define _GNU_SOURCE           
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>

#define DIF 16
#define NUM_THREADS 4
#define STACK_SIZE (1024*12600) 


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

typedef struct {
  Image image;
  int tid;
} ThreadContext;

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

void* process_bmp(void* arg) {
  //printf("Arrived the function");
  int image_rows, image_cols;
  int thread_id = *((int*)arg);
  Pixel *psrc, *pdst, *v0, *v1, *v2, *v3, *v4, *v5, *v6, *v7;

  image_rows = source_image.infoheader.rows;
  image_cols = source_image.infoheader.cols;
  
  // printf("Image rows: %d Image cols: %d", image_rows, image_cols);
  for (int i = 1; i < image_rows - 1; i++) {
    for (int j = 1; j < image_cols - 1; j++) {
      if ((i * (image_rows - 1) + j) % NUM_THREADS != thread_id) {
        continue;
      }
      psrc = source_image.pixel + image_cols*i + j;
      v0 = psrc - image_cols - 1;
      v1 = psrc - image_cols;
      v2 = psrc - image_cols + 1;
      v3 = psrc - 1;
      v4 = psrc + 1;
      v5 = psrc + image_cols - 1;
      v6 = psrc + image_cols;
      v7 = psrc + image_cols + 1;

      pdst = dest_image.pixel + image_cols*i + j;
      // printf("Pixel source red: %d\n", psrc->red);
      if (abs(black_and_white(*psrc) - black_and_white(*v0)) > DIF ||
          abs(black_and_white(*psrc) - black_and_white(*v1)) > DIF ||
          abs(black_and_white(*psrc) - black_and_white(*v2)) > DIF ||
          abs(black_and_white(*psrc) - black_and_white(*v3)) > DIF ||
          abs(black_and_white(*psrc) - black_and_white(*v4)) > DIF ||
          abs(black_and_white(*psrc) - black_and_white(*v5)) > DIF ||
          abs(black_and_white(*psrc) - black_and_white(*v6)) > DIF ||
          abs(black_and_white(*psrc) - black_and_white(*v7)) > DIF) {
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
  return 0;
}


int main(int argc, char* argv[]) {
  int res, image_rows, image_cols;
  char namedest[80];
  char filename[80];
  long long start_ts;
  long long stop_ts;
  long long elapsed_time;
  //pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];
  struct timeval ts;
  char *stack[3];
  char *stackTop[3];

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Initial time

  if (argc < 2) {
    printf("Usage: ./main filename\n");
    exit(-1);
  }

  strcpy(filename, argv[1]);
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

  image_rows = source_image.infoheader.rows;
  image_cols = source_image.infoheader.cols;

  printf("Processing image with %d rows and %d columns\n",
      image_rows, image_cols);

  // Initialize destination image
  memcpy(&dest_image, &source_image, sizeof(Image) - sizeof(Pixel*));
  dest_image.pixel = (Pixel *)malloc(sizeof(Pixel) * image_rows * image_cols);

  for(int i = 0; i < NUM_THREADS; i++){
  stack[i] = malloc(STACK_SIZE);
  if (stack == NULL)
    return -1;
  
  stackTop[i] = stack[i] + STACK_SIZE;  /* Assume stack grows downward */
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    clone(process_bmp, (char*)stackTop[i], CLONE_VM |
     CLONE_FILES | CLONE_FS | SIGCHLD | CLONE_SIGHAND, (void*)&thread_ids[i]);
  }

  for(int i = 0; i < NUM_THREADS; i++) {
    wait(NULL);
  }

  res = save_bmp(namedest, &dest_image);

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
