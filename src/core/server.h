#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>

#include "common.h"
#include "http_request.h"
#include "http_response.h"
#include "log.h"
#include "server_config.h"

#define REQUEST_BUFFER_SIZE 2048
#define CONTENT_STORAGE_URI_SIZE 256

typedef struct HttpServer HttpServer;

HttpServer *create_server(ServerConfig *config);
void release_server(HttpServer *server);

void init_server(HttpServer *server);
void run(HttpServer *server);


#endif