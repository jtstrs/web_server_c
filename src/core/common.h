#ifndef COMMON_H
#define COMMON_H

#define LOG

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0);

#define ALWAYS 1
#define BACKLOG_QUEUE 10
#define INCOMMING_BUFFER_LENGTH 2048
#define HOST_NAME_LENGTH 64

#endif