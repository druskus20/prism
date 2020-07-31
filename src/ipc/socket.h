#pragma once

extern int socket_file_descriptor;

typedef enum {
    WINDOW_MANAGER_IPC,
    CONTROLLER
} ipc_mode_t;

unsigned short initialize_socket(ipc_mode_t);
char *read_from_socket(void);
void finalize_socket(void);
