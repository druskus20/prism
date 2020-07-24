#pragma once

#include "../globals.h"

typedef struct {
    unsigned int x, y, height, width;
    vector_t *children;
} group_t;

group_t *initialize_group(void);
void move_group(group_t*, unsigned int, unsigned int);
