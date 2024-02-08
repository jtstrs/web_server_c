#include "opt_parser.h"
#include "helpers.h"
#include "log.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union OptValue {
    int32_t iv;
} OptValue;

typedef enum OptField {
    Port,
    LogggingLevel,
    Unknown
} OptField;

typedef enum ParseStage {
    WaitForDecl,
    WaitForValue,
    Interrupt
} ParseStage;

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
    static Options opts;
    opts.port = DEFAULT_OPT_PORT;
    opts.logging_level = DEFAULT_OPT_LOGGING_LEVEL;

    if (argc <= 1) {
        log_message(DEBUG_LEVEL, "Cli options wasnt provided");
        return &opts;
    }

    char **opt_token = &argv[1];

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

    return &opts;
}