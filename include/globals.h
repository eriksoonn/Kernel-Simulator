#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "types.h"
#include <pthread.h>
#include <semaphore.h>

extern pthread_mutex_t _clock_mutex, process_init_mutex, _scheduler_mutex, _queue_mutex;
extern sem_t _process_init_semaphore, _scheduler_semaphore, _thread_semaphore;
extern pthread_cond_t condition_1, condition_2;
extern int pid, done;

#endif