#include "json.h"

/*

Variables
    Integers
        static int recurse;
    Strings
        static char *lbrac;
        static char *rbrac;
        static char *commanl;
        static char *nl;
Functions
    Helpers
        Pointer manipulation
            static uint64_t ll_read_u64(long long ll);
            static uint64_t d_read_u64(double d);
            static long long u64_read_ll(uint64_t u64);
            static double u64_read_d(uint64_t u64);
        String manipulation
            static char *putspcs(char *src, unsigned int n);
            static char *enquote(char *src);
            static char *concat(vector_t *str_vec);
    char *json_tostring(json_value start);
    json_value json_parse(char *str);
*/

/*
    This counter is incremented every time `json_tostring()` is called,
    and decremented every time `json_tostring()` returns. If this counter
    exceeds `JSON_RECURSE_MAX` (default = 100) due to `json_tostring()`'s
    recursion, `json_tostring()` will begin to return `"null"` regardless of
    input.
*/
static int recurse = 0;

static char *lbrac = "[\n";
static char *rbrac = "]";
static char *commanl = ",\n";
static char *nl = "\n";

// evil floating point bit level hacking 2: electric boogaloo

static uint64_t d_readas_u64(double d){
    return *(uint64_t *)&d;
}
static double u64_readas_d(uint64_t u64){
    return *(double *)&u64;
}

// Returns an identical string with n spaces at the front
// Returned string is allocated to heap, must be freed
static char *putspcs(char *src, unsigned int n){
    size_t len = strlen(src);
    char *dst = malloc(len + n + 1);
    for (int i = 0; i < n; i++){
        dst[i] = ' ';
    }
    strncpy(dst + n*sizeof(char), src, len);
    return dst;
}

// Returns the string in quotes
// Returned string is allocated to heap, must be freed
static char *enquote(char *src){
    size_t len = strlen(src);
    char *dst = malloc(len + 3);
    dst[0] = dst[len + 1] = '\"';
    dst[len + 2] = '\0';
    strncpy(dst, src, len);
    return dst;
}

/*
    Returns the concatenation of all items in a string vector.
    Returned string is allocated to heap, must be freed

    @param `str_vec` A `vector_t` containing elements of type `char *`.

    @return The concatenation of all strings in `str_vec`. Must be freed using free() once done.
*/
static char *concat(vector_t *str_vec){
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

char *json_tostring(json_value val, unsigned int indent, int first_indent){
    recurse++;
    if (recurse > JSON_RECURSE_MAX) {
        perror("Recurse maximum reached!");
        recurse--;
        return putspcs("null", indent);
    }

    char *ret;
    switch (val.type){
        case JSON_TYPE_NULL:
            ret = (first_indent ? putspcs("null", indent) : "null");
            break;
        case JSON_TYPE_BOOLEAN:
            if (val.data == JSON_TRUE)
                ret = (first_indent ? putspcs("true", indent) : "true");
            else if (val.data == JSON_FALSE)
                ret = (first_indent ? putspcs("false", indent) : "false");
            else
                ret = (first_indent ? putspcs("null", indent) : "null");
            break;
        case JSON_TYPE_NUMBER:
            char *tmp = malloc(16);
            sprintf(tmp, "%lf", u64_readas_d(val.data));
            if (first_indent) {
                ret = putspcs(tmp, indent);
                free(tmp);
            }
            else ret = tmp;
            break;
        case JSON_TYPE_STRING:
            char *qstr = enquote((char *)val.data);
            if (first_indent) {
                ret = putspcs(qstr, indent);
                free(qstr);
            }
            else ret = qstr;
            break;
        case JSON_TYPE_ARRAY:
            json_array_data arr = *(json_array_data *)val.data;

            vector_t *strvec = vector_new(sizeof(char *), 64);
            char **free_later = malloc(sizeof(char *) * arr.size); // strings put here are freed later
            char *indented_str = putspcs("", indent);
            
            if (first_indent) vector_push(strvec, &indented_str);
            vector_push(strvec, &lbrac);

            for (int i = 0; i < arr.size; i++){
                char *str = json_tostring(arr.data[i], JSON_TAB_SIZE, 1);
                vector_push(strvec, &indented_str);
                vector_push(strvec, &str);

                free_later[i] = str;
                if (i != arr.size - 1) vector_push(strvec, &commanl);
            }
            vector_push(strvec, &nl);
            vector_push(strvec, &indented_str);
            vector_push(strvec, &rbrac);
            

            ret = concat(strvec);

            free(indented_str);
            for (int i = 0; i < arr.size; i++){
                free(free_later[i]);
            }
            free(free_later);
            vector_free(strvec);
            break;
        case JSON_TYPE_OBJECT:
            /**
            json_object_data obj = *(json_object_data *)val.data;
            vector_t *strvec = vector_new(sizeof(char *), 64);
            char **free_later = malloc(sizeof(char *) * obj.size * 2); // strings put here are freed later
            char *indented_str = putspcs("", indent);
            
            vector_push(strvec, &indented_str);
            vector_push(strvec, &lbrac);

            for (int i = 0; i < obj.size; i++){
                char *qkey = enquote(obj.data[i].key);
                char *val = json_tostring()
            }
            vector_push(strvec, &nl);
            vector_push(strvec, &indented_str);
            vector_push(strvec, &rbrac);
            

            ret = concat(strvec);

            free(indented_str);
            for (int i = 0; i < arr.size; i++){
                free(free_later[i]);
            }
            free(free_later);
            vector_free(strvec);
            break;
            */


    }
    recurse--;
    return ret;
}