#ifndef OPT_PARSER
#define OPT_PARSER

#include <stdint.h>

struct Options {
    uint32_t port;
};

struct Options *parse_opts(int32_t argc, char *argv[]);

#endif