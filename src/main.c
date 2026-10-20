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
    json_value two = {JSON_TYPE_NUMBER, d_readas_u64(2)};
    json_value arr[] = {one, two};
    json_array_data arr_data = {2, arr};
    json_value arr_val = {JSON_TYPE_ARRAY, (uint64_t)&arr_data};
    printf("%s\n", json_tostring(one, 0));
    printf("%s\n", json_tostring(two, 0));

    char *str = json_tostring(arr_val, 0);
    printf("%s\n\n", str);

    if (str == NULL) printf("NULL");
    else while (*str != '\0'){
        printf("%x ", *str);
        str++;
    }
    
    return EXIT_SUCCESS;
}