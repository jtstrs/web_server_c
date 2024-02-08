#include "server.h"
#include "common.h"
#include "http_request.h"
#include "log.h"
#include "server_config.h"

#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int32_t init_from_config(HttpServer *server, ServerConfig *config) {
    // Probably most of them could have default values???
    strncpy(server->host_name, config->host, HOST_NAME_LENGTH);
    server->port = config->port;
    strncpy(server->content_storage_uri, config->storage_path, HOST_NAME_LENGTH);

    return 0;
}

int32_t init_default(HttpServer *server) {
    server->ac_sock = INVALID_DESCRIPTOR;
    AsyncContext *ctx = create_async_context();

    if (!ctx) {
        return -1;
    }

    server->execution_context = ctx;
    return 0;
}

HttpServer *create_server(ServerConfig *config) {
    if (!config) {
        log_message(ERROR_LEVEL, "Couldnt create server with an empty config");
        return NULL;
    }

    HttpServer *instance = (HttpServer *) malloc(sizeof(HttpServer));

    if (!instance) {
        log_message(ERROR_LEVEL, "Couldnt allocate memory for server instance");
        return NULL;
    }

    int32_t initialize_status = init_from_config(instance, config);
    if (initialize_status == -1) {
        free(instance);
        return NULL;
    }

    initialize_status = init_default(instance);
    if (initialize_status == -1) {
        free(instance);
        return NULL;
    }

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

    log_message(INFO_LEVEL, "Binding listening socket...");

    if (bind(sock_descr, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == ERROR_STATUS) {
        handle_error("bind");
    }

    log_message(INFO_LEVEL, "Start listening on %d port...", server->port);

    if (listen(sock_descr, BACKLOG_QUEUE) == ERROR_STATUS) {
        handle_error("listen");
    }

    server->ac_sock = sock_descr;
}

int32_t send_buffer_to_client(int32_t client_socket, void *data, size_t buffer_size) {
    return send(client_socket, data, buffer_size, 0);
}

int32_t accept_pending_connection(int32_t accept_sock_descr) {
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size = sizeof(struct sockaddr_in);
    memset((char *) &peer_addr, sizeof(peer_addr), 0);

    const int32_t peer_sock = accept(accept_sock_descr, (struct sockaddr *) &peer_addr, &peer_addr_size);

    if (peer_sock == -1) {
        handle_error("peer_sock accept");
    }

    log_message(INFO_LEVEL, "Accepted new connection from host %d", ntohs(peer_addr.sin_port));
    return peer_sock;
}

int32_t open_requested_document(HttpServer *server, char *url) {
    char document_path[PATH_MAX + 1];
    memset(document_path, 0, PATH_MAX);

    strcat(document_path, server->content_storage_uri);
    strcat(document_path, url);

    int32_t doc_desc = open(document_path, O_RDONLY);

    if (doc_desc == -1) {
        log_message(INFO_LEVEL, "Couldnt open document with path %s", document_path);
    } else {
        log_message(INFO_LEVEL, "Opened document with path %s", document_path);
    }

    return doc_desc;
}

HttpResponse *create_not_implemented_response(HttpServer *server, HttpRequest *request, int32_t conn_sock) {
    HttpResponse *response = create_response();

    if (!response) {
        return NULL;
    }

    response->code = NOT_IMPLEMENTED;
    response->version = get_request_version(request);

    return response;
}

HttpResponse *create_get_response(HttpServer *server, HttpRequest *request, int32_t conn_sock) {
    HttpResponse *response = create_response();

    if (!response) {
        return NULL;
    }

    int32_t rs_desc = open_requested_document(server, get_request_uri(request));

    if (read(rs_desc, response->response_body, MAX_RESPONSE_BODY_SIZE) == -1) {
        response->code = NOT_FOUND;
    } else {
        response->code = OK;
    }
    response->version = get_request_version(request);

    return response;
}

void handle_request(HttpServer *server, HttpRequest *request, int32_t conn_sock) {
    if (!request || !server) {
        return;
    }

    HttpResponse *response = NULL;
    HttpMethod method = get_request_method(request);
    char *url = get_request_uri(request);

    if (!url) {
        log_message(INFO_LEVEL, "Malicious request. Drop.");
        return;
    }

    switch (method) {
        case GET:
            log_message(INFO_LEVEL, "GET request %s", url);
            response = create_get_response(server, request, conn_sock);
            break;
        case OPTIONS:
        case HEAD:
        case POST:
        case PUT:
        case DELETE:
        case TRACE:
        case CONNECT:
        default:
            log_message(INFO_LEVEL, "Method %s is unsupported", http_method_to_str(method));
            response = create_not_implemented_response(server, request, conn_sock);
    }

    if (!response) {
        return;
    }

    char *response_buffer = serialize_response(response);
    send_buffer_to_client(conn_sock, response_buffer, strlen(response_buffer));

    release_response(response);
}

void handle_pending_request(HttpServer *server) {
    int32_t peer_sock = accept_pending_connection(server->ac_sock);

    char request_buffer[REQUEST_BUFFER_SIZE + 1];
    memset(request_buffer, 0, REQUEST_BUFFER_SIZE);

    int32_t summary_bytes_count = 0;
    int32_t bytes_read = read(peer_sock, request_buffer, REQUEST_BUFFER_SIZE);

    HttpRequest *request = parse_request(request_buffer);

    handle_request(server, request, peer_sock);

    close(peer_sock);
    free(request);
}

void run(HttpServer *server) {
    if (!server) {
        log_message(ERROR_LEVEL, "Server ptr is null");
        return;
    }
    // schedule_task(server->execution_context, handle_pending_request);
    execute(server->execution_context);
}