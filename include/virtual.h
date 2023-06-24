#ifndef _MACHINE_H
#define _MACHINE_H

#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "process.h"

void init_virtual(virtual_info *machine, int number_of_cpus, int number_of_cores, int number_of_threads);
void show_virtual(virtual_info *machine);
void *execute_thread(void *args);
void init_cpu(virtual_info *machine, int cpu_idx, int core_num);
void init_cores(virtual_info *machine, int cpu_idx, int core_idx, int thread_num);
void init_thread(virtual_info *machine, int cpu_idx, int core_idx, int thread_idx);
#endif 