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

    log_message(DEBUG_LEVEL, "Key config token: %s", key_token);

    char *value_token = strtok(NULL, delims);

    if (!value_token) {
        log_message(ERROR_LEVEL, "Incorrect value for token %s", key_token);
        return;
    }

    log_message(DEBUG_LEVEL, "Value token: %s", value_token);

    if (strcmp(key_token, "host") == 0) {
        strncpy(conf->host, value_token, HOST_NAME_LENGTH);
    } else if (strcmp(key_token, "port") == 0) {
        int32_t parsed_val = parse_int32(value_token);
        conf->port = (parsed_val == -1) ? DEFAULT_PORT_VALUE : parsed_val;
    } else if (strcmp(key_token, "storage_path") == 0) {
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
        parse_line(&server_conf, line);
        memset(line, 0, MAX_CONFIG_LINE_SIZE);
    }

    if (ferror(conf)) {
        log_message(ERROR_LEVEL, "Couldnt open config file. Error code: %d", ferror(conf));
        exit(1);
    }

    fclose(conf);

    return &server_conf;
}