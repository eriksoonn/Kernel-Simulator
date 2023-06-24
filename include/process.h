#ifndef _pcb_list_h
#define _pcb_list_h

#include "globals.h"
#include "types.h"
#include "process_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

void *process_generator(void * args);
void add(PCB_list *pcb_list, PCB_queue *process_queue, PCB_stuct* pcb);
void print_pcbList(PCB_list *pcbList);
void delete(PCB_stuct *process, PCB_list *pcbList);
#endif