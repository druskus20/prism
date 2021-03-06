#include "ipc.h"

#include "../wm/manager.h"

char *(*ipc_commands[IPC_CMD_NULL])(char**) = {
    [IPC_CMD_QUIT] = handle_quit_command,
    [IPC_CMD_SET_TILE_DIRECTION] = handle_tile_command
};

char *handle_quit_command(char **args) {
    (void)args;

    window_manager_is_active = 0;
    return NULL;
}

char *handle_tile_command(char **args) {
    return "test";
}
