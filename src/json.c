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

static char *lbracket = "[";
static char *lbrace = "{";
static char *rbracket = "]";
static char *rbrace = "}";
static char *comma = ",";
static char *colon = ":";

char *json_tostring(json_value val){
    recurse++;
    if (recurse > JSON_RECURSE_MAX) {
        perror("Recurse maximum reached!");
        recurse--;
        return "null";
    }

    char *ret;
    switch (val.type){
    case JSON_TYPE_NULL:
        ret = "null";
        break;
    case JSON_TYPE_BOOLEAN:
        if (val.data == JSON_TRUE)
            ret = "true";
        else if (val.data == JSON_FALSE)
            ret = "false";
        else
            ret = "null";
        break;
    case JSON_TYPE_NUMBER:
        ret = calloc(sizeof(char), 16);
        sprintf(ret, "%lf", read_u64_as_d(val.data));
        break;
    case JSON_TYPE_STRING:
        ret = enquote((char *)val.data);
        break;
    case JSON_TYPE_ARRAY: {
        json_array_data arr = *(json_array_data *)val.data;

        vector_t *strvec = vector_new(sizeof(char *), 64);
        char **free_later = malloc(sizeof(char *) * arr.size); // strings put here are freed later
        
        vector_push(strvec, &lbracket);
        for (int i = 0; i < arr.size; i++){
            char *str = json_tostring(arr.data[i]);
            vector_push(strvec, &str);

            json_type type = arr.data[i].type;

            // booleans and nulls point to regular strings, are not heap allocated
            if (type != JSON_TYPE_BOOLEAN && type != JSON_TYPE_NULL)
                free_later[i] = str;
            else
                free_later[i] = NULL;

            if (i != arr.size - 1) vector_push(strvec, &comma);
        }
        vector_push(strvec, &rbracket);

        ret = concat(strvec);
        for (int i = 0; i < arr.size; i++){
            if (free_later[i] != NULL)
                free(free_later[i]);
        }
        free(free_later);
        vector_free(strvec);
        break;
    }
    case JSON_TYPE_OBJECT: {
        json_object_data obj = *(json_object_data *)val.data;

        vector_t *strvec = vector_new(sizeof(char *), 64);
        char **free_later = malloc(sizeof(char *) * obj.size * 2); // strings put here are freed later
        
        vector_push(strvec, &lbrace);
        for (int i = 0; i < obj.size; i++){
            char *key = enquote(obj.data[i].key);
            char *val = json_tostring(obj.data[i].val);
            vector_push(strvec, &key);
            vector_push(strvec, &colon);
            vector_push(strvec, &val);

            free_later[2*i] = key;

            json_type type = obj.data[i].val.type;

            // booleans and nulls point to regular strings, are not heap allocated
            if (type != JSON_TYPE_BOOLEAN && type != JSON_TYPE_NULL)
                free_later[2*i+1] = val;
            else
                free_later[2*i+1] = NULL;

            if (i != obj.size - 1) vector_push(strvec, &comma);
        }
        vector_push(strvec, &rbrace);

        ret = concat(strvec);
        for (int i = 0; i < obj.size * 2; i++){
            if (free_later[i] != NULL)
                free(free_later[i]);
        }
        free(free_later);
        vector_free(strvec);
        break;
    }
    }
    recurse--;
    return ret;
}