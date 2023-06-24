#include "clock.h"

/**
 * @brief Generate clock signals and pass control to timers (one by one)
 * @return (void)
 */

void* initialize_clock(void* args){
    clock_info* timer_info = (clock_info *)args;

    while (1) {
        pthread_mutex_lock(&_clock_mutex);                      // Lock mutex

        while (done < timer_info->timer_count){
            pthread_cond_wait(&condition_2, &_clock_mutex);     // Wait for timer to retrice control
        }

        done = 0;                                               // Reset finished timer count
        pthread_cond_broadcast(&condition_1);                   // Broadcast signal to all timers
        pthread_mutex_unlock(&_clock_mutex);                    // Unlock mutex
    }
}