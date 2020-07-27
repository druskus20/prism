#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "prism.h"

#include "wm/manager.h"

unsigned int loglevel = 3;

int main(int argc, char **argv) {
    return window_manager();
}

/* Helpers */

vector_t *construct_vector() {
    vector_t *vector = calloc(1, sizeof(vector_t));
    vector->memory = 1;
    vector->size = 0;
    vector->elements = calloc(1, sizeof(void*));
    vector->remaining = 0;

    return vector;
}

void *vector_iterator(vector_t *vector) {
    if (!vector->remaining) {
        vector->remaining = vector->size;
        return NULL;
    }

    unsigned int index = (vector->size - vector->remaining);
    vector->remaining--;

    return get_from_vector(vector, index);
}

void reset_vector_iterator(vector_t *vector) {
    vector->remaining = vector->size;
}

void push_to_vector(vector_t *vector, void *data) {
    if (vector->size == vector->memory) {
        vector->memory *= 2;
        vector->elements = realloc(vector->elements,
            sizeof(void*) * (vector->memory + 1));
    }

    vector->elements[vector->size++] = (void*)data;
    vector->remaining++;
}

void pull_from_vector(vector_t *vector, unsigned int index) {
    if (index >= vector->size)
        return;

    for (; index < vector->size; index++)
        vector->elements[index] = vector->elements[index + 1];

    vector->elements[vector->size--] = NULL;
    vector->remaining--;

    if ((vector->size * 2) == vector->memory) {
        vector->memory /= 2;
        vector->elements = realloc(vector->elements,
            sizeof(void *) * (vector->memory + 1));
    }
}

void *get_from_vector(vector_t *vector, unsigned int index) {
    if (index >= vector->size)
        return NULL;

    return vector->elements[index];
}

void deconstruct_vector(vector_t *vector) {
    free(vector->elements);
    free(vector);
}

void _log(unsigned short level, const char *file, const char *function,
    const int line, char *formatting, ...) {

    /*
     * Loglevels:
     * 0 - Absolutely fucking nothing
     * 1 - Fatal
     * 2 - Message
     * 3 - Debug
     */

     if (loglevel < level)
        return;

    fprintf(stderr, "%s:%s L%d: ", file, function, line);

    va_list ap;
    va_start(ap, formatting);
    vfprintf(stderr, formatting, ap);
    va_end(ap);

    fputs("\n", stderr);
}
