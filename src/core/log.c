#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIME_BUFFER_SIZE 32
#define CONTENT_BUFFER_SIZE 256
#define LOG_MESSAGE_BUFFER_SIZE 512

struct Logger {
    LogLevel log_level;
} default_logger;

void set_accept_log_level(Logger *logger, LogLevel log_lvl) {
    if (!logger) {
        logger = &default_logger;
    }

    if (log_lvl > LOG_LEVELS_COUNT) {
        log_lvl = DEBUG_LEVEL;
    }

    logger->log_level = log_lvl;
}

char *log_level_to_str(LogLevel log_level) {
    switch (log_level) {
        case DEBUG_LEVEL:
            return " [DEBUG] ";
        case INFO_LEVEL:
            return " [INFO] ";
        case WARN_LEVEL:
            return " [WARN] ";
        case ERROR_LEVEL:
            return " [ERROR] ";
        case FATAL_LEVEL:
            return " [FATAL] ";
        default:
            return " [UNKNOWN] ";
    }
}

void log_message_impl(const Logger *logger, LogLevel log_level, char *formatted_message) {
    if (!logger) {
        logger = &default_logger;
    }

    if (log_level < logger->log_level) {
        return;
    }

    time_t timer;
    struct tm *tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);

    char time_buffer[TIME_BUFFER_SIZE];
    memset(time_buffer, 0, TIME_BUFFER_SIZE);

    strftime(time_buffer, TIME_BUFFER_SIZE, "[%Y-%m-%d %H:%M:%S]", tm_info);

    char *log_lvl_buffer = log_level_to_str(log_level);


    char log_message_buffer[LOG_MESSAGE_BUFFER_SIZE];
    memset(log_message_buffer, 0, LOG_MESSAGE_BUFFER_SIZE);

    strcat(log_message_buffer, time_buffer);
    strcat(log_message_buffer, log_lvl_buffer);
    strcat(log_message_buffer, formatted_message);

    printf("%s\n", log_message_buffer);
}

void log_message_with_logger(const Logger *logger, LogLevel log_level, char *fmt, ...) {
    char content_buffer[CONTENT_BUFFER_SIZE];
    memset(content_buffer, 0, CONTENT_BUFFER_SIZE);
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(content_buffer, CONTENT_BUFFER_SIZE, fmt, argptr);
    va_end(argptr);

    log_message_impl(logger, log_level, content_buffer);
}

void log_message(LogLevel log_level, char *fmt, ...) {
    char content_buffer[CONTENT_BUFFER_SIZE];
    memset(content_buffer, 0, CONTENT_BUFFER_SIZE);
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(content_buffer, CONTENT_BUFFER_SIZE, fmt, argptr);
    va_end(argptr);

    log_message_impl(NULL, log_level, content_buffer);
}
