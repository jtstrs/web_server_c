#ifndef OPT_PARSER
#define OPT_PARSER

#include <stdint.h>

typedef struct Options {
    uint32_t port;
    uint32_t logging_level;
} Options;

Options *parse_opts(int32_t argc, char *argv[]);

#endif