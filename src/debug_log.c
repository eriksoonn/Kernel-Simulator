#include "debug_log.h"

void debug_messages(const char* message, Color alert_color) {
    const char* color_code = "";

    switch (alert_color) {
        case GREEN:
            color_code = "\x1b[32m";
            break;
        case YELLOW:
            color_code = "\x1b[33m";
            break;
        case BLUE:
            color_code = "\x1b[34m";
            break;
        default:
            break;
    }

    printf("%s%s%s", color_code, message, "\x1b[0m");
}