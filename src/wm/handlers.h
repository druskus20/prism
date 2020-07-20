#pragma once

#include <signal.h>
#include <xcb/xcb.h>

#ifndef SIGUNUSED
#define SIGUNUSED SIGSYS
#endif

extern void (*xcb_events[XCB_NO_OPERATION])(xcb_generic_event_t*);
extern void (*signals[SIGUNUSED])(void);

void handle_map_request(xcb_generic_event_t*);

void handle_button_down(xcb_generic_event_t*);
void handle_button_up(xcb_generic_event_t*);
void handle_pointer(xcb_generic_event_t*);

void handle_termination_signal(void);
