#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>

#define CYCLES 			0
#define INSTRUCTIONS 	1
#define CACHEREFS		2
#define CACHEMISSES		3

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, 
							int cpu, int group_fd, unsigned long flags){

	int ret; 

	ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
	return ret;

}