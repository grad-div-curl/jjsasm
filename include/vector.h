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

/*
    Creates a `vector_t` in the heap with specified element size and capacity.

    @param `element_size` The size in bytes of each element. Should use `sizeof()` to find.
    @param `capacity` The starting capacity of the vector, prior to any under-the-hood resizing.

    @return A pointer to the instantiated vector, `vector_t*`. Should be freed using `vector_free()`.
*/
vector_t *vector_new(size_t element_size, size_t capacity);

/*
    Returns a pointer to the `i`th element of `vec`, or `NULL` if failed.
    The returned pointer can be cast to the intended type of the vector.

    @param `vec` A pointer to the `vector_t` to search in.
    @param `i` The index of the desired element.

    @return A pointer to the desired element, or `NULL` if the index was out of bounds (0 <= `i` < `vec->size`)
*/
void *vector_get(vector_t *vec, uint64_t i);

/*
    Appends an element to the end/top of `vec`.
    If no room is available, `vec->data` is reallocated with `vec->capacity` doubled.

    @param `vec` A pointer to the `vector_t` to push to.
    @param `element` A pointer to the element to push.

    @return A pointer to the pushed element in `vec`.
*/
void* vector_push(vector_t *vec, void *element);

/*
    Removes and returns the last element of `vec`, or `NULL` if not possible.
    No reallocation is performed if the vector size dips below previously smaller thresholds that initiated resizing.

    @param `vec` A pointer to the `vector_t` to pop from.
    @param `out` A buffer to write the element's data to.
    
    @return `out` if successful, `NULL` if vec->size == 0.
*/
void *vector_pop(vector_t *vec, void *out);

/*
    Returns the last element of 'vec', or 'NULL' if not possible.
    No reallocation is performed if the vector size dips below previously smaller thresholds that initiated resizing.

    @param `vec` A pointer to the `vector_t` to pop from.
    @param `out` A buffer to write the element's data to.

    @return 'out' if successful, 'NULL' if vec->size == 0.
*/
void *vector_poll(vector_t *vec, void *out);

/*
    Inserts `element` into index `i` of `vec`, pushing rightward elements right.
    Returns a pointer to the element in `vec`, or `NULL` if failed.
    If no room is available, `vec->data` is reallocated with `vec->capacity` doubled.

    @param `vec` A pointer to the `vector_t` to insert into.
    @param `element` A pointer to the element to insert.
    @param `i` The index to insert `element` into.

    @return A pointer to the inserted element in `vec`, or `NULL` if `i` was out of bounds.
*/
void *vector_insert(vector_t *vec, void *element, uint64_t i);

/*
    Removes the element of index `i` from `vec`, pulling rightward elements left.
    Returns `out` if successful, or `NULL` if failed.
    No reallocation is performed if the vector size dips below previously smaller thresholds that initiated resizing.

    @param `vec` A pointer to the `vector_t` to remove from.
    @param `out` A buffer to write the element's data to.
    @param `i` The index to remove from.

    @return `out` if the removal was successful, or `NULL` if `i` was out of bounds.
*/
void *vector_remove(vector_t *vec, void *out, uint64_t i);

/*
    Frees `vec` from memory using `free()`.

    @param `vec` The vector to free.
    @param `free_all` If `free_all == 1`, individually attempts to call `free()` on each non-`NULL` element.
*/
void vector_free(vector_t *vec, int free_all);

#endif