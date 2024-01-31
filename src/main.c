#include <stdint.h>

#include "core/common.h"
#include "core/opt_parser.h"
#include "core/server.h"

int32_t main(int32_t argc, char *argv[]) {
    struct Options *opts = parse_opts(argc, argv);
    struct HttpServer *server = create_server("", opts->port);
    init_server(server);

    while (ALWAYS) {
        handle_pending_request(server);
    }
    release_server(server);

    return 0;
}
