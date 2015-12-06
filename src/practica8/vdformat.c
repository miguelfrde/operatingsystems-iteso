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

#define SECSIZE 512

int writeToSector(SECBOOT *sector, int driveNumber) {
  unsigned char buffer[SECSIZE];

  for (int i = 0; i < sizeof(SECBOOT); i++) {
    buffer[i] = *((char*) sector + i);
    printf("%c", buffer[i]);
  }

  if (vdwritesector(driveNumber, 0, 0, 2, 1, buffer) == -1) {
    printf("Error al escribir\n");
  }

  return 0;
}

void initializeBoot(SECBOOT *boot, char jump[], char nombre_particion[],
                    unsigned char sec_res, unsigned char sec_mapa_bits_bloques,
                    unsigned short sec_tabla_nodos_i, unsigned int sec_log_particion,
                    unsigned char sec_x_bloque, unsigned char heads, unsigned char cyls,
                    unsigned char secfis, char restante[]){

  strcpy(boot->jump, jump);
  strcpy(boot->nombre_particion, nombre_particion);
  boot->sec_res = sec_res;
  boot->sec_mapa_bits_bloques = sec_mapa_bits_bloques;
  boot->sec_tabla_nodos_i = sec_tabla_nodos_i;
  boot->sec_log_particion = sec_log_particion; 
  boot->sec_x_bloque = sec_x_bloque;
  boot->heads = heads;
  boot->cyls = cyls;
  boot->secfis = secfis;
  strcpy(boot->restante, restante);

}

int main(int argc, char* argv[]) {
  if(argc != 2){
    printf("Cylinder number not found in the args, fatal error");
    exit(-1);
  }

  int cylinderNumber = atoi(argv[1]);

  SECBOOT boot;
  
  char jump[4] = "0000";
  char nombre_particion[8] = "MIGBETOS";
  unsigned char sec_res = 1;
  unsigned char sec_mapa_bits_bloques = 16;
  unsigned short sec_tabla_nodos_i = 18;
  unsigned int sec_log_particion = 130559;
  unsigned char sec_x_bloque = 2;
  unsigned char heads = 48;
  unsigned char cyls = 160;
  unsigned char secfis = 17;
  char restante[486];
  
  memset(restante, 0, 486);
  
  initializeBoot(&boot, jump, nombre_particion, sec_res, sec_mapa_bits_bloques, sec_tabla_nodos_i, sec_log_particion, sec_x_bloque, heads, cyls, secfis, restante);

  writeToSector(&boot, cylinderNumber);
  return 0;
}
