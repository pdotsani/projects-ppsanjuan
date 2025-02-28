#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "sysreport.h"

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