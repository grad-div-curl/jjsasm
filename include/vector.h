#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct vector_t {
    size_t element_size;
    size_t size;
    size_t capacity;
    void *data;
} vector_t;

vector_t *vector_new(size_t element_size, size_t capacity);
void *vector_get(vector_t *vec, uint64_t i);
void vector_push(vector_t *vec, void *element);
void *vector_pop(vector_t *vec, void *out);
void *vector_poll(vector_t *vec, void *out);
void vector_insert(vector_t *vec, void *element, uint64_t i);
void *vector_remove(vector_t *vec, void *out, uint64_t i);
void vector_free(vector_t *vec); // Does not free() each individual element, only vec and vec->data

#endif