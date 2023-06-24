#include "process.h"

PCB_queue* init_queue(int size){
    PCB_queue *process_queue = malloc(sizeof(process_queue));
    process_queue->queue = malloc(size*sizeof(PCB_stuct*));
    process_queue->size = size;
    process_queue->last_pcb = -1;
    return process_queue;
}

PCB_stuct* initialize_pcb_default() {
    PCB_stuct* pcb = malloc(sizeof(PCB_stuct));
    
    if (pcb == NULL) { return NULL; }

    pcb->pid = pid;
    pcb->initial_quantum = 8;
    pcb->quantum = 8;
    pcb->state = 0;
    pcb->counter = rand() % 100;

    return pcb;
}

void *process_generator(void * args){
    process_generation_info *options = (process_generation_info *)args;

    while(1) {
        sem_wait(&_process_init_semaphore); 
        PCB_stuct* pcb = initialize_pcb_default();          
        add(options->pcb_list, options->pcb_queue, pcb);
    }
}

void add(PCB_list *pcbList, PCB_queue *processQueue, PCB_stuct* pcb){
    if (pcbList->size >= processQueue->size){ return; }

    if (pcbList->first_pcb == NULL){
        pcbList->first_pcb=pcb;
        pcbList->last_pcb=pcb;
    
    } else{
        pid++;
        pcb->previous_pcb = pcbList->last_pcb;
        pcbList->last_pcb->next_pcb=pcb;
        pcbList->last_pcb=pcb;
    }

    pcbList->size++;
    push(processQueue, pcb);

}

void delete(PCB_stuct *process, PCB_list *pcbList) {
    if (pcbList->last_pcb->pid == process->pid) {
        pcbList->last_pcb = pcbList->last_pcb->previous_pcb;
    }

    if (pcbList->first_pcb->pid == process->pid) {
        pcbList->first_pcb = pcbList->first_pcb->next_pcb;
    }

    if (process->next_pcb == NULL) {
        process->previous_pcb->next_pcb = process->next_pcb;
    
    } else if(process->previous_pcb == NULL) {
        process->next_pcb->previous_pcb = process->previous_pcb;
    
    } else{
        process->next_pcb->previous_pcb = process->previous_pcb;
        process->previous_pcb->next_pcb = process->next_pcb;
    }
    
    pcbList->size--;
    free(process);
}

void push(PCB_queue *process_queue, PCB_stuct* pcb){
    pthread_mutex_lock(&_queue_mutex);

    if (process_queue->last_pcb+1 < process_queue->size) {
        process_queue->queue[process_queue->last_pcb+1] = pcb;
        process_queue->last_pcb++;
    }

    pthread_mutex_unlock(&_queue_mutex);
}

PCB_stuct *pop(PCB_queue *process_queue){
    pthread_mutex_lock(&_queue_mutex);
    PCB_stuct *pcb = NULL;

    if (process_queue->last_pcb == -1) { 
        pthread_mutex_unlock(&_queue_mutex);
        return pcb; 
    }

    pcb = process_queue->queue[0];
    for (int i=0; i < process_queue->last_pcb; i++) {
        process_queue->queue[i] = process_queue->queue[i+1];
    }

    process_queue->last_pcb--;
    pthread_mutex_unlock(&_queue_mutex);
    return pcb;
}

void show_queue(PCB_queue *process_queue) {
    debug_messages("----------------------------------------------\n", BLUE);
    debug_messages("process queue: PID\n", BLUE);

    char buffer[64];
    for (int i = 0; i <= process_queue->last_pcb; i++){
        sprintf(buffer, "%d\t", process_queue->queue[i]->pid);
        debug_messages(buffer, BLUE);
    }
    printf("\n");
}