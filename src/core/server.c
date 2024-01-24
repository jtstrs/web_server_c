#include "server.h"
#include "stdlib.h"

struct HttpServer
{
    char * host_name;
    int32_t port;
    int32_t conn_sock_descr;
};


struct HttpServer * createServer(char * host, int32_t port) {
    struct HttpServer * instance = (struct HttpServer*) malloc(sizeof(struct HttpServer));
    return instance;
}

void releaseServer(struct HttpServer * server) {
    if (!server) {
        return;
    }
    free(server);
}