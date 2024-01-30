#ifndef SERVER_H
#define SERVER_H

#include "stdint.h"

struct HttpServer *create_server(char *host, int32_t port);
void release_server(struct HttpServer *server);

void init_server(struct HttpServer *server);
void handle_pending_request(struct HttpServer *server);
void run(struct HttpServer *server);


#endif