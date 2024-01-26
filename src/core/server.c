#include "server.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

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
    printf("Start listening on %d port...\n", server->port);
#endif
    if (listen(inc_sock_descr, BACKLOG_QUEUE) == -1) {
        handle_error("listen");
    }

    while (ALWAYS) {
        struct sockaddr_in peer_addr;
        socklen_t peer_addr_size;
        memset((char*)&peer_addr, sizeof(peer_addr), 0);

        const int32_t conn_sock_desc = accept(inc_sock_descr, (struct sockaddr*)&peer_addr, &peer_addr_size);
        if (conn_sock_desc == -1) {
            handle_error("csocket");
        }

        printf("Accepted new connection from host %d\n", ntohs(peer_addr.sin_port));

        char buffer[INCOMMING_BUFFER_LENGTH];
        memset(buffer, 0, INCOMMING_BUFFER_LENGTH);

        if (read(conn_sock_desc, buffer, INCOMMING_BUFFER_LENGTH) == -1) {
            handle_error("read data");
        }

        printf("Received message %s\n", buffer);

        close(conn_sock_desc);
    }
}