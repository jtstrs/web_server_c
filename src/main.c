#include <stdio.h>
#include <stdint.h>

#include "core/server.h"
#include "core/opt_parser.h"


int32_t main(int32_t argc, char *argv[])
{
    
    struct Options * opts = parse_opts(argc, argv);
    struct HttpServer * server = createServer("", opts->port);
    start(server);

    releaseServer(server);
    return 0;
}
