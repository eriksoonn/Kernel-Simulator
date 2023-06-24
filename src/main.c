#include "clock.h"
#include "timer.h"
#include "process.h"
#include "scheduler.h"
#include "process_manager.h"
#include "virtual.h"

#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int pid = 0, done = 0;
pthread_cond_t condition_1, condition_2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t _clock_mutex, process_init_mutex, _scheduler_mutex, _queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t _process_init_semaphore, _scheduler_semaphore, _thread_semaphore;

int init_threads(virtual_info *virtual, int cpus, int cores, int threads, int thread_period, int timers, PCB_list *pcb_list, PCB_queue *process_queue) {
    pthread_t thread_id_array[cpus * cores * threads];
    pthread_t timer_id_array[cpus * cores * threads];

    for (int cpu_idx = 0; cpu_idx < virtual->cpu_count; cpu_idx++)
    {
        for (int core_idx = 0; core_idx < virtual->cpu_array[cpu_idx]->core_count; core_idx++) 
        {
            for (int thread_idx = 0; thread_idx < virtual->cpu_array[cpu_idx]->core_array[core_idx]->thread_count; thread_idx++)
            {
                sem_t* semaphore                        = malloc(sizeof(sem_t));
                thread_running_info* thread_options     = malloc(sizeof(thread_running_info));
                timer_info* timer_options               = malloc(sizeof(timer_info));

                thread_options->virtual     = virtual;
                thread_options->cpu_id      = cpu_idx;
                thread_options->core_id     = core_idx;
                thread_options->id          = thread_idx;
                thread_options->pcb_list    = pcb_list;
                thread_options->semaphore   = semaphore;
                thread_options->pcb_queue   = process_queue;

                sem_init(semaphore, 0, 0);

                timer_options->semaphore_type   = T_SEM;
                timer_options->period           = thread_period;
                timer_options->semaphore        = semaphore;

                pthread_create(&(thread_id_array[cpu_idx * core_idx + thread_idx]), NULL, &execute_thread, thread_options);
                pthread_create(&(timer_id_array[cpu_idx * core_idx + thread_idx]), NULL, &initialize_timer, timer_options);

                timers++;
            }
        }
    }
}

int main(int argc, char **argv){
    unsigned int cpus = 1, cores = 1, threads = 2, timers = 2, thread_period = 1000;

    timer_info *process_generation_timer    = malloc(sizeof(timer_info));
    timer_info *scheduler_timer             = malloc(sizeof(timer_info));
    clock_info *clock                       = malloc(sizeof(clock_info));
    scheduler_info *scheduler               = malloc(sizeof(scheduler_info));
    process_generation_info *process_init   = malloc(sizeof(process_generation_info));
    virtual_info *virtual                   = malloc(sizeof(virtual));

    PCB_list *pcb_list                      = malloc(sizeof(pcb_list));
    PCB_queue *process_queue                = init_queue(10);

    process_init->pcb_list = pcb_list;
    process_generation_timer->period = 1000;
    process_generation_timer->semaphore_type = P_SEM;
    scheduler_timer->period = 100;
    scheduler_timer->semaphore_type = S_SEM;

    int options;
    while ((options = getopt(argc, argv, ":hp:s:q:c:r:a:d:")) != -1) {
        switch (options){
            case 'p':
                process_generation_timer->period=atoi(optarg);
                break;
            case 's':
                scheduler_timer->period=atoi(optarg);
                break;
            case 'q':
                free(process_queue);
                process_queue = init_queue(atoi(optarg));
                break;
            case 'c':
                cpus = atoi(optarg);
                break;
            case 'r':
                cores = atoi(optarg);
                break;
            case 'a':
                threads = atoi(optarg);
                break;
            case 'd':
                thread_period = atoi(optarg);
                break;
        }
    }

    printf("---- Program options -----\n");
    printf("CPUs:    %d \n", cpus);
    printf("cores:   %d \n", cores);
    printf("threads: %d \n", threads);
    printf("Timer --> scheduler:   %d \n", scheduler_timer->period);
    printf("Timer --> process gen: %d \n", process_generation_timer->period);
    printf("\n");

    srand(time(NULL));
    sem_init(&_process_init_semaphore, 0 ,0);
    sem_init(&_scheduler_semaphore, 0, 0);

    init_virtual(virtual, cpus, cores, threads);
    show_virtual(virtual);

    scheduler->pcb_queue = process_queue;
    scheduler->virtual = virtual;

    sleep (1);
    pthread_t thread_id_array[cpus * cores * threads];
    pthread_t timer_id_array[cpus * cores * threads];

    timers = init_threads(virtual, cpus, cores, threads, thread_period, timers, pcb_list, process_queue);

    process_init->pcb_list=pcb_list;
    process_init->pcb_queue=process_queue;
    clock->timer_count=timers;

    pthread_t timer_process_generator_th, timer_scheduler_th, clock_th, process_generator_th, scheduler_th;
    
    pthread_create(&process_generator_th, NULL, &process_generator, process_init);
    pthread_create(&scheduler_th, NULL, &schedule_worker, scheduler);
    pthread_create(&timer_process_generator_th, NULL, &initialize_timer, process_generation_timer);
    pthread_create(&timer_scheduler_th, NULL, &initialize_timer, scheduler_timer);
    pthread_create(&clock_th, NULL, &initialize_clock, clock);

    pthread_join(clock_th, NULL);

    return 0;
}