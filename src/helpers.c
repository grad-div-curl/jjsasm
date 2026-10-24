#include "helpers.h"

uint64_t read_z64_as_u64(int64_t z64){
    uint64_t u64;
    memcpy(&u64, &z64, sizeof(uint64_t));
    return u64;
}

uint64_t read_d_as_u64(double d){
    uint64_t u64;
    memcpy(&u64, &d, sizeof(uint64_t));
    return u64;
}

int64_t read_u64_as_z64(uint64_t u64){
    int64_t z64;
    memcpy(&z64, &u64, sizeof(int64_t));
    return z64;
}

double read_u64_as_d(uint64_t u64){
    double d;
    memcpy(&d, &u64, sizeof(double));
    return d;
}

void dump(void *data, size_t n){
    for (int i = 0; i < n; i++){
        printf("%.2x ", ((char *)data)[n]);
    }
    puts("\n\n");
}

char *putspcs(char *str, unsigned int n){
    size_t len = strlen(str);
    char *spcstr = malloc(len + n + 1);
    for (int i = 0; i < n; i++){
        spcstr[i] = ' ';
    }
    memcpy(spcstr + n, str, len);
    return spcstr;
}

char *enquote(char *str){
    size_t len = strlen(str);
    char *qstr = malloc(len + 3);
    qstr[0] = '\"';
    qstr[len + 1] = '\"';
    qstr[len + 2] = '\0';
    memcpy(qstr + 1, str, len);
    return qstr;
}

char *concat(vector_t *str_vec){
    size_t total_len = 0;
    for (int i = 0; i < str_vec->size; i++){
        char *str = *(char **)vector_get(str_vec, i);
        total_len += strlen(str);
    }

    char *cnct = malloc(total_len + 1);
    char *ptr = cnct;

    for (int i = 0; i < str_vec->size; i++){
        char *str = *(char **)vector_get(str_vec, i);
        size_t len = strlen(str);
        strncpy(ptr, str, len);
        ptr += len;
    }

    cnct[total_len] = '\0';
    return cnct;
}