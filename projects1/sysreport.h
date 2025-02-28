/**
 * CS 521 Project 1 - System Report
 *
 * @author your_name_here
 */

 #ifndef SYSREPORT_H
 #define SYSREPORT_H
 
 struct loadavg {
     double one;
     double five;
     double fifteen;
 };
 
 struct mem_info {
     unsigned long total;
     unsigned long avail;
 };
 
 struct cpuinfo {
     unsigned long idle_time;
     unsigned long total_time;
 };
 
 /**
  * Retrieves the host name of this machine from procfs.
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  * @param name buffer to store the hostname
  *
  * @return 0 on success, -1 on failure
  */
 int host_name(int proc_fd, char name[64]);
 
 /**
  * Retrieves the kernel version of this machine from procfs.
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  * @param version buffer to store the version string
  *
  * @return 0 on success, -1 on failure
  */
 int kernel_version(int proc_fd, char version[32]);
 
 /**
  * Retrieves the number of CPUs available on this machine.
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  *
  * @return number of CPUs, or -1 on error
  */
 int num_cpus(int proc_fd);
 
 /**
  * Retrieves the amount of main memory (RAM) available.
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  *
  * @return available memory, or 0 on failure
  */
 unsigned long total_memory(int proc_fd);
 
 /**
  * Retrieves the total system uptime (time running since boot) as a human
  * readable string in the format:
  *
  *  X days, HH:MM:SS
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  * @param buf buffer to store the formatted string
  * @buf_sz size of the buffer
  *
  * @return 0 on success, -1 on failure
  */
 int uptime(int proc_fd, char *buf, size_t buf_sz);
 
 /**
  * Retrieves the system load average over the last minute, five minutes, and
  * fifteen minutes.
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  *
  * @return loadavg struct with load average information, or a zeroed out struct
  * on failure.
  */
 struct loadavg load_average(int proc_fd);
 
 /**
  * Retrieves memory usage information.
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  *
  * @return mem_info struct with total and available memory, or a zeroed out
  * struct on failure.
  */
 struct mem_info memory_usage(int proc_fd);
 
 /**
  * Retrieves CPU usage information.
  *
  * @param proc_fd file descriptor of the proc directory (from openat)
  *
  * @return cpuinfo struct with idle time and total time spent by the CPU during
  * the last sampling period.
  */
 struct cpuinfo cpu_usage(int proc_fd);
 
 /**
  * Converts a number of bytes into human-friendly units (such as MiB, GiB, etc)
  * expressed as a string.
  *
  * @param num_bytes the raw amount of bytes to convert
  * @param decimal_places number of decimal places to show after the '.' in the * output
  * @param buf buffer to place the formatted string
  * @param buf_sz size of the output buffer
  */
 void human_readable_sz(
         unsigned long num_bytes, unsigned short decimal_places,
         char *buf, size_t buf_sz);
 
 #endif