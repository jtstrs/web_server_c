#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdint.h>

#define DEBUG_LVL 0
#define INFO_LVL 1
#define WARN_LVL 2
#define ERROR_LVL 3
#define FATAL_LVL 4

void log_message(int32_t log_level, char *fmt, ...);

#endif