#include "client.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOG

#define REMOTE_HOST_BUFFER_SIZE 64
#define REMOTE_ADDR_BUFFER_SIZE 16

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0);

struct Client
{
    // Logging stuff
    struct {
        char host_name[REMOTE_HOST_BUFFER_SIZE];
        char addr[REMOTE_ADDR_BUFFER_SIZE];
        uint32_t port;
    } base;

    // Network stuff
    struct {
        struct in_addr * addr;
        uint32_t port;
    } binary;
};

struct Client* create_client(char * remote_host, uint32_t port) 
{
    if (!remote_host) {
        handle_error("Cannot create client without provided host token");
    }

    struct Client * instance = (struct Client *)malloc(sizeof(struct Client));

    if (!instance) {
        handle_error("Cannot allocate memory for client instance");
    }

    const struct hostent * host_entry = gethostbyname(remote_host);

    if (!host_entry) {
        handle_error("host resolution");
    }

    strcpy(instance->base.host_name, host_entry->h_name);
    strcpy(instance->base.addr, inet_ntoa(*(struct in_addr*)host_entry->h_addr));
    instance->base.port = port;


    instance->binary.addr = (struct in_addr*) malloc(sizeof(struct in_addr));
    memcpy(instance->binary.addr, ((char*)((struct in_addr*)host_entry->h_addr)), sizeof(struct in_addr));

    instance->binary.port = htonl(port);

    return instance;
}

void release_client(struct Client * client)
{
    if (!client) {
        return;
    }

    if (client->binary.addr) {
        free(client->binary.addr);
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
    remote_addr.sin_port = client->binary.port;

#ifdef LOG
    printf("Resolving remote host %s[%s]:%d\n", client->base.host_name, client->base.addr, client->base.port);
#endif

    if (inet_pton(AF_INET, client->base.addr, &remote_addr.sin_addr) <= 0) {
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