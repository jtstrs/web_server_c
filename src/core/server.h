#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

struct HttpServer * createServer(char * host, int32_t port);
void releaseServer(struct HttpServer * server);


#endif