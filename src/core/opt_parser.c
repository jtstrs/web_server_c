#include "opt_parser.h"
#include "common.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


union OptValue {
    int32_t iv;
};

enum OptField {
    Port,
    LogggingLevel,
    Unknown
};

enum ParseStage {
    WaitForDecl,
    WaitForValue,
    Interrupt
};

typedef union OptValue OptValue;
typedef enum OptField OptField;
typedef enum ParseStage ParseStage;

int32_t parse_int32(char *token) {
    if (!token) {
        return -1;
    }

    char *endptr;

    int32_t value = (int32_t) strtoul(token, &endptr, 10);

    if (endptr == token) {
        return -1;
    }

    if (value < 0 || value >= UINT32_MAX) {
        return -1;
    }

    return value;
}

OptField parse_opt_field(char *token) {
    if (!strcmp("-p", token) || !strcmp("--port", token)) {
        return Port;
    }
    if (!strcmp("-l", token) || !strcmp("--logging_level", token)) {
        return LogggingLevel;
    } else {
        return Unknown;
    }
}

OptValue parse_opt_value(char *token, OptField field, bool *ok) {
    OptValue val;

    switch (field) {
        case LogggingLevel:
        case Port:
            int32_t int_val = parse_int32(token);
            if (int_val == -1) {
                *ok = false;
            } else {
                val.iv = int_val;
            }
            break;
        case Unknown:
            break;
        default:
            // Never should happen
            exit(EXIT_FAILURE);
            break;
    }
    return val;
}

void assign_option(Options *opts, OptField opt, OptValue val) {
    switch (opt) {
        case Port:
            opts->port = val.iv;
            break;
        case LogggingLevel:
            opts->logging_level = val.iv;
            break;
        case Unknown:
        default:
            // Never should happen
            exit(EXIT_FAILURE);
    }
}

char *opt_field_to_str(OptField opt) {
    switch (opt) {
        case Port:
            return "Port";
        case LogggingLevel:
            return "LoggingLevel";
        // [Fallthrough]
        case Unknown:
            return "Unknown";
        default:
            // Never should happen
            exit(EXIT_FAILURE);
    };
}

Options *parse_opts(int32_t argc, char *argv[]) {
    if (argc <= 1) {
        printf("Too small arguments count. Write --help to see available options.\n");
        exit(0);
    }

    char **opt_token = &argv[1];

    static Options opts;
    opts.port = 0;
    opts.logging_level = 0;

    ParseStage stage = WaitForDecl;
    OptField opt = Unknown;

    OptValue val;
    val.iv = 0;

    while (*opt_token != NULL) {
        switch (stage) {
            case WaitForDecl:
                opt = parse_opt_field(*opt_token);
                stage = (opt != Unknown) ? WaitForValue : Interrupt;
                break;
            case WaitForValue:
                bool parse_status = true;
                val = parse_opt_value(*opt_token, opt, &parse_status);
                if (!parse_status) {
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

        if (stage == Interrupt) {
            break;
        }

        *opt_token++;
    }

    if (opt == Unknown && stage == Interrupt) {
        printf("Provided incorrect option: %s. Abort\n", *opt_token);
        exit(0);
    }

    if (stage == Interrupt || stage == WaitForValue) {
        printf("Value for %s option wasnt provied. Abort.", opt_field_to_str(opt));
        exit(0);
    }

    if (opts.port == 0) {
        printf("Mandatory --port option wasnt provided. Abort.");
        exit(0);
    }

    return &opts;
}