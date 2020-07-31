#pragma once

#include "../ipc/parsing.h"

extern char *(*ipc_commands[IPC_CMD_NULL])(char**);

char *handle_quit_command(char**);
char *handle_test_command(char**);
