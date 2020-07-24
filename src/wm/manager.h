#pragma once

#include <xcb/xcb.h> // REMOVE

#include "group.h"

#include "../globals.h"

extern unsigned char window_manager_is_active;

extern xcb_window_t focused_window; // REMOVE
extern group_t *focused_group;

extern vector_t *managed_windows;
extern vector_t *groups;

int window_manager(void);
void handle_xcb_events(void);
void handle_signals(int);
