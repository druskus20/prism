#pragma once

extern unsigned int loglevel;

void _log(unsigned short, const char*, const char*, const int, char*, ...);
#define log_fatal(...) _log(1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_message(...) _log(2, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_debug(...) _log(3, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define suppress_unused(...) (void)(__VA_ARGS__)
