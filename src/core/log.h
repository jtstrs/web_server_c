#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdint.h>

enum LogLevel {
    DEBUG_LEVEL,
    INFO_LEVEL,
    WARN_LEVEL,
    ERROR_LEVEL,
    FATAL_LEVEL,
    LOG_LEVELS_COUNT
};

struct Logger;
typedef struct Logger Logger;

typedef enum LogLevel LogLevel;

void set_accept_log_level(Logger *logger, LogLevel log_level);

void log_message_with_logger(const Logger *logger, LogLevel log_level, char *fmt, ...);
void log_message(LogLevel log_level, char *fmt, ...);

#endif