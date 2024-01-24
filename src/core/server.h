#ifndef SERVER_H
#define SERVER_H

#include "stdint.h"

struct HttpServer * createServer(char * host, int32_t port);
void releaseServer(struct HttpServer * server);

void start(struct HttpServer * server);


#endif