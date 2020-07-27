#pragma once

#include "window.h"

#include "../prism.h"

typedef struct {
    unsigned int x, y, height, width;
    vector_t *children;
} group_t;

group_t *initialize_group(void);
void push_window_to_group(group_t*, window_t*);
void move_group(group_t*, unsigned int, unsigned int);
void resize_group(group_t *, unsigned int, unsigned int);
