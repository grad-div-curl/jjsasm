#include "vector.h"
#include <stdio.h> // remember to remove this

vector_t *vector_new(size_t element_size, size_t capacity){
    vector_t *vec = malloc(sizeof(vector_t));

    vec->element_size = element_size;
    vec->size = 0;
    vec->capacity = capacity;
    vec->data = malloc(element_size * capacity);

    return vec;
}

void *vector_get(vector_t *vec, uint64_t i){
    if (i >= vec->size) return NULL;
    return vec->data + vec->element_size * i;
}

void* vector_push(vector_t *vec, void *element){
    if (vec->size == vec->capacity){
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->capacity);
    }
    memcpy(vec->data + vec->element_size * vec->size, element, vec->element_size);
    return vec->data + vec->element_size * vec->size++;
}

// returns out if an element exists, NULL if vec->size == 0
void *vector_pop(vector_t *vec, void *out){
    if (vec->size == 0) return NULL;
    vec->size--;
    // data[size] now points to the former last element
    memcpy(out, vec->data + vec->size * vec->element_size, vec->element_size);
    return out;
}

// returns out if an element exists, NULL if vec->size == 0
void *vector_poll(vector_t *vec, void *out){
    if (vec->size == 0) return NULL;
    memcpy(out, vec->data + (vec->size - 1) * vec->element_size, vec->element_size);
    return out;
}

void *vector_insert(vector_t *vec, void *element, uint64_t i){
    if (i > vec->size) return NULL;

    size_t e_size = vec->element_size;

    if (vec->size == vec->capacity){
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->capacity);
    }

    uint8_t prev[e_size];
    memcpy(prev, element, e_size);

    uint8_t next[e_size];
    
    // the last 'next' is discarded anyways
    for (int j = i; j <= vec->size; j++){
        memcpy(next, vec->data + e_size * j, e_size); // copy current element to 'next'
        memcpy(vec->data + e_size * j, prev, e_size);  // copy 'prev' to current element
        memcpy(prev, next, e_size); // copy 'next' to 'prev'
    }

    vec->size++;
    return vec->data + i * e_size;
}

void *vector_remove(vector_t *vec, void *out, uint64_t i){
    if (i >= vec->size) return NULL;

    size_t e_size = vec->element_size;

    uint8_t next[e_size];
    memcpy(next, vec->data + e_size * (vec->size - 1), e_size);

    uint8_t prev[e_size];

    // Me getting to use the same algorithm for both insert and remove is proof of a higher being
    // If only I could find a way to remove that pesky && to better handle when j is negative
    for (int j = vec->size-1; j >= i && j >= 0; j--){
        memcpy(next, vec->data + e_size * j, e_size); // copy current element to 'next'
        memcpy(vec->data + e_size * j, prev, e_size);  // copy 'prev' to current element
        memcpy(prev, next, e_size); // copy 'next' to 'prev'
    }

    vec->size--;
    memcpy(out, next, e_size);
    return out;
}

void vector_free(vector_t *vec, int free_all){
    if (free_all == 1){
        for (int i = 0; i < vec->size; i++){
            void *v = vector_get(vec, i);
            if (v != NULL) free(v);
        }
    }

    free(vec->data);
    free(vec);
}