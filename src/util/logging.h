#pragma once

typedef enum {
    NO_LOGGING,
    FATAL_LOGGING,
    MESSAGE_LOGGING,
    DEBUG_LOGGING,
} logging_level_t;

extern logging_level_t loglevel;

void _log(logging_level_t, const char*, const char*, const int, char*, ...);
#define log_fatal(...) _log(1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_message(...) _log(2, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_debug(...) _log(3, __FILE__, __func__, __LINE__, __VA_ARGS__)
