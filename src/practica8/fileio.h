#include "global.h"

#ifndef _FILE_IO_H_
#define _FILE_IO_H_


int vdcreat(char* filename, unsigned short flags);

int vdopen(char* filename, unsigned short flags);

int vdread(int fd, char *buf, int count);

int vdwrite(int fd, char *buf, int bytes);

int vdseek(int fd, int offset, int whence);

int vdclose(int fd);

int vdunlink(char *filename);

VDDIR *vdopendir(char *path);

int vdclosedir(VDDIR *dirdesc);

vddirent *vdreaddir(VDDIR *dirdesc);

#endif
