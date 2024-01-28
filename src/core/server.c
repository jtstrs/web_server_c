#include "server.h"
#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>

#define REQ_CHUNK_SIZE 256


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

#ifdef DEBUG
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

#ifdef DEBUG
    printf("Bindning listening socket...\n");
#endif

    if (bind(sock_descr, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == ERROR_STATUS) {
        handle_error("bind");
    }


#ifdef DEBUG
    printf("Start listening on %d port...\n", server->port);
#endif
    if (listen(sock_descr, BACKLOG_QUEUE) == ERROR_STATUS) {
        handle_error("listen");
    }

    server->ac_sock = sock_descr;
}

int32_t accept_pending_connection(int32_t accept_sock_descr) {
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size;
    memset((char *) &peer_addr, sizeof(peer_addr), 0);

    const int32_t peer_sock = accept(accept_sock_descr, (struct sockaddr *) &peer_addr, &peer_addr_size);

    if (peer_sock == -1) {
        handle_error("peer_sock accept");
    }

#ifdef DEBUG
    printf("Accepted new connection from host %d\n", ntohs(peer_addr.sin_port));
#endif
    return peer_sock;
}

void handle_pending_request(struct HttpServer *server) {
    int32_t peer_sock = accept_pending_connection(server->ac_sock);

    char request_buffer[INCOMMING_BUFFER_LENGTH];
    memset(request_buffer, 0, INCOMMING_BUFFER_LENGTH);

    char chunk[REQ_CHUNK_SIZE];

    int32_t summary_bytes_count = 0;
    int32_t bytes_read = 0;
    do {
        memset(chunk, 0, REQ_CHUNK_SIZE);
        bytes_read = read(peer_sock, chunk, REQ_CHUNK_SIZE);
        if (bytes_read < 0) {
            handle_error("Reading data");
        }

        summary_bytes_count += bytes_read;

        if (summary_bytes_count >= INCOMMING_BUFFER_LENGTH) {
            handle_error("Request buffer overflow");
        }

        strcat(request_buffer, chunk);
    } while (bytes_read != 0);

    printf("Received message %s\n", request_buffer);

    close(peer_sock);
}
