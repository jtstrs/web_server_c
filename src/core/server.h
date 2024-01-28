#ifndef SERVER_H
#define SERVER_H

#include "stdint.h"

struct HttpServer *createServer(char *host, int32_t port);
void releaseServer(struct HttpServer *server);

void initServer(struct HttpServer *server);
void handle_pending_request(struct HttpServer *server);


#endif