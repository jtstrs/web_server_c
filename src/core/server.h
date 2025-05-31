#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>

#include "../containers/hash_map.h"
#include "common.h"
#include "http_request.h"
#include "http_response.h"
#include "log.h"
#include "server_config.h"

#define REQUEST_BUFFER_SIZE 2048
#define CONTENT_STORAGE_URI_SIZE 256

typedef struct HttpServer {
    char host_name[HOST_NAME_LENGTH + 1];
    int32_t port;
    int32_t ac_sock;

    char content_storage_uri[CONTENT_STORAGE_URI_SIZE + 1];
} HttpServer;

HttpServer *create_server(ServerConfig *config);
void release_server(HttpServer *server);

void init_server(HttpServer *server);
void handle_pending_request(HttpServer *server);
void run(HttpServer *server);


#endif