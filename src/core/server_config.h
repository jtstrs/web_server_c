#ifndef CONFIG_H
#define CONFIG_H

#include "../containers/hash_map.h"
#include "common.h"

#define STORAGE_PATH_SIZE 256

typedef struct ServerConfig {
    char host[HOST_NAME_LENGTH + 1];
    int32_t port;
    char storage_path[STORAGE_PATH_SIZE + 1];
} ServerConfig;

ServerConfig *parse_config(char *config_path);

#endif