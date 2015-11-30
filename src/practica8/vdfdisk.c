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
  /*
  strcpy(buffer, sector.bootstrap_code);
  strcat(buffer, sector.partition[0].drive_status);
  */
  return 0;
}

int main(int argc, char *argv[]){
  //Reading the drive number from the args
  int driveNumber = atoi(argv[1]);

  //MBR
  MBR firstSector;
  char bootstrap_code[446];
  strcpy(bootstrap_code, "ASSEMBLY CODE HERE");
  PARTITION partition;
  short boot_signature;

  //Partition default values
  int drive_status = 1;
  char chs_begin[3];
  strcpy(chs_begin, "000");
  char partition_type = 'f';
  char chs_end[3];
  strcpy(chs_end, "FFF");
  int lba = 0;
  int secs_partition = 0;
  

  //Initialize partition
  partition.drive_status = drive_status;
  strcpy(partition.chs_begin, chs_begin); 
  partition.partition_type = 'F';
  strcpy(partition.chs_end, chs_end);
  partition.lba = lba;
  secs_partition = secs_partition;

  //Initialize MBR
  strcpy(firstSector.bootstrap_code, bootstrap_code);
  firstSector.partition[0] = partition;

  //writeToSector(firstSector, driveNumber);
  return 0;
}
