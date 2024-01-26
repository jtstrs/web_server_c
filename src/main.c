#include <stdio.h>
#include <stdint.h>
#include "core/server.h"


int32_t main(int32_t argc, char const *argv[])
{
    struct HttpServer * server = createServer("localhost", 12000);
    start(server);
    return 0;
}
