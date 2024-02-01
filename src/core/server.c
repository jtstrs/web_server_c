#include "server.h"
#include "common.h"

#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "async_context.h"
#include <netinet/in.h>
#include <sys/socket.h>

#define REQUEST_BUFFER_SIZE 2048


struct HttpServer {
    char host_name[HOST_NAME_LENGTH];
    int32_t port;
    int32_t ac_sock;

    AsyncContext *execution_context;
};


HttpServer *create_server(char *host, int32_t port) {
    HttpServer *instance = (HttpServer *) malloc(sizeof(HttpServer));

    if (!instance) {
        return NULL;
    }

    strcpy(instance->host_name, host);

    instance->port = port;
    instance->ac_sock = INVALID_DESCRIPTOR;

    AsyncContext *ctx = create_async_context();

    if (!ctx) {
        free(instance);
        return NULL;
    }

    instance->execution_context = ctx;
    return instance;
}

void release_socket(int32_t socket_descriptor) {
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

void release_server(HttpServer *server) {
    if (!server) {
        return;
    }
    release_socket(server->ac_sock);
    release_async_context(server->execution_context);
    free(server);
}

void init_server(HttpServer *server) {

    if (!server) {
        return;
    }

    const int32_t sock_descr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock_descr == ERROR_STATUS) {
        handle_error("psocket");
    }

    struct sockaddr_in serv_addr;
    memset((char *) &serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(server->port);
    serv_addr.sin_family = AF_INET;

    log_message(INFO_LVL, "Binding listening socket...");

    if (bind(sock_descr, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == ERROR_STATUS) {
        handle_error("bind");
    }

    log_message(INFO_LVL, "Start listening on %d port...", server->port);

    if (listen(sock_descr, BACKLOG_QUEUE) == ERROR_STATUS) {
        handle_error("listen");
    }

    server->ac_sock = sock_descr;
}

int32_t accept_pending_connection(int32_t accept_sock_descr) {
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size = sizeof(struct sockaddr_in);
    memset((char *) &peer_addr, sizeof(peer_addr), 0);

    const int32_t peer_sock = accept(accept_sock_descr, (struct sockaddr *) &peer_addr, &peer_addr_size);

    if (peer_sock == -1) {
        handle_error("peer_sock accept");
    }

    log_message(INFO_LVL, "Accepted new connection from host %d", ntohs(peer_addr.sin_port));
    return peer_sock;
}

void handle_pending_request(HttpServer *server) {
    int32_t peer_sock = accept_pending_connection(server->ac_sock);

    char request_buffer[REQUEST_BUFFER_SIZE + 1];
    memset(request_buffer, 0, REQUEST_BUFFER_SIZE);

    int32_t summary_bytes_count = 0;
    int32_t bytes_read = read(peer_sock, request_buffer, REQUEST_BUFFER_SIZE);

    log_message(INFO_LVL, "Message content \n%s\n", request_buffer);
    close(peer_sock);
}

void run(HttpServer *server) {
    if (!server) {
        log_message(ERROR_LVL, "Server ptr is null");
        return;
    }
    // schedule_task(server->execution_context, handle_pending_request);
    execute(server->execution_context);
}