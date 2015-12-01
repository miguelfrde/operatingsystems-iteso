#include <time.h>
#include "vdisk.h"
#include "vdlib.h"

#define BLOCKSMAP_SIZE 512

/********************
 * Global variables *
 ********************/

SECBOOT secboot;
int secboot_en_memoria = 0;
int blocksmap_en_memoria = 0;
int inicio_area_datos;
int mapa_bits_bloques;
char blocksmap[BLOCKSMAP_SIZE];

/*************************************************
 * Functions to read and write to a logic sector *
 *************************************************/

int vdwriteseclog(int sec_log, char *buffer) {
  // TODO
  return 0;
}

int vdreadseclog(int sec_log, char *buffer) {
  // TODO
  return 0;
}

/**************************************************
 * Functions used for the bitmap in the data area *
 **************************************************/

/**
 * Checks if a given block is free or not (1 = free)
 */
int isblockfree(int block) {
  int offset = block / 8;
  int shift = block % 8;

  // We need to have the bootsector of the partition in memory, since
  // the necessary parameters to know in which logical sector the bitmap
  // begins
  if (!secboot_en_memoria) {
    vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
    secboot_en_memoria = 1;
  }

  mapa_bits_bloques = secboot.sec_res;

  // If the bitmap is not in memory, load it
  if (!blocksmap_en_memoria) {
    for (int i = 0; i < secboot.sec_mapa_bits_bloques; i++) {
      vdreadseclog(mapa_bits_bloques + i, blocksmap + i*512);
    }
    blocksmap_en_memoria = 1;
  }

  return (blocksmap[offset] & (1<<shift))? 0 : 1;
}

/**
 * Find the next free block in the bitmap
 */
int nextfreeblock() {
  int i, j;

  if (!secboot_en_memoria) {
    vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
    secboot_en_memoria = 1;
  }

  mapa_bits_bloques = secboot.sec_res;

  if (!blocksmap_en_memoria) {
    for (int i = 0; i < secboot.sec_mapa_bits_bloques; i++) {
      vdreadseclog(mapa_bits_bloques + i, blocksmap + i*512);
    }
    blocksmap_en_memoria = 1;
  }

  // Go through each byte in the bitmap until a byte that is not 0xFF
  // is found
  i = 0;
  while (blocksmap[i] == 0xFF && i < secboot.sec_mapa_bits_bloques*512) {
    i++;
  }

  // If we find a byte in the bitmap with a bit = 0, that means that
  // there's a free block
  if (i < secboot.sec_mapa_bits_bloques * 512) {
    j = 0;
    while (blocksmap[i] & (1 << j) && j < 8) {
      j++;
    }
    return i*8 + j;
  }

  return -1; // No blocks available in the partition
}

/**
 * Assign a block, write in the bitmap that the block is in use (1)
 */
int assignblock(int block) {
  int offset = block / 8;
  int shift = block % 8;
  int sector;

  if (!secboot_en_memoria) {
    vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
    secboot_en_memoria = 1;
  }

  mapa_bits_bloques= secboot.sec_res;

  if (!blocksmap_en_memoria) {
    for (int i = 0; i < secboot.sec_mapa_bits_bloques; i++) {
      vdreadseclog(mapa_bits_bloques + i, blocksmap + i*512);
    }
    blocksmap_en_memoria = 1;
  }

  // Del byte que corresponde al bloque, poner en 1 el bit
  // From the byte that belongs to the block, put the bit = 1
  blocksmap[offset] |= 1 << shift;

  // Update the bitmap
  sector = offset / 512;
  vdwriteseclog(mapa_bits_bloques + sector, blocksmap + sector*512);
  return 1;
}


/**
 * Unassigns a block, writes 0 in the bitmap to represent that the block
 * is not in use anymore
 */
int unassignblock(int block) {
  int offset=block / 8;
  int shift = block % 8;
  int sector;

  if (!secboot_en_memoria) {
    vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
    secboot_en_memoria = 1;
  }

  mapa_bits_bloques= secboot.sec_res;

  if (!blocksmap_en_memoria) {
    for (int i = 0; i < secboot.sec_mapa_bits_bloques; i++) {
      vdreadseclog(mapa_bits_bloques + i, blocksmap + i*512);
    }
    blocksmap_en_memoria = 1;
  }

  blocksmap[offset] &= (char) ~(1 << shift);

  sector = offset / 512;
  vdwriteseclog(mapa_bits_bloques + sector, blocksmap + sector*512);
  return 1;
}

/**********************************
 * Block read and write functions *
 **********************************/

int writeblock(int block,char *buffer) {
  if (!secboot_en_memoria) {
    // Read the logical sector 0 of the partition
    vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
    secboot_en_memoria = 1;
  }

  inicio_area_datos = secboot.sec_res + secboot.sec_mapa_bits_bloques + secboot.sec_tabla_nodos_i;

  // Write all the sectors that belong to the block
  for (int i = 0; i < secboot.sec_x_bloque; i++) {
    vdwriteseclog(inicio_area_datos + (block - 1) * secboot.sec_x_bloque + i, buffer + 512*i);
  }
  return 1;
}

int readblock(int block, char *buffer) {
  if (!secboot_en_memoria) {
    vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
    secboot_en_memoria = 1;
  }

  inicio_area_datos = secboot.sec_res + secboot.sec_mapa_bits_bloques + secboot.sec_tabla_nodos_i;

  for (int i = 0; i < secboot.sec_x_bloque; i++) {
    vdreadseclog(inicio_area_datos + (block - 1)*secboot.sec_x_bloque + i,buffer + 512*i);
  }
  return 1;
}

/*************************************************
 * Functions for datetime handling in the inodes *
 *************************************************/

unsigned int datetoint(struct DATE date) {
  unsigned int val = 0;

  val = date.year - 1970;
  val <<= 4;
  val |= date.month;
  val <<= 5;
  val |= date.day;
  val <<= 5;
  val |= date.hour;
  val <<= 6;
  val |= date.min;
  val <<= 6;
  val |= date.sec;

  return val;
}

int inttodate(struct DATE *date, unsigned int val) {
  date->sec = val&0x3F;
  val >>= 6;
  date->min = val & 0x3F;
  val >>= 6;
  date->hour = val & 0x1F;
  val >>= 5;
  date->day = val & 0x1F;
  val >>= 5;
  date->month = val & 0x0F;
  val >>= 4;
  date->year = (val & 0x3F) + 1970;
  return 1;
}

unsigned int currdatetimetoint() {
  struct tm *tm_ptr;
  time_t the_time;
  struct DATE now;

  (void) time(&the_time);
  tm_ptr = gmtime(&the_time);

  now.year = tm_ptr->tm_year - 70;
  now.month = tm_ptr->tm_mon + 1;
  now.day = tm_ptr->tm_mday;
  now.hour = tm_ptr->tm_hour;
  now.min = tm_ptr->tm_min;
  now.sec = tm_ptr->tm_sec;

  return datetoint(now);
}

