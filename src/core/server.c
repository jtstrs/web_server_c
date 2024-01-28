#include "server.h"
#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>


struct HttpServer {
    char host_name[HOST_NAME_LENGTH];
    int32_t port;
    int32_t ac_sock;
};


struct HttpServer *createServer(char *host, int32_t port) {
    struct HttpServer *instance = (struct HttpServer *) malloc(sizeof(struct HttpServer));

    strcpy(instance->host_name, host);
    instance->port = port;
    instance->ac_sock = INVALID_DESCRIPTOR;

    return instance;
}

void releaseSocket(int32_t socket_descriptor) {
    if (socket_descriptor != INVALID_DESCRIPTOR) {
        return;
    }

    int32_t closing_status = close(socket_descriptor);
    if (closing_status == ERROR_STATUS) {
        if (shutdown(socket_descriptor, SHUT_RDWR) == ERROR_STATUS) {
            handle_error("Cannot gracefuly close socket ");
        }
    }
}

void releaseServer(struct HttpServer *server) {
    if (!server) {
        return;
    }
    releaseSocket(server->ac_sock);
    free(server);
}

void initServer(struct HttpServer *server) {
    const int32_t sock_descr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifdef LOG
    printf("Create listening socket...\n");
#endif

    if (sock_descr == ERROR_STATUS) {
        handle_error("psocket");
    }

    struct sockaddr_in serv_addr;
    memset((char *) &serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(server->port);
    serv_addr.sin_family = AF_INET;

#ifdef LOG
    printf("Bindning listening socket...\n");
#endif

    if (bind(sock_descr, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == ERROR_STATUS) {
        handle_error("bind");
    }


#ifdef LOG
    printf("Start listening on %d port...\n", server->port);
#endif
    if (listen(sock_descr, BACKLOG_QUEUE) == ERROR_STATUS) {
        handle_error("listen");
    }

    server->ac_sock = sock_descr;
}

void start(struct HttpServer *server) {
    while (ALWAYS) {
        struct sockaddr_in peer_addr;
        socklen_t peer_addr_size;
        memset((char *) &peer_addr, sizeof(peer_addr), 0);

        const int32_t conn_sock_desc = accept(server->ac_sock, (struct sockaddr *) &peer_addr, &peer_addr_size);
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