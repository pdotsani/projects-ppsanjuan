#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "sysreport.h"
#include "lookup.h"

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

  read(fd, name, 32);

  close(fd);

  return 0;
}

int num_cpus(int proc_fd) {
  int fd = openat(proc_fd, "./cpuinfo", O_RDONLY);
  if (fd == -1) {
    perror("openat");
  }
  
  char *args[] = {
    "",
    "cpu cores",
    ":"
  };
  
  char cpus[100];
  struct keyword key;
  configure_keyword(&key, args);

  lookup(&key, &fd, cpus);

  return atoi(cpus);
}

unsigned long total_memory(int proc_fd) {
  int fd = openat(proc_fd, "./meminfo", O_RDONLY);
  if (fd == -1) {
    perror("openat");
    return -1;
  }

  char *args[] = {
    "",
    "MemAvailable",
    ":"
  };
  char buf[10000];
  struct keyword key;
  configure_keyword(&key, args);

  lookup(&key, &fd, buf);

  close(fd);

  return atoi(buf);
}

int uptime(int proc_fd, char *buf, size_t buf_sz) {
  char time[buf_sz];
  long double starttime, endtime, uptime;
  int days;

  int fd = openat(proc_fd, "./uptime", O_RDONLY);
  if (fd == -1) {
    perror("openat");
    return -1;
  }

  read(fd, time, buf_sz);

  // extract 2 long values from string
  sscanf(time, "%Lf %Lf", &starttime, &endtime);

  uptime = endtime - starttime;

  days = (int)(uptime / 86400);
  int remaining = (int)uptime % 86400;
  int hours = remaining / 3600;             
  remaining %= 3600;
  int minutes = remaining / 60;
  int seconds = remaining % 60;

  char str_days[100];
  char str_hours[100];
  char str_minutes[100];
  char str_seconds[100];
  sprintf(str_days, "%d days,", days);
  sprintf(str_hours, " %d", hours);
  sprintf(str_minutes, ":%d:", minutes);
  sprintf(str_seconds, "%d", seconds);
  strcat(buf, str_days);
  strcat(buf, str_hours);
  strcat(buf, str_minutes);
  strcat(buf, str_seconds);

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

  char kn[32];
  kernel_version(proc_fd, kn);
  
  int mem = total_memory(proc_fd);
  int cpus = num_cpus(proc_fd);

  char time[50];
  uptime(proc_fd, time, 50);

  printf("Welcome to %s\n", hn);
  printf("Kernel is %s\n", kn);
  printf("Memory is %d\n", mem);
  printf("cpus %d\n", cpus);
  printf("uptime %s\n", time);

  return 0;
}
