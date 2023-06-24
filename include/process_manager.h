#ifndef _PROCESS_QUEUE_MANAGER_H
#define _PROCESS_QUEUE_MANAGER_H

#include "types.h"
#include "stdlib.h"
#include "stdio.h"
#include "globals.h"
#include "debug_log.h"

PCB_queue * init_queue(int size);
void push(PCB_queue *processQueue, PCB_stuct* process);
PCB_stuct *pop(PCB_queue *processQueue);
void show_queue(PCB_queue *processQueue);

#endif