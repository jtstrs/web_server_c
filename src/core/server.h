#ifndef SERVER_H
#define SERVER_H

#include "async_context.h"
#include "common.h"
#include "http_request.h"
#include "http_response.h"
#include "log.h"
#include "stdint.h"

#define REQUEST_BUFFER_SIZE 2048
#define CONTENT_STORAGE_URI_SIZE 256

typedef struct HttpServer {
    char host_name[HOST_NAME_LENGTH + 1];
    int32_t port;
    int32_t ac_sock;

    char content_storage_uri[CONTENT_STORAGE_URI_SIZE + 1];
    AsyncContext *execution_context;
} HttpServer;

HttpServer *create_server(char *host, int32_t port);
void release_server(HttpServer *server);

void init_server(HttpServer *server);
void handle_pending_request(HttpServer *server);
void run(HttpServer *server);


#endif