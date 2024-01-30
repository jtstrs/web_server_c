#include <stdint.h>
#include <stdio.h>

#include "core/common.h"
#include "core/opt_parser.h"
#include "core/server.h"

int32_t main(int32_t argc, char *argv[]) {
    struct Options *opts = parse_opts(argc, argv);
    struct HttpServer *server = create_server("", opts->port);
    init_server(server);
    run(server);

    // release_server(server);

    return 0;
}
