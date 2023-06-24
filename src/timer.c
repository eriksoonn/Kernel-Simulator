#include "timer.h"

/**
 * @brief Generate signal to thread, reduced from clock with given period
 * @param optios structure containing period and semaphore
 * @return (void)
 */
void* initialize_timer(void * args){
    int tick = 0;                               // Period counter
    timer_info *arguments = (timer_info *)args;
    sem_t *semaphore;

    /* Select semaphore type*/
    switch (arguments->semaphore_type) {
        case P_SEM:
            semaphore = &_process_init_semaphore;
            break;
        case S_SEM:
            semaphore = &_scheduler_semaphore;
            break;
        case T_SEM:
            semaphore = arguments->semaphore;
            break;
        default:
            break;
    }

    pthread_mutex_lock(&_clock_mutex);                  // Lock mutex
    while (1){
        tick++;

        if (tick > arguments->period){
            sem_post(semaphore);                        // Send continue signal to other thread
            tick = 0;
        }

        done++;                                         // Increment finished timer counter
        pthread_cond_signal(&condition_2);
        pthread_cond_wait(&condition_1, &_clock_mutex); // Wait for clock signal to continue
    }
}