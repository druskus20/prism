#include "signal.h"

#include "../wm/manager.h"

void (*signals[SIGUNUSED])(void) = {
    [SIGINT]  = handle_termination_signal,
    [SIGTERM] = handle_termination_signal
};

/* Process signal handling */
void handle_termination_signal() {
    window_manager_is_active = 0;
}
