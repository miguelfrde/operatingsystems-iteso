#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "global.h"
#include "fileio.h"
#include "vdlib.h"
#include "vdisk.h"

/**
 * Auxiliary function
 */
unsigned short *postoptr(int fd, int pos) {
	int currinode;
	unsigned short *currptr;
	unsigned short indirect1;

	currinode = openfiles[fd].inode;

	// In the first 16K
  if ((pos / 1024) < 16) {
		// In the first 16 direct pointers
		currptr = &inode[currinode].blocks[pos / 1024];
	} else if ((pos / 1024) < 528) {
		// If the indirect is empty, assign a block to it
    if (inode[currinode].indirect == 0) {
			// The first available block
			indirect1 = nextfreeblock();
			assignblock(indirect1);
			inode[currinode].indirect=indirect1;
		}
		currptr = &openfiles[fd].buffindirect[pos/1024 - 16];
	} else {
		return NULL;
	}

	return currptr;
}

/**
 * Given the current position of the filepointer, return the memory address
 * in the inode or in the block of pointers that contains the block where the
 * file pointer is
 */
unsigned short *currpostoptr(int fd) {
  unsigned short *currptr;
	currptr = postoptr(fd, openfiles[fd].currpos);
  return currptr;
}


/**
 * TODO: go through the inodes table until we find a free one
 */
int nextfreeinode() {
  // TODO
  return 0;
}


// Esta función se va a usar para crear una función en el
// sistema de archivos
int vdcreat(char *filename, unsigned short perms) {
  int numinode;

  // Check if the file exists
  numinode = searchinode(filename);

  // If the file doesn't exist
  if (numinode == -1) {
    // Find a blank inode in the bitmap (inodes)
    numinode = nextfreeinode();

    // There's no space available for more files
    if (numinode == -1) {
      return -1;
    }
  } else {
    removeinode(numinode);
  }

  // Write the file in the found inode
  setninode(numinode, filename, perms, getuid(), getgid());

  if (!openfiles_inicializada) {
    for (int i = 3; i < 16; i++) {
      openfiles[i].inuse = 0;
      openfiles[i].currbloqueenmemoria = -1;
    }
    openfiles_inicializada = 1;
  }

  // Check if there's place in the open file table. If not, return -1
  for (int i = 3; i < MAX_OPEN_FILES; i++) {
    if (!openfiles[i].inuse) {
      openfiles[i].inuse = 1;
      openfiles[i].inode = numinode;
      openfiles[i].currpos = 0;
      return i;
    }
  }
  return -1;
}


int vdopen(char* pathname, unsigned short flags) {
  // TODO
  return 0;
}


int vdread(int fd, void *buf, int count) {
  // TODO
  return 0;
}


int vdwrite(int fd, char *buffer, int bytes) {
  int currblock;
  int currinode;
  int cont=0;
  int sector;
  unsigned short *currptr;

  // If the file is not open, return error
  if (openfiles[fd].inuse == 0) {
    return -1;
  }

  currinode = openfiles[fd].inode;

  while (cont < bytes) {
    // Get the address where the block corresponding to the current position is
    currptr = currpostoptr(fd);
    if (currptr == NULL) {
      return -1;
    }

    currblock = *currptr;

    // If the block is blank, give it one
    if (currblock==0) {
      currblock = nextfreeblock();
      *currptr = currblock; // Put the block found where the pointer to the current block points
      assignblock(currblock);  // Assign it in the bitmap

      // Write the sector in the inode table in the disk
      sector = (currinode / 4) * 4;
      // NOTE (miguel): I'm casting the inode to char here to be able to pass it to this function
      vdwriteseclog(inicio_nodos_i + sector, (char*)(&inode[sector * 8]));
    }

    // If the block in the current position is not in memory, read the block to the file buffer
    if (openfiles[fd].currbloqueenmemoria != currblock) {
      // Read the current block to the buffer that is in the open files table
      readblock(currblock, openfiles[fd].buffer);
      openfiles[fd].currbloqueenmemoria = currblock;
    }

    // Copy the character to the buffer
    openfiles[fd].buffer[openfiles[fd].currpos % 1024] = buffer[cont];

    // Increment position
    openfiles[fd].currpos++;

    // If the position is greater than the size, update the size
    if (openfiles[fd].currpos > inode[currinode].size) {
      inode[openfiles[fd].inode].size=openfiles[fd].currpos;
    }

    cont++;

    // If the buffer is full, write it
    if (openfiles[fd].currpos % 1024 == 0) {
      writeblock(currblock,openfiles[fd].buffer);
    }
  }
  return(cont);
}


