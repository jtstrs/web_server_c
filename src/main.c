#include <stdint.h>

#include "core/common.h"
#include "core/log.h"
#include "core/opt_parser.h"
#include "core/server.h"
#include "core/server_config.h"

#define DEFAULT_CONFIG_PATH "resources/sample_config.conf"

void merge_args(ServerConfig *srv_conf, Options *cli_opts) {
    if (!srv_conf) {
        log_message(ERROR_LEVEL, "Couldnt create server instance without proper configuration");
        exit(1);
    }

    if (cli_opts->port != DEFAULT_OPT_PORT) {
        log_message(DEBUG_LEVEL, "Override config port value with value provided by cli");
        srv_conf->port = cli_opts->port;
    }
}

int32_t main(int32_t argc, char *argv[]) {

    ServerConfig *srv_conf = parse_config(DEFAULT_CONFIG_PATH);
    Options *opts = parse_opts(argc, argv);
    merge_args(srv_conf, opts);

    Logger *logger = get_default_logger();
    set_accept_log_level(logger, opts->logging_level);

    HttpServer *server = create_server(srv_conf);
    init_server(server);
    run(server);
    release_server(server);

    return 0;
}
