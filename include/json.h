// I'm too lazy to use another person's json library so ima just make my own barebones one

#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

// Describes 
typedef struct json_value {
    json_type type;
    uint64_t data;
} json_value;

typedef struct json_kv {
    char *key;
    json_value val;
} json_kv;

typedef struct json_object_data {
    size_t size;
    json_value *data;
} json_object_data;

typedef struct json_array_data {
    size_t size;
    json_value *data;
} json_array_data;

#endif // JSON_H