int vdseek(int fd, int offset, int whence) {
  unsigned short oldblock, newblock;

  // If the fd is closed, return error
  if (openfiles[fd].inuse == 0) {
    return -1;
  }

  oldblock = *currpostoptr(fd);

  if (whence == 0) {  // From start
    if (offset < 0 || openfiles[fd].currpos + offset > inode[openfiles[fd].inode].size) {
      return -1;
    }
    openfiles[fd].currpos = offset;

  } else if (whence == 1) {  // From the current position
    if (openfiles[fd].currpos + offset > inode[openfiles[fd].inode].size || openfiles[fd].currpos + offset < 0) {
      return -1;
    }
    openfiles[fd].currpos += offset;

  } else if (whence == 2) { // From the end
    if (offset > inode[openfiles[fd].inode].size || openfiles[fd].currpos - offset < 0) {
      return -1;
    }
    openfiles[fd].currpos = inode[openfiles[fd].inode].size - offset;
  } else {
    return -1;
  }

  // Check that the new position of the pointer is a block different to the one we
  // were at, if so, load that block to memory
  newblock = *currpostoptr(fd);

  if (newblock != oldblock) {
    writeblock(oldblock, openfiles[fd].buffer); // Write the old block
    readblock(newblock,openfiles[fd].buffer);   // Read the new block
    openfiles[fd].currbloqueenmemoria = newblock; // Mark the new block as the current one in the openfile table
  }

  return openfiles[fd].currpos ;
}

int vdclose(int fd) {
  // TODO
  return 0;
}

int vdunlink(char *filename) {
  int numinode = searchinode(filename);
  if (numinode == -1) {  // The file doesn't exist
    return -1;
  }
  removeinode(numinode);
  return 0;
}

VDDIR *vdopendir(char *path) {
	int i = 0;

	if (!secboot_en_memoria) {
    // Note: changed the given 0,0,0,1,1 for 0,0,0,2,1
		vdreadsector(0, 0, 0, 2, 1, (unsigned char *) &secboot);
		secboot_en_memoria = 1;
	}

	inicio_nodos_i = secboot.sec_res+ secboot.sec_mapa_bits_bloques;

	if (!nodos_i_en_memoria) {
		for (int i = 0; i < secboot.sec_tabla_nodos_i; i++) {
			// Note: changed the given i*8 for i*4
			vdreadseclog(inicio_nodos_i + i, (char *)&inode[i*4]);
		}

		nodos_i_en_memoria = 1;
	}

	if (strcmp(path, ".") != 0) {
		return NULL;
  }

	i = 0;
	while (dirs[i] != -1 && i < 2) {
		i++;
  }

	if (i == 2) {
		return NULL;
  }

	dirs[i] = 0;

	return &dirs[i];
}

int vdclosedir(VDDIR *dirdesc) {
	(*dirdesc) = -1;
  return 0;
}

vddirent *vdreaddir(VDDIR *dirdesc) {
	if (!nodos_i_en_memoria) {
		for (int i = 0; i < secboot.sec_tabla_nodos_i; i++) {
			// Note: changed i*8 for i*4
			vdreadseclog(inicio_nodos_i + i, (char*)&inode[i * 4]);
		}

		nodos_i_en_memoria = 1;
	}

	// Mientras no haya nodo i, avanza
  // Increase while there's no inode
	while (isinodefree(*dirdesc) && *dirdesc < 60) {
		(*dirdesc)++;
  }


	// Point to where the inode name is
  current.d_name = inode[*dirdesc].name;

	(*dirdesc)++;

	if (*dirdesc >= 60) {
		return NULL;
  }

	return &current;
}
