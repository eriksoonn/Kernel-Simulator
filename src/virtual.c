#include "virtual.h"

void init_virtual(virtual_info *machine, int cpu_num, int core_num, int thread_num){
    machine->cpu_count = cpu_num;
    machine->cpu_array = malloc(cpu_num*sizeof(cpu_info*));

    for(int cpu_idx = 0; cpu_idx < cpu_num; cpu_idx++){
        init_cpu(machine, cpu_idx, core_num);

        for(int core_idx = 0; core_idx < core_num; core_idx++){
            init_cores(machine, cpu_idx, core_idx, thread_num);

            for (int thread_idx = 0; thread_idx < thread_num; thread_idx++){
                init_thread(machine, cpu_idx, core_idx, thread_idx);
            }
        }
    }
}

void init_cpu(virtual_info *machine, int cpu_idx, int core_num) {
    machine->cpu_array[cpu_idx] = malloc(sizeof(cpu_info));

    machine->cpu_array[cpu_idx]->core_array = malloc(core_num * sizeof(core_info*));
    machine->cpu_array[cpu_idx]->id = cpu_idx;
    machine->cpu_array[cpu_idx]->core_count = core_num;
}

void init_cores(virtual_info *machine, int cpu_idx, int core_idx, int thread_num) {
    machine->cpu_array[cpu_idx]->core_array[core_idx] = malloc(sizeof(core_info));

    machine->cpu_array[cpu_idx]->core_array[core_idx]->thread_array = malloc(thread_num * sizeof(thread_info*));
    machine->cpu_array[cpu_idx]->core_array[core_idx]->id = core_idx;
    machine->cpu_array[cpu_idx]->core_array[core_idx]->thread_count = thread_num;

}

void init_thread(virtual_info *machine, int cpu_idx, int core_idx, int thread_idx) {
    machine->cpu_array[cpu_idx]->core_array[core_idx]->thread_array[thread_idx] = malloc(sizeof(thread_info));
    machine->cpu_array[cpu_idx]->core_array[core_idx]->thread_array[thread_idx]->id = thread_idx;
}

void* execute_thread(void *args) {
    thread_running_info *arguments = (thread_running_info*) args;
    virtual_info *virtual = arguments->virtual;

    char buffer[256];

    unsigned int thread_id = arguments->id;
    unsigned int core_id = arguments->core_id;
    unsigned int cpu_id = arguments->cpu_id;

    while(1) {
        sem_wait(arguments->semaphore);
        thread_info* process = virtual->cpu_array[cpu_id]->core_array[core_id]->thread_array[thread_id];

        if (process->process_array == NULL) { continue; }

        sprintf(buffer, "Process: %d\n--> CPU id: %d, core id: %d, thread id: %d\n--> quantum: %d counter:%d\n", 
            process->process_array->pid, 
            virtual->cpu_array[cpu_id]->id, virtual->cpu_array[cpu_id]->core_array[core_id]->id, 
            process->id, 
            process->process_array->quantum, 
            process->process_array->counter);
            
        debug_messages(buffer, GREEN);
        process->process_array->counter -= 10;
        process->process_array->quantum -= 10;

        if (process->process_array->counter <= 0){
            sprintf(buffer, "END of process with PID : %d\n", process->process_array->pid);
            debug_messages(buffer, YELLOW);
            delete(process->process_array, arguments->pcb_list);
            
            process->process_array = NULL;
            continue;
        }

        if (process->process_array->quantum <= 0){
            process->process_array->initial_quantum *= 2;
            process->process_array->quantum=process->process_array->initial_quantum;
            process->process_array->state = 0;
            
            push(arguments->pcb_queue, process->process_array);
            process->process_array = NULL;
            show_queue(arguments->pcb_queue);
        }
    }
}

void show_virtual(virtual_info *machine) {
    printf("---- Virtual System Info ----\n");

    for (int cpu_idx = 0; cpu_idx < machine->cpu_count; cpu_idx++) {
        printf("|--> CPU ID: %d\n", machine->cpu_array[cpu_idx]->id);
        machine->cpu_array[cpu_idx]->id = cpu_idx;
        
        for (int core_idx = 0; core_idx < machine->cpu_array[cpu_idx]->core_count; core_idx++){
            printf("     |--> Core ID: %d\n",machine->cpu_array[cpu_idx]->core_array[core_idx]->id);

            for (int thread_idx = 0; thread_idx < machine->cpu_array[cpu_idx]->core_array[core_idx]->thread_count; thread_idx++){
                printf("          |--> Thread ID: %d\n", machine->cpu_array[cpu_idx]->core_array[core_idx]->thread_array[thread_idx]->id);
            }
        }
    }
    printf("-----------------------------\n");
}
