#pragma once

#include "group.h"
#include "window.h"

#include "../prism.h"

typedef enum {
    NORTHERN_SPLIT,
    SOUTHERN_SPLIT,
    EASTERN_SPLIT,
    WESTERN_SPLIT
} split_mode_t;

void split_current_view_port(split_mode_t, group_t*, window_t*, window_t*);
