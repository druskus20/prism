#include <stdlib.h>

#include "controller.h"
#include "socket.h"

#include "../util/logging.h"

int prism_controller(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if (!initialize_socket(CONTROLLER))
        return EXIT_FAILURE;

    write_to_socket("-q", socket_file_descriptor);
    log_debug("%s", read_from_socket(socket_file_descriptor));

    finalize_socket();

    return EXIT_SUCCESS;
}
