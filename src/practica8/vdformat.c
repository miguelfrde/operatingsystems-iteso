/*
3.2.1
This program will create the data structures and necessary tables in the partition created
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vdlib.h"
#include "vdisk.h"
#include <string.h>


int writeToSector(SECBOOT *sector, int driveNumber) {
  unsigned char buffer[BLOCKSMAP_SIZE];

  for (int i = 0; i < sizeof(SECBOOT); i++) {
    buffer[i] = *((char*) sector + i);
  }

  if (vdwritesector(driveNumber, 0, 0, 2, 1, buffer) == -1) {
    printf("Error al escribir\n");
  }

  return 0;
}


int main(int argc, char* argv[]) {
  if(argc != 2){
    printf("Cylinder number not found in the args, fatal error");
    exit(-1);
  }

  int cylinderNumber = atoi(argv[1]);

  SECBOOT boot;
  
  char restante[486];
  memset(restante, 0, 486);
  
  strcpy(boot.jump, "0000");
  strcpy(boot.nombre_particion, "MIGBETOS");
  boot.sec_res = 1;
  boot.sec_mapa_bits_bloques = 16;
  boot.sec_tabla_nodos_i = 15;
  boot.sec_log_particion = 130559; 
  boot.sec_x_bloque = 2;
  boot.heads = HEADS;
  boot.cyls = CYLINDERS;
  boot.secfis = SECTORS;
  strcpy(boot.restante, restante);


  writeToSector(&boot, cylinderNumber);
  return 0;
}
