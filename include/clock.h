#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"
#include "types.h"
#include <pthread.h>

void *initialize_clock(void *args);

#endif