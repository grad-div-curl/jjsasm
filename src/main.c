#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vector.h"
#include "jjsasm.h"
#include "json.h"

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

    
    
    return EXIT_SUCCESS;
}