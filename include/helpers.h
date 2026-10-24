// Helper functions I use throughout the project that are just generally useful
// Includes functions I only use in like one file but want to have handy for later

#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vector.h"

/*
    Byte reading
        uint64_t read_z64_as_u64(int64_t z64);
        uint64_t read_d_as_u64(double d);
        int64_t read_u64_as_z64(uint64_t u64);
        double read_u64_as_d(uint64_t u64);
        void dump(void *data, size_t n);
    String manipulation
        char *putspcs(char *src, unsigned int n);
        char *enquote(char *src);
        char *concat(vector_t *str_vec)
*/

// Reads an `int64_t`'s bytes as a `uint64_t`.
uint64_t read_z64_as_u64(int64_t z64);

//  Reads a `double`'s bytes as a `uint64_t`.
uint64_t read_d_as_u64(double d);

// Reads a `uint64_t`'s bytes as an `int64_t`.
int64_t read_u64_as_z64(uint64_t u64);

// Reads a `uint64_t`'s bytes as a `double`.
double read_u64_as_d(uint64_t u64);

// Dumps `n` bytes of `data` into stdout as hexadecimal.
// Mostly for debugging.
void dump(void *data, size_t n);

// Returns a string with `n` spaces concatenated to the front of `src`.
// Must be freed.
char *putspcs(char *src, unsigned int n);

// Returns a copy of `src` enclosed within quotation marks ("").
// Must be freed.
char *enquote(char *src);

/*
    Returns the concatenation of all items in a string vector.
    Returned string is allocated to heap, must be freed

    @param `str_vec` A `vector_t` containing elements of type `char *`.

    @return The concatenation of all strings in `str_vec`. Must be freed using free() once done.
*/
char *concat(vector_t *str_vec);


#endif // HELPERS_H