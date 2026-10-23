#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vector.h"
#include "jjsasm.h"
#include "json.h"

static uint64_t d_readas_u64(double d) {
    return *(uint64_t *)&d;
}

int main(int argc, char **argv){
    /*
    char *str = calloc(2048, sizeof(str));
    char str_ph[] = "ADD 1 2\nCMP 3 4";

    if (argc < 2) {
        str = str_ph;
    }
    else {
        FILE *fp = fopen(argv[1], "rb");
        if (fp == NULL){
            perror("File not found");
            exit(EXIT_FAILURE);
        }
        fread(str, sizeof(char), 2048, fp);
        fclose(fp);
    }

    vector_t *vec = jjsasm_tokenize(str);
    if (vec == NULL)
        printf("null");
    else
        jjsasm_print_token_vector(vec);
    // */

    json_value one = {JSON_TYPE_NUMBER, d_readas_u64(1)};
    json_value vtrue = {JSON_TYPE_BOOLEAN, JSON_TRUE};
    json_value hello = {JSON_TYPE_STRING, (uintptr_t)"Hello, World!"};
    json_value vnull = {JSON_TYPE_NULL, 0};
    
    json_value two = {JSON_TYPE_NUMBER, d_readas_u64(2)};
    json_value nums[] = {one, two};
    json_array_data nums_data = {2, nums};
    json_value nums_arr = {JSON_TYPE_ARRAY, (uintptr_t)&nums_data};

    json_value vals[] = {one, vtrue, hello, vnull, nums_arr};
    json_array_data arr_data = {sizeof(vals) / sizeof(json_value), vals};
    json_value arr = {JSON_TYPE_ARRAY, (uintptr_t)&arr_data};
    
    printf(json_tostring(arr, 0, 0));
    
    return EXIT_SUCCESS;
}