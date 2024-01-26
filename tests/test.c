#include "client.h"

int32_t main(int32_t argc, char * argv[]) {
    struct Client * client = create_client("localhost", 12000); 
    ping_request(client);
    release_client(client);
}