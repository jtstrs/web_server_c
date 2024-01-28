#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>

struct Client *create_client(char *, uint32_t);
void release_client(struct Client *);
void ping_request(struct Client *client);

#endif