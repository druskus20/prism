#pragma once

extern unsigned int loglevel;

typedef struct {
    void** elements;
    unsigned int memory;
    unsigned int size;
    unsigned int remaining;
} vector_t;

vector_t *construct_vector(void);
void *vector_iterator(vector_t*);
void reset_vector_iterator(vector_t*);
void push_to_vector(vector_t*, void*);
void pull_from_vector(vector_t*, unsigned int);
void *get_from_vector(vector_t*, unsigned int);
void deconstruct_vector(vector_t*);

void _log(unsigned short, const char*, const char*, const int, char*, ...);
#define log_fatal(...) _log(1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_message(...) _log(2, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_debug(...) _log(3, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define suppress_unused(...) (void)(__VA_ARGS__)
