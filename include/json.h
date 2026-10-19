/*
    I'm too lazy to use another person's json library so ima just make my own barebones one
    I also lwk don't even need to convert from string to JSON but it's a fun activity
*/

#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
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

typedef enum json_bool {
    JSON_FALSE = 0,
    JSON_TRUE = 1
} json_bool;

typedef enum json_type {
    JSON_TYPE_NULL,
    JSON_TYPE_NUMBER,
    JSON_TYPE_STRING,
    JSON_TYPE_BOOLEAN,
    JSON_TYPE_ARRAY,
    JSON_TYPE_OBJECT
} json_type;

/*
    Describes a number, string, boolean, etc. just anything
    `uint64_t data` contains whatever value is stored READ (not cast or converted, READ) as a `uint64_t`
    Store data in variable `V` as `*(uint64_t *)&V`
*/
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

json_value json_parse(char *str);
char *json_tostring(json_value start);

#endif // JSON_H