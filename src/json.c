#include "json.h"

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
    Helpers
            static uint64_t ll_read_u64(long long ll);
            static uint64_t d_read_u64(double d);
            static long long u64_read_ll(uint64_t u64);
            static double u64_read_d(uint64_t u64);
            static inline void putspcs(unsigned int n);
    json_value json_parse(char *str);
    char *json_tostring(json_value start);

*/

// evil floating point bit level hacking 2: electric boogaloo

static uint64_t d_readas_u64(double d){
    return *(uint64_t *)&d;
}
static double u64_readas_d(uint64_t u64){
    return *(double *)&u64;
}

// Returns an identical string with n spaces at the front
static char *putspcs(char *src, unsigned int n){
    int len = strlen(src);
    char *dst = calloc(len + n + 1, sizeof(char));
    for (int i = 0; i < n; i++){
        dst[i] = ' ';
    }
    strncpy(dst + n*sizeof(char), src, len);
    return dst;
}

char *json_tostring(json_value val, unsigned int indent){
    switch (val.type){
        case JSON_TYPE_NULL: return "NULL";
        case JSON_TYPE_BOOLEAN:
            if (val.data == JSON_TRUE) return "true";
            if (val.data == JSON_FALSE) return "false";
            return "?";
        case JSON_TYPE_NUMBER:
            char *tmp = malloc(16 * sizeof(char));
            sprintf(tmp, "%lf", u64_readas_d(val.data));
            char *ret = putspcs(tmp, indent);
            free(tmp);
            return ret;
        case JSON_TYPE_STRING:
            return putspcs((char *)val.data, indent);
        case JSON_TYPE_ARRAY:
            json_array_data arr = *(json_array_data *)val.data;
            vector_t *chvec = vector_new(sizeof(char), 64);
            
            vector_push(chvec, putspcs("[\n", indent));
            for (int i = 0; i < arr.size; i++){
                vector_push(chvec, putspcs(json_tostring(arr.data[i], JSON_TAB_SIZE), indent));
                if (i != arr.size - 1) vector_push(chvec, ",\n");
            }
            vector_push(chvec, "\n");
            vector_push(chvec, putspcs("]", indent));

            int total_len = 0;
            for (int i = 0; i < chvec->size; i++){
                total_len += strlen(vector_get(chvec, i));
            }

            char *str = malloc(total_len + 1);
            for (int i = 0; i < chvec->size; i++){
                char *ch_i = (char *)vector_get(chvec, i);
                int len = strlen(ch_i);
                strncpy(str, ch_i, len);
                str += len;
            }
            str[total_len + 1] = '\0';
            vector_free(chvec);
            return str;
        case JSON_TYPE_OBJECT:
            // unimplemented
    }
}