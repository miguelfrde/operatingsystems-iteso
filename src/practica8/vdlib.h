#ifndef _VD_LIB_H_
#define _VD_LIB_H_

#define MAX_NUM_OF_FILES_IN_ROOT 60
#define BLOCKSMAP_SIZE 512

typedef struct PARTITION {
  char drive_status;
  char chs_begin[3];
  char partition_type;
  char chs_end[3];
  int lba;
  int secs_partition;
} PARTITION;

// Lo que vamos a escribir en el primer sector del disco
// Debe medir 512 bytes
typedef struct MBR {
  char bootstrap_code[446];
  struct PARTITION partition[4];
  short boot_signature;
} MBR;
// printf("%d\n",sizeof(struct MBR));


// Debe medir 512 bytes
typedef struct SECBOOT {
  char jump[4];
  char nombre_particion[8];
  unsigned char sec_res;    // 1 sector reservado
  unsigned char sec_mapa_bits_bloques;  // 16 sectores
  unsigned short sec_tabla_nodos_i; //  15 sectores
  unsigned int sec_log_particion;   // 130559 sectores
  unsigned char sec_x_bloque;     // 2 sectores por bloque
  unsigned char heads;        // 48 superficies
  unsigned char cyls;         // 160 cilindros
  unsigned char secfis;       // 17 sectores por track
  char restante[486];
} SECBOOT;
// printf("%d\n",sizeof(struct SECBOOT));


// Debe medir 128 bytes
typedef struct INODE {
  char name[69];
  char status;
  unsigned short uid;   // 16 bits
  unsigned short gid;   // 16 bits
  unsigned short perms; // 16 bits
  unsigned int datetimecreat; // 32 bits
  unsigned int datetimemodif; // 32 bits
  unsigned int datetimelaacc; // 32 bits
  unsigned int size;      // 32 bits
  unsigned short blocks[16];  // 16 x 32 bits = 2 x 16 bytes = 32 bytes
  unsigned short indirect;  // 16 bits
  unsigned short indirect2; // 16 bits
} INODE;

typedef struct DATE {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
} DATE;

int vdwriteseclog(int sec_log, char *buffer);
int vdreadseclog(int sec_log, char *buffer);

int isblockfree(int block);
int nextfreeblock();
int assignblock(int block);
int unassignblock(int block);
int writeblock(int block, char *buffer);
int readblock(int block, char *buffer);

unsigned int datetoint(DATE date);
int inttodate(DATE *date, unsigned int val);
unsigned int currdatetimetoint();

int setninode(int num, char *filename, unsigned short atribs, int uid, int gid);
int searchinode(char *filename);
int removeinode(int numinode);

#endif
