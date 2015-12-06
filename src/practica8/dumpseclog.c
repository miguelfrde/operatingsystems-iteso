#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"
#include "vdisk.h"
#include "vdlib.h"

#define LINESIZE 16
#define SECSIZE 512

int main(int argc, char *argv[]) {
  int sector;
  char buffer[SECSIZE];
  unsigned char c;

  if (argc == 2) {
    sector = atoi(argv[1]);
    if (sector > SECTORS || sector < 0) {
      fprintf(stderr, "Posición invalida\n");
      exit(1);
    }
    printf("Desplegando de disco el sector logico %d\n", sector);
  } else {
    fprintf(stderr, "Error en los argumentos\n");
    exit(1);
  }

  if (vdreadseclog(sector, buffer) == -1) {
    fprintf(stderr, "Error al abrir disco virtual\n");
    exit(1);
  }

  for (int i = 0; i < SECSIZE / LINESIZE; i++) {
    printf("\n %3X -->", i * LINESIZE);

    for (int j = 0; j < LINESIZE; j++) {
      c = buffer[i*LINESIZE + j];
      printf("%2X ", c);
    }

    printf("  |  ");

    for (int j = 0; j < LINESIZE; j++) {
      c = buffer[i*LINESIZE + j] % 256;

      if (c > 0x1F && c < 127) {
        printf("%c",c);
      } else {
        printf(".");
      }
    }
  }
  printf("\n");

  return 0;
}

