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
  }

  if (vdwritesector(driveNumber, 0, 0, 1, 1, buffer) == -1) {
    printf("Error al escribir\n");
  }

  return 0;
}


int main(int argc, char *argv[]) {
  //Reading the drive number from the args
  int driveNumber = atoi(argv[1]);

  MBR firstSector;
  char bootstrap_code[446];

  //Fills with A's
  memset(bootstrap_code, 65, 446);

  PARTITION partition;
  short boot_signature = 1;

  //Initialize PARTITION
  partition.drive_status = 1;
  strcpy(partition.chs_begin, "000");
  partition.partition_type = 'A';
  strcpy(partition.chs_end, "FFF");
  partition.lba = 0;
  partition.secs_partition = 130559;

  //Initialize MBR
  strcpy(firstSector.bootstrap_code, bootstrap_code);
  firstSector.partition[0] = partition;
  firstSector.boot_signature = boot_signature;

  writeToSector(&firstSector, driveNumber);

  return 0;
}
