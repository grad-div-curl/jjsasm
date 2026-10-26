#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vector.h"
#include "jjsasm.h"
#include "json.h"
#include "helpers.h"

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

    json_value nums_arr[] = {
        {JSON_TYPE_NUMBER, read_d_as_u64(1.0)},
        {JSON_TYPE_NUMBER, read_d_as_u64(2.0)},
        {JSON_TYPE_NUMBER, read_d_as_u64(3.0)}
    };
    json_array_data nums_ad = {sizeof(nums_arr) / 16, nums_arr};
    json_value nums = {JSON_TYPE_ARRAY, (uintptr_t)&nums_ad};

    json_value arr1_arr[] = {
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        {JSON_TYPE_BOOLEAN, JSON_TRUE}
    };
    json_array_data arr1_ad = {sizeof(arr1_arr) / 16, arr1_arr};
    json_value arr1 = {JSON_TYPE_ARRAY, (uintptr_t)&arr1_ad};

    json_value arr2_arr[] = {
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        arr1
    };
    json_array_data arr2_ad = {sizeof(arr2_arr) / 16, arr2_arr};
    json_value arr2 = {JSON_TYPE_ARRAY, (uintptr_t)&arr2_ad};

    json_value arr3_arr[] = {
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        {JSON_TYPE_BOOLEAN, JSON_TRUE},
        arr2
    };
    json_array_data arr3_ad = {sizeof(arr3_arr) / 16, arr3_arr};
    json_value arr3 = {JSON_TYPE_ARRAY, (uintptr_t)&arr3_ad};
    
    
    printf("%s\n", json_tostring(arr3));
    
    
    return EXIT_SUCCESS;
}