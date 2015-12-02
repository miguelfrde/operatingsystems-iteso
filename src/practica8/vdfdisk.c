/*
3.1
This program will create the data structures and necessary tables in the first disk sector
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vdlib.h"
#include "vdisk.h"
#include <string.h>

#define SECSIZE 512

int writeToSector(MBR *sector, int driveNumber) {
  unsigned char buffer[SECSIZE];

  for (int i = 0; i < sizeof(MBR); i++) {
    buffer[i] = *((char*) sector + i);
    printf("%c", buffer[i]);
  }

  if (vdwritesector(driveNumber, 0, 0, 1, 1, buffer) == -1) {
    printf("Error al escribir\n");
  }

  return 0;
}

int main(int argc, char *argv[]) {
  //Reading the drive number from the args
  int driveNumber = atoi(argv[1]);

  //MBR
  MBR firstSector;
  char bootstrap_code[446];
  
  for (int i = 0; i < 446; i++) {
    bootstrap_code[i] = 'A';
  }

  PARTITION partition;
  short boot_signature = 1;

  //Partition default values
  int drive_status = 49;
  char chs_begin[3];
  strcpy(chs_begin, "00P");
  char partition_type = 'A';
  char chs_end[3];
  strcpy(chs_end, "NDA");
  int lba = 49; //ascci value: 1
  int secs_partition = 49; 

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
  firstSector.boot_signature = boot_signature;

  writeToSector(&firstSector, driveNumber);

  return 0;
}
