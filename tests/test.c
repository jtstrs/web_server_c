#include <stdint.h>
#include <stdio.h>
#include "client.h"

#include "netdb.h"

int32_t main(int32_t argc, char * argv[]) {

    const char host_domain_name[] = "localhost";
    const char host_interface_name[] = "127.0.0.1";

    struct hostent * host_addr = gethostbyname(host_domain_name);

    printf("Hostent structure for localhost\n");
    printf("h_name: %s\n", host_addr->h_name);
    printf("h_length: %d\n", host_addr->h_length);
    printf("h_addrtype: %d\n", host_addr->h_addrtype);
    printf("h_addr: %s\n", host_addr->h_addr_list[1]);
    printf("h_addr: %s\n", host_addr->h_aliases[0]);


    // struct Client * client = create_client("localhost", 12000); 
    // ping_request(client);
    // release_client(client);
}