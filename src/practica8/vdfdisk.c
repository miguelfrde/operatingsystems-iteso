/*
3.1
This program will create the data structures and necessary tables in the first disk sector
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.h"
#include "vdisk.h"
#include <string.h>

#define SECSIZE 512
int writeToSector(MBR sector, int driveNumber){
  
  char buffer[SECSIZE];
  
  printf("%s", sector.bootstrap_code);
  /*for(unsigned int i = 0; i<sizeof(sector); i++){
    buffer[i] = *((char*)(&sector) + i); 
  }*/
  memcpy(buffer, sector.bootstrap_code, sizeof(sector));

  //debugging what's inside the buffer
  for(unsigned int i = 0; i<sizeof(sector); i++){
    printf("%c", buffer[i]); 
  }

  if(vdwritesector(driveNumber, 0, 0, 1, 1, buffer) == -1){
    printf("Error al escribir\n"); 
  }
 
  char readBuffer[SECSIZE];
  if(vdreadsector(driveNumber, 0, 0, 1, 1, readBuffer) == -1){
    printf("Error al leer\n"); 
  }
  
  printf("Read buffer: \n%s\n", readBuffer); 

  return 0;
}

int main(int argc, char *argv[]){
  //Reading the drive number from the args
  int driveNumber = atoi(argv[1]);

  //MBR
  MBR firstSector;
  char bootstrap_code[446];
  for (int i = 0; i < 447; i++){
    bootstrap_code[i] = 'A';
  }
  //strcpy(bootstrap_code, "ASSEMBLY CODE HERE");
  PARTITION partition;
  short boot_signature = 1;

  //Partition default values
  int drive_status = 49;
  char chs_begin[3];
  strcpy(chs_begin, "00P");
  char partition_type = 'A';
  char chs_end[3];
  strcpy(chs_end, "NDA");
  int lba = 48;
  int secs_partition = 48;
 
  //Initialize partition
  partition.drive_status = drive_status;
  strcpy(partition.chs_begin, chs_begin); 
  partition.partition_type = partition_type;
  strcpy(partition.chs_end, chs_end);
  partition.lba = lba;
  secs_partition = secs_partition;


  //Initialize MBR
  strcpy(firstSector.bootstrap_code, bootstrap_code);
  firstSector.partition[0] = partition;
  firstSector.partition[1] = partition;
  firstSector.partition[2] = partition;
  firstSector.partition[3] = partition;
  firstSector.boot_signature = boot_signature;

  writeToSector(firstSector, driveNumber);
  return 0;
}
