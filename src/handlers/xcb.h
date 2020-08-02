#pragma once

#include <xcb/xcb.h>

extern void (*xcb_events[XCB_NO_OPERATION])(xcb_generic_event_t*);

void handle_window_map_request(xcb_generic_event_t*);
void handle_window_destruction(xcb_generic_event_t*);

void handle_button_down(xcb_generic_event_t*);
void handle_button_up(xcb_generic_event_t*);
void handle_pointer(xcb_generic_event_t*);
