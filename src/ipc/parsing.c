#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"

ipc_command_t ipc_get_command(char **args) {
    char *command = args[0];

    if (!strcmp(command, "-q"))
        return IPC_CMD_QUIT;
    else if (!strcmp(command, "-T"))
        return IPC_CMD_SET_TILE_DIRECTION;

    return IPC_CMD_NULL;
}

char **ipc_split_input(char *buffer) {
    char **argv = calloc(1, sizeof(char*));
    unsigned int argc  = 1;
    unsigned int index = 0;

    char *token;
    while ((token = strsep(&buffer, " "))) {
        if (index + 1 > argc) {
            argc++;
            argv = realloc(argv, sizeof(char*) * argc);
        }

        argv[index] = token;
        printf("%s\n", argv[index]);
        index++;
    }

    return argv;
}
