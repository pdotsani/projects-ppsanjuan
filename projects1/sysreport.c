#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "sysreport.h"

int host_name(int proc_fd, char name[64])
{
  int fd = openat(proc_fd, "/etc/hostname", O_RDONLY);
  if (fd == -1) {
    perror("openat");
    return -1;
  }

  read(fd, name, 64);

  close(fd);

  return 0;
}

int main(void)
{
  int proc_fd = open("/proc", O_RDONLY); // open proofs, read only
  if (proc_fd == -1) {
    perror("open");
    return 1;
  }

  char hn[64];
  host_name(proc_fd, hn);

  printf("Welcome to %s\n", hn);

  return 0;
}
