#include <stddef.h>
#include "fileio.h"

int vdcreat(char* pathname, unsigned short flags) {
  // TODO
  return 0;
}

int vdopen(char* pathname, unsigned short flags) {
  // TODO
  return 0;
}

int vdread(int fd, void *buf, int count) {
  // TODO
  return 0;
}

int vdwrite(int fd, void *buf, int count) {
  // TODO
  return 0;
}

int vdseek(int fd, int offset, int whence) {
  // TODO
  return 0;
}

int vdclose(int fd) {
  // TODO
  return 0;
}

int vdunlink(char *pathname) {
  // TODO
  return 0;
}

VDDIR *vdopendir(char *name) {
  // TODO
  return NULL;
}

int vdclosedir(VDDIR *dirp) {
  // TODO
  return 0;
}

vddirent *vdreaddir(VDDIR *dirp) {
  // TODO
  return NULL;
}
