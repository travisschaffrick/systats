#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXBUFLEN 256

#define CPU_IDLE 4
#define MEM_TOTAL 0
#define MEM_AVAIL 2
#define MEM_LOC 1
#define ONE_MIN 0
#define FIVE_MIN 1
#define FIFTEEN_MIN 2
#define UPTIME 0

struct cpu_usage get_cpu_usage();

struct
cpu_usage {
  unsigned long long idle_time; // 10ms
  unsigned long long total_time; // 10ms
};

struct
mem_usage {
  float total; // kB
  float avail; // kB
};

struct
load_avg {
  float one_min;
  float five_min;
  float fifteen_min;
};

struct
uptime {
  int seconds;
};


/*
@brief returns a struct containing the current CPU idle time and total time in units of 10ms
*/
struct cpu_usage
get_cpu_usage() {
  struct cpu_usage cpu = {0};

  char line_buffer[MAXBUFLEN];  

  FILE *fp = fopen("/proc/stat", "r");
  
  int i = 0;
  if (fp != NULL) {
    if (fgets(line_buffer, sizeof(line_buffer), fp)) {
      line_buffer[MAXBUFLEN-1] = '\0';
      char *token = strtok(line_buffer, " ");
      while (token != NULL) {
         if (i == CPU_IDLE) {
           cpu.idle_time = strtoll(token, NULL, 10);
         }
         cpu.total_time += strtoll(token, NULL, 10);
        i++;
        if (i > 0)
          token = strtok(NULL, " ");
      }
    }
  }

  fclose(fp);
  return cpu;
}

/*
@brief returns a struct containing the current memory usage (total and available)
*/
struct mem_usage 
get_mem_usage() {
  struct mem_usage mem = {0};

  char line_buffer[MAXBUFLEN];  

  FILE *fp = fopen("/proc/meminfo", "r");
  
  int i = 0;
  if (fp != NULL) {
    while (fgets(line_buffer, sizeof(line_buffer), fp)) {
      line_buffer[MAXBUFLEN-1] = '\0';
      char *token = strtok(line_buffer, " ");
      int j = 0;
      while (token != NULL) {
        if (j == MEM_LOC) {
          if (i == MEM_TOTAL) { 
            mem.total = strtof(token, NULL);
          }
          if (i == MEM_AVAIL)   
            mem.avail = strtof(token, NULL);
        }
        token = strtok(NULL, " ");
        j++;
      }
  
      i++;
    }
  }

  fclose(fp);
  return mem;
}

struct load_avg
get_load_avg() {
  struct load_avg load = {0};

  char line_buffer[MAXBUFLEN];  

  FILE *fp = fopen("/proc/loadavg", "r");
  
  int i = 0;
  if (fp != NULL) {
    if (fgets(line_buffer, sizeof(line_buffer), fp)) {
      line_buffer[MAXBUFLEN-1] = '\0';
      char *token = strtok(line_buffer, " ");
      while (token != NULL) {
         if (i == ONE_MIN) {
            load.one_min = strtof(token, NULL);
         }
         if (i == FIVE_MIN) {
            load.five_min = strtof(token, NULL);
         }
         if (i == FIFTEEN_MIN) {
            load.fifteen_min = strtof(token, NULL);
         }
        i++;
        token = strtok(NULL, " ");
      }
    }
  }

  fclose(fp);
  return load;
}

struct uptime
get_uptime() {
  struct uptime uptime = {0};

  char line_buffer[MAXBUFLEN];  

  FILE *fp = fopen("/proc/uptime", "r");
  
  if (fp != NULL) {
    if (fgets(line_buffer, sizeof(line_buffer), fp)) {
      line_buffer[MAXBUFLEN-1] = '\0';
      char *token = strtok(line_buffer, " ");
      uptime.seconds = strtol(token, NULL, 10);
    }
  }

  fclose(fp);
  return uptime;
}

int 
main() {
  // init cpu
  struct cpu_usage cpu_usage1 = get_cpu_usage();
  sleep(1);
  struct cpu_usage cpu_usage2 = get_cpu_usage();

  while (1) {
    // clear screen

    // print cpu info
    cpu_usage2 = get_cpu_usage();
    float diff_total = cpu_usage2.total_time - cpu_usage1.total_time; 
    float diff_idle = cpu_usage2.idle_time - cpu_usage1.idle_time;

    // update cpu info
    cpu_usage1 = cpu_usage2;
    printf("\e[1;1H\e[2J");
    sleep(1);

    if (diff_total > 0)
      printf("CPU Usage: %.2f%%\n", (float)(diff_total - diff_idle ) / (float)diff_total * 100);



    // get mem info
    struct mem_usage mem = get_mem_usage();
    printf("Memory Usage: %.2fGiB/%.2fGiB (%.2f%%)\n",
        (mem.total - mem.avail) / (1024 * 1024),
        mem.total / (1024 * 1024),    
        (mem.total - mem.avail) / mem.total * 100);

    // get load avg
    struct load_avg load = get_load_avg(); 
    printf("Load Avg: %.2f %.2f %.2f\n", load.one_min, load.five_min, load.fifteen_min);

    // get uptime
    struct uptime up = get_uptime();
    printf("Uptime: %d hours, %d minutes, %d seconds\n", up.seconds/60/60, up.seconds/60 % 60, up.seconds%60);
    fflush(stdout);
  }
  
  return 0;
}


