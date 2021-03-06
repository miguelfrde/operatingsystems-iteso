#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fileio.h"
#include "vdisk.h"
#include "vdlib.h"

#define MAXLEN 80
#define BUFFERSIZE 512


void locateend(char *cmd);
int executecmd(char *cmd);
int isinvd(char *arg);
int copyuu(char *arg1, char *arg2);
int copyuv(char *arg1, char *arg2);
int copyvu(char *arg1,char *arg2);
int copyvv(char *arg1,char *arg2);
int catu(char *arg1);
int catv(char *arg1);
int diru(char *arg1);
int dirv();
int delu(char *file);
int delv(char *file);

int main() {
  char linea[MAXLEN];
  int result = 1;

  while (result) {
    printf("vshell > ");
    fflush(stdout);
    read(0, linea, 80);
    locateend(linea);
    result = executecmd(linea);
  }
}


/**
 * Find the end of the string/line to put \0
 */
void locateend(char *linea) {
  int i = 0;
  while (i < MAXLEN && linea[i] != '\n') {
    i++;
  }
  linea[i] = '\0';
}


int executecmd(char *linea) {
  char *cmd;
  char *arg1;
  char *arg2;
    
  /*
   *vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
   *secboot_en_memoria = 1;
   *for (int i = 0; i < secboot.sec_mapa_bits_bloques; i++) {
   *  vdreadseclog(mapa_bits_bloques + i, blocksmap + i*512);
   *}
   *blocksmap_en_memoria = 1;
   */

  // Splits the command and the two possible arguments
  cmd = strtok(linea, " ");
  arg1 = strtok(NULL, " ");
  arg2 = strtok(NULL, " ");

  if (cmd == NULL) {
    return 1;
  }

  // exit command
  if (strcmp(cmd, "exit") == 0) {
    return 0;
  }

  // copy command
  if (strcmp(cmd,"copy") == 0) {
    if (arg1 == NULL && arg2 == NULL) {
      fprintf(stderr,"Error en los argumentos\n");
      return(1);
    }

    if (!isinvd(arg1) && !isinvd(arg2)) {
      copyuu(&arg1[2], &arg2[2]);
    } else if (!isinvd(arg1) && isinvd(arg2)) {
      copyuv(&arg1[2],arg2);
    } else if (isinvd(arg1) && !isinvd(arg2)) {
      copyvu(arg1,&arg2[2]);
    } else if(isinvd(arg1) && isinvd(arg2)) {
      copyvv(arg1,arg2);
    }
  }

  // cat command
  if (strcmp(cmd, "cat") == 0) {
    if (isinvd(arg1)) {
      catv(arg1);
    } else {
      catu(&arg1[2]);
    }
  }

  // dir command
  if (strcmp(cmd, "dir") == 0) {
    if (arg1 == NULL) {
      dirv();
    } else if(!isinvd(arg1)) {
      diru(&arg1[2]);
    }
  }

  // delete command
  if (strcmp(cmd, "delete") == 0){
    if (isinvd(arg1)){
      if(delv(arg1)){
        printf("Error trying to delete the file");
      }
    } else {
      if(delu(arg1)){
        printf("Error trying to delete the file"); 
      }
    }
  }

  return 1;
}


/**
 * True if the name of the file doesn't begin with // and hence is not in the
 * virtual disk
 */
int isinvd(char *arg) {
  if (strncmp(arg, "//", 2) != 0) {
    return 1;
  }
  return 0;
}


/**
 * Copies a file in the UNIX filesystem to a destiny file in the same UNIX
 * filesystem
 */
int copyuu(char *arg1, char *arg2) {
  int sfile, dfile;
  char buffer[BUFFERSIZE];
  int ncars;

  if ((sfile = open(arg1, 0)) == -1) {
    fprintf(stderr, "Error: archivo no existe\n");
    return -1;
  }

  dfile = creat(arg2, 0640);

  do {
    ncars = read(sfile, buffer, BUFFERSIZE);
    write(dfile,buffer,ncars);
  } while(ncars==BUFFERSIZE);

  close(sfile);
  close(dfile);

  return 1;
}


