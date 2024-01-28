#include <stdint.h>
#include <stdio.h>

#include "core/common.h"
#include "core/opt_parser.h"
#include "core/server.h"


int32_t main(int32_t argc, char *argv[]) {

    struct Options *opts = parse_opts(argc, argv);
    struct HttpServer *server = createServer("", opts->port);
    initServer(server);

    while (ALWAYS) {
        handle_pending_request(server);
    }

    releaseServer(server);
    return 0;
}
