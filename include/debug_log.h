#ifndef _UTILS_H
#define _UTILS_H

typedef enum {
    GREEN,
    YELLOW,
    BLUE,
} Color;

#include <stdio.h>

void debug_messages(char const* const text, Color color);

#endif