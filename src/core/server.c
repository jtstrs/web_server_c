#include "server.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"

#include <sys/socket.h>
#include <netinet/in.h>

#define ALWAYS 1
#define BACKLOG_QUEUE 10
#define INCOMMING_BUFFER_LENGTH 2048
#define HOST_NAME_LENGTH 64

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0);


#define LOG

struct HttpServer
{
    char host_name[HOST_NAME_LENGTH];
    int32_t port;
    int32_t conn_sock_descr;
};


struct HttpServer * createServer(char * host, int32_t port) {
    struct HttpServer * instance = (struct HttpServer*) malloc(sizeof(struct HttpServer));

    strcpy(instance->host_name, host);
    instance->port = port;

    return instance;
}

void releaseServer(struct HttpServer * server) {
    if (!server) {
        return;
    }
    free(server);
}

void start(struct HttpServer * server) {
    if (server->host_name == NULL) {
        handle_error("Hostname is null. Unable to start listening");
        return;
    }

    const int32_t inc_sock_descr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifdef LOG
    printf("Create listening socket...\n");
#endif

    if (inc_sock_descr == -1) {
        handle_error("psocket");
    }

    struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(server->port);
    serv_addr.sin_family = AF_INET;

#ifdef LOG
    printf("Bindning listening socket...\n");
#endif

    if (bind(inc_sock_descr, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        handle_error("bind");
    }


#ifdef LOG
    printf("Start listening...\n");
#endif
    if (listen(inc_sock_descr, BACKLOG_QUEUE) == -1) {
        handle_error("listen");
    }

    while (ALWAYS) {
        struct sockaddr_in peer_addr;
        socklen_t peer_addr_size;
        const int32_t conn_sock_desc = accept(inc_sock_descr, (struct sockaddr*)&peer_addr, &peer_addr_size);
        if (conn_sock_desc == -1) {
            handle_error("csocket");
        }

        char buffer[INCOMMING_BUFFER_LENGTH];
        recv(conn_sock_desc, buffer, INCOMMING_BUFFER_LENGTH, MSG_WAITALL);
        printf("Received message\n %s", buffer);
    }
}