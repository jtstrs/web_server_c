#ifndef SERVER_H
#define SERVER_H

#include "stdint.h"

typedef struct HttpServer HttpServer;

HttpServer *create_server(char *host, int32_t port);
void release_server(HttpServer *server);

void init_server(HttpServer *server);
void handle_pending_request(HttpServer *server);
void run(HttpServer *server);


#endif