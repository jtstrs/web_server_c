#include "opt_parser.h"
#include "common.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void print_help()
{

}

int32_t parse_int32(char * token) 
{
    if (!token) {
        return -1;
    }

    char * endptr;

    int32_t value = (int32_t) strtoul(token, &endptr, 10);

    if (endptr == token)
    {
        return -1;   
    }

    if (value < 0 || value >= UINT32_MAX) {
        return -1;
    }

    return value;
}

union OptValue
{
    int32_t iv;
};

enum OptField {
    Port,
    Unknown
};

enum ParseStage 
{
    WaitForDecl,
    WaitForValue,
    Interrupt
};

enum OptField parse_opt_field(char * token)
{
    if (!strcmp("-p", token) || !strcmp("--port", token)) {
        return Port;
    } else {
        return Unknown;
    }
}

union OptValue parse_opt_value(char * token, enum OptField field, bool* ok)
{
    union OptValue val;

    switch (field)
    {
    case Port:
        int32_t port_val = parse_int32(token);
        if (port_val == -1) {
            *ok = false;
        } else {
            val.iv = port_val;
        }
        break;
    default:
        // Never should happen
        exit(EXIT_FAILURE);
        break;
    }
    return val;
}

void assign_option(struct Options * opts, enum OptField opt, union OptValue val)
{
    switch (opt) {
        case Port:
            opts->port = val.iv;
        break;
        case Unknown:
        default:
            // Never should happen 
            exit(EXIT_FAILURE);
    }
}

char * opt_field_to_str(enum OptField opt)
{
    switch (opt) {
        case Port:
            return "Port";
        // [Fallthrough]
        case Unknown:
        default:
            // Never should happen
            exit(EXIT_FAILURE);
    };
}

struct Options * parse_opts(int32_t argc, char *argv[]) 
{
    if (argc <= 1) {
        printf("Too small arguments count. Write --help to see available options.\n");
        exit(0);
    }

    char ** opt_token = &argv[1];

    static struct Options opts;
    opts.port = 0;

    enum ParseStage stage = WaitForDecl;
    enum OptField opt = Unknown;

    union OptValue val;
    val.iv = 0;
    
    bool val_parse_suc = true;

    while (*opt_token != NULL) 
    {
        switch (stage) {
            case WaitForDecl:
                opt = parse_opt_field(*opt_token);
                stage = WaitForValue;
            break;
            case WaitForValue:
                val = parse_opt_value(*opt_token, opt, &val_parse_suc);
                if (!val_parse_suc) {
                    stage = Interrupt;
                    break;
                }
                assign_option(&opts, opt, val);
                stage = WaitForDecl;
            break;
            case Interrupt:
            break;
            default:
            // Never should happen
            exit(EXIT_FAILURE);
            break;
        }

        *opt_token++;
    }

    if (stage == Interrupt || stage == WaitForValue || !val_parse_suc) {
        printf("Value for %s option wasnt provied. Abort.", opt_field_to_str(opt));
        exit(EXIT_FAILURE);
    }

    return &opts;
}