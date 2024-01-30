#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIME_BUFFER_SIZE 32
#define CONTENT_BUFFER_SIZE 256
#define LOG_MESSAGE_BUFFER_SIZE 512

char *log_lvl_as_str(int32_t log_level) {
    switch (log_level) {
        case DEBUG_LVL:
            return " [DEBUG] ";
        case INFO_LVL:
            return " [INFO] ";
        case WARN_LVL:
            return " [WARN] ";
        case ERROR_LVL:
            return " [ERROR] ";
        case FATAL_LVL:
            return " [FATAL] ";
        default:
            return " [UNKNOWN] ";
    }
}

void log_message(int32_t log_level, char *fmt, ...) {
    time_t timer;
    struct tm *tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);

    char time_buffer[TIME_BUFFER_SIZE];
    memset(time_buffer, 0, TIME_BUFFER_SIZE);

    strftime(time_buffer, TIME_BUFFER_SIZE, "[%Y-%m-%d %H:%M:%S]", tm_info);

    char *log_lvl_buffer = log_lvl_as_str(log_level);

    char content_buffer[CONTENT_BUFFER_SIZE];
    memset(content_buffer, 0, CONTENT_BUFFER_SIZE);

    va_list argptr;
    va_start(argptr, fmt);

    vsnprintf(content_buffer, CONTENT_BUFFER_SIZE, fmt, argptr);

    va_end(argptr);

    char log_message_buffer[LOG_MESSAGE_BUFFER_SIZE];
    memset(log_message_buffer, 0, LOG_MESSAGE_BUFFER_SIZE);

    strcat(log_message_buffer, time_buffer);
    strcat(log_message_buffer, log_lvl_buffer);
    strcat(log_message_buffer, content_buffer);

    printf("%s\n", log_message_buffer);
}
