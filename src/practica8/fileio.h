#ifndef _FILE_IO_H_
#define _FILE_IO_H_

typedef struct OPEN_FILE {
  int inuse;
  int inode;
  int currbloqueenmemoria;
  int currpos;
  char* buffer;
} OPEN_FILE;

typedef struct VDDIR {
  // TODO
} VDDIR;

typedef struct vddirent {
  char *d_name;
  // ino_t d_ino
} vddirent;


int vdcreat(char* pathname, unsigned short flags);

int vdopen(char* pathname, unsigned short flags);

int vdread(int fd, void *buf, int count);

int vdwrite(int fd, char *buf, int bytes);

int vdseek(int fd, int offset, int whence);

int vdclose(int fd);

int vdunlink(char *filename);

VDDIR *vdopendir(char *name);

int vdclosedir(VDDIR *dirp);

vddirent *vdreaddir(VDDIR *dirp);

#endif
