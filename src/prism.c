#include <stdio.h>
#include <stdarg.h>

#include "globals.h"

#include "wm/manager.h"

unsigned int loglevel = 3;

int main(int argc, char **argv) {
    return window_manager();
}

void _log(unsigned short level, const char *file, const char *function,
    const int line, char *formatting, ...) {

    /*
     * Loglevels:
     * 0 - Absolutely fucking nothing
     * 1 - Fatal
     * 2 - Message
     * 3 - Debug
     */

     if (loglevel < level)
        return;

    fprintf(stderr, "%s:%s L%d: ", file, function, line);

    va_list ap;
    va_start(ap, formatting);
    vfprintf(stderr, formatting, ap);
    va_end(ap);

    fputs("\n", stderr);
}
