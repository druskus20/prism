#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <poll.h>

#include "wm/manager.h"
#include "util/logging.h"
#include "xcb/connection.h"

int main(int argc, char **argv) {
    if (!initialize_prism()) {
        log_fatal("Unable to initialize prism.");
        return EXIT_FAILURE;
    }

    struct pollfd file_descriptors[] = {
        { xcb_file_descriptor, .events = POLLIN }
    };

    while (window_manager_is_active) {
        poll(file_descriptors, 1 /* number of descriptors*/, -1);

        if (file_descriptors[0].revents & POLLIN)
            handle_xcb_events();
    }

    finalize_prism();

    return EXIT_SUCCESS;
}
