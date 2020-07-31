#pragma once

typedef enum {
    IPC_CMD_QUIT,

    IPC_CMD_NULL = 256
} ipc_command_t;

typedef struct {
    char *type;
    void *data;
} ipc_arg_t;