/**
 * Copies a file in the UNIX filesystem to a file in the virtual disk
 */
int copyuv(char *arg1, char *arg2) {
  int sfile, dfile;
  char buffer[BUFFERSIZE];
  int ncars;

  if ((sfile = open(arg1, 0)) == -1) {
    fprintf(stderr, "Error: archivo no existe\n");
    return -1;
  }

  dfile = vdcreat(arg2, 0640);

  do {
    ncars = read(sfile, buffer, BUFFERSIZE);
    vdwrite(dfile, buffer, ncars);
  } while (ncars == BUFFERSIZE);

  close(sfile);
  vdclose(dfile);

  return 1;
}


/**
 * Copies a file in the virtual disk to a file in the UNIX
 * filesystem
 */
int copyvu(char *arg1,char *arg2) {
  int sfile, dfile;
  char buffer[BUFFERSIZE];
  int ncars;

  if((sfile = vdopen(arg1, 0)) == -1) {
    fprintf(stderr, "Error: archivo no existe\n");
    return -1;
  }
  dfile = creat(arg2, 0640);

  do {
    ncars = vdread(sfile, buffer, BUFFERSIZE);
    write(dfile, buffer, ncars);
  } while (ncars == BUFFERSIZE);

  vdclose(sfile);
  close(dfile);

  return 1;
}


/**
 * Copies the file in the virtual disk to some file in the same
 * virtual disk
 */
int copyvv(char *arg1,char *arg2) {
  int sfile, dfile;
  char buffer[BUFFERSIZE];
  int ncars;

  if ((sfile = vdopen(arg1, 0)) == -1) {
    fprintf(stderr, "Error: archivo no existe\n");
    return -1;
  }

  dfile = vdcreat(arg2, 0640);

  do {
    ncars = vdread(sfile, buffer, BUFFERSIZE);
    vdwrite(dfile, buffer, ncars);
  } while (ncars == BUFFERSIZE);

  vdclose(sfile);
  vdclose(dfile);

  return 1;
}


/**
 * Shows a file from the virtual disk
 */
int catv(char *arg1) {
  int sfile;
  char buffer[BUFFERSIZE];
  int ncars;

  sfile = vdopen(arg1, 0);

  do {
    ncars = vdread(sfile, buffer, BUFFERSIZE);
    write(1, buffer, ncars);
  } while (ncars == BUFFERSIZE);

  vdclose(sfile);

  return 1;
}


/**
 * Shows a file in the UNIX filesystem
 */
int catu(char *arg1) {
  int sfile;
  char buffer[BUFFERSIZE];
  int ncars;

  sfile = open(arg1, 0);

  do {
    ncars = read(sfile, buffer, BUFFERSIZE);
    write(1, buffer, ncars);
  } while (ncars == BUFFERSIZE);

  close(sfile);

  return 1;
}


/**
 * Shows a directory in the UNIX filesystem
 */
int diru(char *arg1) {
  DIR *dd;
  struct dirent *entry;

  if (arg1[0] == '\0') {
    strcpy(arg1, ".");
  }

  printf("Directorio %s\n",arg1);

  dd = opendir(arg1);
  if (dd == NULL) {
    fprintf(stderr, "Error al abrir directorio\n");
    return -1;
  }

  while ((entry = readdir(dd)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dd);
  return 1;
}

/**
 * Shows a directory in the virtual disk filesystem
 */
int dirv() {
  VDDIR *dd;
  vddirent *entry;

  printf("Directorio del disco virtual\n");

  dd = vdopendir(".");
  if (dd == NULL) {
    fprintf(stderr, "Error al abrir directorio\n");
    return -1;
  }

  while ((entry = vdreaddir(dd)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  vdclosedir(dd);

  return 1;
}

/*
* Deletes a file from the unix system
*/
int delu(char *file){
  //Ignores the double slash
  file += 2;
  if (unlink(file) == 0){
    printf("file %s removed from unix system", file);
    return 0;
  }

  return -1;
}

/*
* Deletes a file from the vd
*/
int delv(char *file){
  return vdunlink(file);
}
