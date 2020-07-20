#pragma once

extern unsigned char window_manager_is_active;

int window_manager(void);
void handle_xcb_events(void);
void handle_signals(int);
