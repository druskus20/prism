#pragma once

#include <signal.h>

#ifndef SIGUNUSED
#define SIGUNUSED SIGSYS
#endif

extern void (*signals[SIGUNUSED])(void);

void handle_termination_signal(void);
