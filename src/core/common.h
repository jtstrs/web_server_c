#ifndef COMMON_H
#define COMMON_H

#define DEBUG

#define handle_error(msg)   \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0);

#define INVALID_DESCRIPTOR -1
#define ERROR_STATUS -1
#define ALWAYS 1
#define BACKLOG_QUEUE 10
#define INCOMMING_BUFFER_LENGTH 2048
#define HOST_NAME_LENGTH 64

#endif