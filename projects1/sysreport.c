#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "sysreport.h"

int host_name(int proc_fd, char name[64])
{
  int fd = openat(proc_fd, "./sys/kernel/hostname", O_RDONLY);
  if (fd == -1) {
    perror("openat");
    return -1;
  }

  read(fd, name, 64);

  close(fd);

  return 0;
}

int kernel_version(int proc_fd, char name[32]) {
  int fd = openat(proc_fd, "./sys/kernel/osrelease", O_RDONLY);
  if (fd == -1) {
    perror("openat");
    return -1;
  }

  read(fd, name, 64);

  close(fd);

  return 0;
}

struct cpuinfo cpu_usage(int proc_fd) {
  int fd = openat(proc_fd, "./cpuinfo", O_RDONLY);
  if (fd == -1) {
    perror("openat");
  }
  struct cpuinfo cpu;

  return cpu;
}

unsigned long total_memory(int proc_fd) {
  int fd = openat(proc_fd, "./meminfo", O_RDONLY);
  if (fd == -1) {
    perror("openat");
    return -1;
  }

  // run find keyword here

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

  char kn[32];
  kernel_version(proc_fd, kn);

  printf("Welcome to %s\n", hn);
  printf("Kernel is %s\n", kn);

  return 0;
}
