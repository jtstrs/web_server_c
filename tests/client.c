#include "client.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOG
#define REMOTE_HOST_BUFFER_SIZE 64

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0);

struct Client
{
    char remote_host[REMOTE_HOST_BUFFER_SIZE];
    uint32_t remote_host_port;
};

struct Client* create_client(char * remote_host, uint32_t port) 
{
    struct Client * instance = (struct Client *)malloc(sizeof(struct Client));

    if (!instance) {
        handle_error("client alloc");
    }
    strcpy(instance->remote_host, remote_host);
    instance->remote_host_port = htonl(port);

    return instance;
}

void release_client(struct Client * client)
{
    if (!client) {
        return;
    }
    free(client);
}

void ping_request(struct Client * client)
{
#ifdef LOG
    printf("Creating socket for connecting to remote host...\n");
#endif

    int32_t sock_descr = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_descr == -1) {
        handle_error("sock descr")
    }

#ifdef LOG
    printf("Success. Socket descriptor %d\n", sock_descr);
#endif

    struct sockaddr_in remote_addr;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = client->remote_host_port;

#ifdef LOG
    printf("Resolving remote host %s:%d\n", client->remote_host, ntohl(client->remote_host_port));
#endif

    if (inet_pton(AF_INET, client->remote_host, &remote_addr.sin_addr) <= 0) {
        handle_error("host trans");
    }

#ifdef LOG
    printf("Success. Trying to connect to remote host\n");
#endif

    if (connect(sock_descr, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) == -1) {
        handle_error("connection");
    }

    const char ping_buffer[] = "PING";

#ifdef LOG
    printf("Success. Send \"PING\" message\n");
#endif

    send(sock_descr, ping_buffer, strlen(ping_buffer), MSG_CONFIRM);
}