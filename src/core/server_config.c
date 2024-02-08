#include "server_config.h"
#include "common.h"
#include "helpers.h"
#include "log.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CONFIG_LINE_SIZE 256
#define DEFAULT_PORT_VALUE 10000

void parse_line(ServerConfig *conf, char *line) {
    if (!line) {
        return;
    }

    static const char *delims = "=";
    char *key_token = strtok(line, delims);

    if (!key_token) {
        log_message(ERROR_LEVEL, "Couldnt parse config token");
        return;
    }

    char *value_token = strtok(NULL, delims);

    if (!value_token) {
        log_message(ERROR_LEVEL, "Incorrect value for token %s", key_token);
        return;
    }

    if (strcmp(key_token, "host")) {
        strncpy(conf->host, value_token, HOST_NAME_LENGTH);
    } else if (strcmp(key_token, "port")) {
        int32_t parsed_val = parse_int32(key_token);
        conf->port = (parsed_val == -1) ? DEFAULT_PORT_VALUE : parsed_val;
    } else if (strcmp(key_token, "storage_path")) {
        strncpy(conf->storage_path, value_token, STORAGE_PATH_SIZE);
    } else {
        log_message(DEBUG_LEVEL, "Unknown configuration option %s", key_token);
        exit(1);
    }
}

ServerConfig *parse_config(char *config_path) {
    static ServerConfig server_conf;

    FILE *conf = fopen(config_path, "r");

    if (conf == NULL) {
        log_message(ERROR_STATUS, "Incorrect config path %s", config_path);
        return NULL;
    }

    char line[MAX_CONFIG_LINE_SIZE + 1];
    memset(line, 0, MAX_CONFIG_LINE_SIZE);

    while (!ferror(conf) && !feof(conf) && fgets(line, MAX_CONFIG_LINE_SIZE, conf) != NULL) {
        log_message(DEBUG_LEVEL, "Parse config line: %s", line);
        parse_line(&server_conf, line);
        memset(line, 0, MAX_CONFIG_LINE_SIZE);
    }

    fclose(conf);
    if (ferror(conf)) {
        log_message(ERROR_LEVEL, "Couldnt open config file");
        exit(1);
    }

    return &server_conf;
}