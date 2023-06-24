#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <semaphore.h>

typedef struct virtual_info{
    int cpu_count;
    struct cpu_info** cpu_array;
} virtual_info;

typedef struct cpu_info {
    int id;
    struct core_info** core_array;
    int core_count;
} cpu_info;

typedef struct core_info {
    int id;
    struct thread_info** thread_array;
    int thread_count;
} core_info;

typedef struct thread_info{
    int id;
    struct PCB_stuct* process_array;
} thread_info;

typedef struct PCB_stuct{
    struct PCB_stuct *previous_pcb;
    struct PCB_stuct *next_pcb;

    int pid;
    int state;
    int quantum;
    int initial_quantum;
    int counter;
} PCB_stuct;

typedef struct PCB_queue{
    int size;
    int last_pcb;
    PCB_stuct** queue;
} PCB_queue;

typedef struct PCB_list{
    int size;
    struct PCB_stuct *first_pcb;
    struct PCB_stuct *last_pcb;

} PCB_list;

typedef struct thread_running_info{
    int id;
    int core_id;
    int cpu_id;

    sem_t *semaphore;

    struct virtual_info *virtual;
    struct PCB_list *pcb_list;
    struct PCB_queue *pcb_queue;

} thread_running_info;

typedef enum {
    P_SEM,
    S_SEM,
    T_SEM,
} timer_type;

typedef struct timer_info{
    int period;
    timer_type semaphore_type;
    sem_t *semaphore;
} timer_info;

typedef struct scheduler_info{
    struct virtual_info *virtual;
    struct PCB_queue *pcb_queue;
} scheduler_info;

typedef struct process_generation_info{
    struct PCB_list *pcb_list;
    struct PCB_queue *pcb_queue;
} process_generation_info;

typedef struct clock_info{
    int timer_count;
} clock_info;

#endif