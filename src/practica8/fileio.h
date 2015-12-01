#ifndef _FILE_IO_H_
#define _FILE_IO_H_

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

int vdwrite(int fd, void *buf, int count);

int vdseek(int fd, int offset, int whence);

int vdclose(int fd);

int vdunlink(char *pathname);

VDDIR *vdopendir(char *name);

int vdclosedir(VDDIR *dirp);

vddirent *vdreaddir(VDDIR *dirp);

#endif
