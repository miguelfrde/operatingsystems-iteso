#include "global.h"

SECBOOT secboot;
INODE inode[64];
int secboot_en_memoria = 0;
int blocksmap_en_memoria = 0;
int nodos_i_en_memoria = 0;
int inicio_area_datos;
int mapa_bits_bloques;
int inicio_nodos_i;
char blocksmap[BLOCKSMAP_SIZE];
OPEN_FILE openfiles[MAX_OPEN_FILES];
int openfiles_inicializada = 0;
VDDIR dirs[2] = {-1, -1};
vddirent current;
