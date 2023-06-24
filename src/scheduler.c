#include "scheduler.h"

void *schedule_worker(void *args){
    scheduler_info *arguments = (scheduler_info*)args;

    PCB_queue *process_queue = arguments->pcb_queue;
    virtual_info *virtual = arguments->virtual;
    PCB_stuct *pcb;

    while(1) {
        sem_wait(&_scheduler_semaphore);

        for (int cpu_idx = 0; cpu_idx < virtual->cpu_count; cpu_idx++)
        {
            for (int core_idx = 0; core_idx < virtual->cpu_array[cpu_idx]->core_count; core_idx++)
            {
                for (int thread_idx = 0; thread_idx < virtual->cpu_array[cpu_idx]->core_array[core_idx]->thread_count; thread_idx++)
                {
                    if (virtual->cpu_array[cpu_idx]->core_array[core_idx]->thread_array[thread_idx]->process_array != NULL) { continue; }
                    
                    if (process_queue->last_pcb > -1) {
                        virtual->cpu_array[cpu_idx]->core_array[core_idx]->thread_array[thread_idx]->process_array = pop(process_queue);
                        virtual->cpu_array[cpu_idx]->core_array[core_idx]->thread_array[thread_idx]->process_array->state = 1;
                    }
                }
            }
        }
    }
}