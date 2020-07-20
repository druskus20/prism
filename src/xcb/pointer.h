#pragma once

#include <xcb/xcb.h>

xcb_query_pointer_cookie_t pointer_cookie;
xcb_query_pointer_reply_t *get_pointer_coordinates(void);
