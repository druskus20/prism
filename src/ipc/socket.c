#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "socket.h"

#include "../util/logging.h"

int socket_file_descriptor;

unsigned short initialize_socket(ipc_mode_t mode) {
    struct sockaddr_un address = {
        .sun_family = AF_UNIX,
        .sun_path   = "/tmp/prism.socket"
    };
    struct sockaddr *generic = (struct sockaddr*)&address;
    // Unix socket addr -> generic socket addr

    socket_file_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);

    if (!socket_file_descriptor) {
        log_fatal("Unable to initialize socket.");
        return 0;
    }

    switch (mode) {
        case WINDOW_MANAGER_IPC:
            if (bind(socket_file_descriptor, generic, sizeof(address)) < 0 ||
                listen(socket_file_descriptor, 1) < 0) {
                log_fatal("Unable to bind or listen to address.");
                return 0;
            }
            break;
        case CONTROLLER:
            if (connect(socket_file_descriptor, generic, sizeof(address)) < 0) {
                log_fatal("Unable to connect to socket.");
                return 0;
            }
            break;
    }

    return 1;
}

void write_to_socket(char *buffer) {
    write(socket_file_descriptor, buffer, 256);
}

char *read_from_socket() {
    int file_descriptor;

    if (!(file_descriptor = accept(socket_file_descriptor, NULL, 0))) {
        log_fatal("Unable to accept socket connection.");
        return NULL;
    }

    char *buffer = calloc(256, sizeof(char));
    read(file_descriptor, buffer, 256);

    return buffer;
}

void finalize_socket() {
    unlink("/tmp/prism.socket");
    close(socket_file_descriptor);
}
