#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd, ok;
  char buffer[16 * 4096];

  fd = creat("swap", 0640);
  ok = write(fd, buffer, 16 * 4096);
  if (!ok) {
    perror("Errors found while writting to file");
  }
  close(fd);
  return 0;
}
