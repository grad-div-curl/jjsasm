/*
    I'm too lazy to use another person's json library so ima just make my own barebones one
    I also lwk don't even need to convert from string to JSON but it's a fun activity
    Don't expect this to be fast it's not for actual bigass databases it's for jjsasm scripts
*/

#ifndef JSON_H
#define JSON_H

#ifndef JSON_RECURSE_MAX
#define JSON_RECURSE_MAX 100
#endif
#ifndef JSON_TAB_SIZE
#define JSON_TAB_SIZE 2
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vector.h"
#include "helpers.h"

/*

Defines
    JSON_RECURSE_MAX
    JSON_TAB_SIZE
Types
    Enums
        json_bool
        json_type
    Types
        json_value
        json_kv_pair
        json_object_data
        json_array_data
Functions
    json_value json_parse(char *str);
    char *json_tostring(json_value start);

*/

/*
    Boolean values for JSON.

    `JSON_FALSE`: False, or `0`.
    `JSON_TRUE`:  True, or `1`.
*/
typedef enum json_bool {
    JSON_FALSE = 0,
    JSON_TRUE = 1
} json_bool;

/*
    Types used in JSON.

    `JSON_TYPE_NULL`:    Specifies no data.
    `JSON_TYPE_NUMBER`:  A `double` or similar 64-bit floating-point number.
    `JSON_TYPE_STRING`:  A `char *` pointing to a null-terminated string of text.
    `JSON_TYPE_BOOLEAN`: A `json_bool` value, either `JSON_FALSE` or `JSON _TRUE`.
    `JSON_TYPE_ARRAY`:   A `json_array_data *` value.
    `JSON_TYPE_OBJECT`:  A `json_object_data *` value.
*/
typedef enum json_type {
    JSON_TYPE_NULL,
    JSON_TYPE_NUMBER,
    JSON_TYPE_STRING,
    JSON_TYPE_BOOLEAN,
    JSON_TYPE_ARRAY,
    JSON_TYPE_OBJECT
} json_type;

/*
    Determines indentation for json_tostring().
    
    JSON_INDENT_NONE   : No indentation or newlines.
    JSON_INDENT_NOFIRST: Indent, but not the first line.
    JSON_INDENT_ALL    : Indent.
*/
typedef enum json_indent_flag {
    JSON_INDENT_NONE,
    JSON_INDENT_NOFIRST,
    JSON_INDENT_ALL
} json_indent_flag;

// Describes a JSON value using its type and stored data.
typedef struct json_value {
    json_type type;
    uint64_t data;
} json_value;

// Self explanatory, it's a key:value pair
typedef struct json_kv_pair {
    char *key;
    json_value val;
} json_kv_pair;

// Stores data that would be contained in an object
typedef struct json_object_data {
    size_t size;
    json_kv_pair *data;
} json_object_data;

// Stores data that would be contained in an array
typedef struct json_array_data {
    size_t size;
    json_value *data;
} json_array_data;

/*
    Parses a string into a JSON value

    @param str String to be parsed
    @return Parsed JSON value, or 
*/
json_value json_parse(char *str);

/*
    Returns a string representing a JSON value, no indentation
    The returned string is allocated to the heap and must be freed using free()

    @param val JSON value to be parsed
    
    @return Parsed JSON
*/
char *json_tostring(json_value val);

#endif // JSON_H