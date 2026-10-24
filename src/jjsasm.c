// Code by graddivcurl

#include "jjsasm.h"

/*

Helpers
    static uint64_t ll_read_u64(long long ll);
    static uint64_t d_read_u64(double d);
    static long long u64_read_ll(uint64_t u64);
    static double u64_read_d(uint64_t u64);
Conversion to jjsasm types
    size_t jjsasm_argc_of(jjsasm_instr_e instruction);
String conversion and prints
    char *jjsasm_instr_e_to_a(jjsasm_instr_e instr_e);
    char *jjsasm_type_e_to_a(jjsasm_type_e type_e);
    void jjsasm_print_arg_t(jjsasm_arg_t arg_t);
    void jjsasm_print_instr_t(jjsasm_instr_t instr_t);
Parsing strings
    jjsasm_instr_e jjsasm_a_to_instr_e(char *str);
    jjsasm_type_e jjsasm_parsenum(char *str);
    vector_t *jjsasm_tokenize(char *str);

*/

size_t jjsasm_argc_of(jjsasm_instr_e instr_e){
    if (instr_e >= jjsasm_argc_size) return 0;
    return jjsasm_argc[(int)instr_e];
}
const char *jjsasm_type_e_to_a(jjsasm_type_e type_e){
    if (type_e >= jjsasm_type_strs_size) return NULL;
    return jjsasm_type_strs[(int)type_e];
}
const char *jjsasm_instr_e_to_a(jjsasm_instr_e instr_e){
    if (instr_e >= jjsasm_instr_strs_size) return NULL;
    return jjsasm_instr_strs[(int)instr_e];
}
const char *jjsasm_token_e_to_a(jjsasm_token_e token_e){
    if (token_e >= jjsasm_token_strs_size) return NULL;
    return jjsasm_token_strs[(int)token_e];
}

void jjsasm_print_arg_t(jjsasm_arg_t arg_t){
    printf("(%s : ", jjsasm_type_e_to_a(arg_t.type_e));

    switch (arg_t.type_e){
        case JJSASM_TYPE_UNKNOWN:
            printf("%.16x", (uintptr_t)arg_t.data);
            break;
        case JJSASM_TYPE_INTEGER:
            printf("%ld", read_u64_as_z64(arg_t.data));
            break;
        case JJSASM_TYPE_DECIMAL:
            printf("%lf", read_u64_as_d(arg_t.data));
            break;
        case JJSASM_TYPE_STRING:
            printf("\"%s\"", (char *)arg_t.data);
            break;
        case JJSASM_TYPE_TAG:
            printf("%c%s", JJSASM_TAG_CHAR, (char *)arg_t.data);
            break;
        case JJSASM_TYPE_BAD:
            printf("%.16x", (uintptr_t)arg_t.data);
            break;
        default:
            printf("what the fuck?");
            break;
    }

    putchar(')');
}
void jjsasm_print_instr_t(jjsasm_instr_t instr_t){
    printf(jjsasm_instr_e_to_a(instr_t.instr_e));
    for (int i = 0; i < instr_t.argc; i++){
        putchar(' ');
        jjsasm_print_arg_t(instr_t.argv[i]);
    }
}
void jjsasm_print_token_t(jjsasm_token_t token_t){
    printf("[%s : ", jjsasm_token_e_to_a(token_t.token_e));

    switch (token_t.token_e){
        case JJSASM_TOKEN_UNKNOWN:
            printf("%.16x", (uintptr_t)token_t.data);
            break;
        case JJSASM_TOKEN_INSTRUCTION:
            jjsasm_print_instr_t(*(jjsasm_instr_t *)token_t.data);
            break;
        case JJSASM_TOKEN_COMMENT:
            printf((char *)token_t.data);
            break;
        case JJSASM_TOKEN_LABEL:
            printf((char *)token_t.data);
            break;
        case JJSASM_TOKEN_BAD:
            printf("%.16x", (uintptr_t)token_t.data);
            break;
    }

    putchar(']');
}
void jjsasm_print_token_vector(vector_t *vec){
    for (int i = 0; i < vec->size; i++){
        jjsasm_print_token_t(*(jjsasm_token_t *)vector_get(vec, i));
        putchar('\n');
    }
}

jjsasm_instr_e jjsasm_a_to_instr_e(char *str){
    if (strcmp(str, "NOP") == 0) return JJSASM_INSTR_NOP;
    if (strcmp(str, "MOV") == 0) return JJSASM_INSTR_MOV;
    if (strcmp(str, "ADD") == 0) return JJSASM_INSTR_ADD;
    if (strcmp(str, "SUB") == 0) return JJSASM_INSTR_SUB;
    if (strcmp(str, "CMP") == 0) return JJSASM_INSTR_CMP;
    if (strcmp(str, "JMP") == 0) return JJSASM_INSTR_JMP;
    if (strcmp(str, "JE") == 0)  return JJSASM_INSTR_JE;
    if (strcmp(str, "JL") == 0)  return JJSASM_INSTR_JL;
    if (strcmp(str, "JG") == 0)  return JJSASM_INSTR_JG;
    if (strcmp(str, "SKL") == 0) return JJSASM_INSTR_SKL;
    return JJSASM_INSTR_BAD;
}
jjsasm_instr_e jjsasm_a_to_instr_e_N(char *str){
    int ws = 0;
    while (strchr(JJSASM_STR_WHITESPACE, str[ws]) == NULL && str[ws] != '\0')
        ws++;

    // i know this looks like total shit but im tryna account for
    // NOP literally taking zero arguments and possibly ending in a null char before whitespace due to strtok
    // Honestly i have GOT to get rid of NOP this is being annoying asf
    if (strncmp(str, "NOP ", ws) == 0 && ws >= 3) return JJSASM_INSTR_NOP;
    if (strncmp(str, "MOV ", ws) == 0 && ws >= 3) return JJSASM_INSTR_MOV;
    if (strncmp(str, "ADD ", ws) == 0 && ws >= 3) return JJSASM_INSTR_ADD;
    if (strncmp(str, "SUB ", ws) == 0 && ws >= 3) return JJSASM_INSTR_SUB;
    if (strncmp(str, "CMP ", ws) == 0 && ws >= 3) return JJSASM_INSTR_CMP;
    if (strncmp(str, "JMP ", ws) == 0 && ws >= 3) return JJSASM_INSTR_JMP;
    if (strncmp(str, "JE ", ws) == 0  && ws >= 2) return JJSASM_INSTR_JE;
    if (strncmp(str, "JL ", ws) == 0  && ws >= 2) return JJSASM_INSTR_JL;
    if (strncmp(str, "JG ", ws) == 0  && ws >= 2) return JJSASM_INSTR_JG;
    if (strncmp(str, "SKL ", ws) == 0 && ws >= 3) return JJSASM_INSTR_SKL;
    return JJSASM_INSTR_BAD;
}
jjsasm_type_e jjsasm_parsenum(char *str){
    int i = 0;
    int dec_used = 0;

    while (strchr(JJSASM_STR_WHITESPACE, str[i]) == NULL){
        if (str[i] == '.'){
            if (dec_used == 1) return JJSASM_TYPE_UNKNOWN;
            dec_used = 1;
        }
        else if ('0' <= str[i] && str[i] <= '9'){
            // pass
        }
        else {
            return JJSASM_TYPE_UNKNOWN;
            // wont add exponent support, maybe if i build a higher-level compiler into JJSASM?
        }
        i++;
    }

    if (dec_used == 0)
        return JJSASM_TYPE_INTEGER;
    else
        return JJSASM_TYPE_DECIMAL;
}
vector_t *jjsasm_tokenize(char *str){
    if (str == NULL) {
        perror("Tokenize argument \"str\" expected valid address for a char *, got NULL");
        return NULL;
    } // note to self: strtok(NULL, {delim}) without 1st call is bad


    // Initialize char *str_token for tracking lines in const char *str using strtok
    char *str_token  = strtok(str, JJSASM_STR_LINEBREAK);
    // vec handles elements of type jjsasm_token_t
    vector_t *vec = vector_new(sizeof(jjsasm_token_t), 64);

    while (str_token != NULL){
        // Skip all whitespace at start of line early
        while (strchr(JJSASM_STR_WHITESPACE, *str_token) != NULL)
            str_token++;

        // Declare our token for later!!!
        jjsasm_token_t token_t;
        size_t len = strlen(str_token); // literally used like once lmao

        if (str_token[0] == '/' && str_token[1] == '/'){
            token_t.token_e = JJSASM_TOKEN_COMMENT;
            token_t.data = &str_token[2];
            
            goto NEXT_TOKEN;
        }
        else if (str_token[len - 1] == ':'){
            token_t.token_e = JJSASM_TOKEN_LABEL;
            token_t.data = str_token;

            str_token[len - 1] = '\0'; // ok maybe its used twice
            goto NEXT_TOKEN;
        }

        // From here on out, all logic in this token loop is for handling instruction parsing
        // Comments and labels will have used goto NEXT_TOKEN to jump to the end of this loop

        jjsasm_instr_e instr_e = jjsasm_a_to_instr_e_N(str_token);
        if (instr_e == JJSASM_INSTR_BAD){
            perror("Tokenize encountered unknown/BAD instruction");
            goto TOKENIZE_FAILURE;
        }

        // Initialize jjsasm_arg_t *argv and add argc # of arguments to argv
        size_t argc = jjsasm_argc_of(instr_e);
        jjsasm_arg_t *argv;
        if (argc != 0)
            argv = malloc(sizeof(jjsasm_arg_t) * argc);
        else
            argv = NULL;
        
        // Iterate through each expected argument
        for (int i = 0; i < argc; i++){
            // Find next argument
            while (strchr(JJSASM_STR_WHITESPACE, *str_token) == NULL){
                str_token++;
                // If a null is encountered, it probably expected more arguments
                if (*str_token == '\0'){
                    perror("Tokenize encountered null terminator while parsing instruction arguments; Not enough arguments?");
                    goto TOKENIZE_FAILURE;
                }
            }
            str_token++;

            // Parse string of argument as a jjsasm_type_e with accompanying data
            jjsasm_type_e type_e = JJSASM_TYPE_UNKNOWN;
            uint64_t data;
            size_t end = strlen(str_token)-1; // Index of last char in arg

            int quote_i = 1; // if a string is found, quote_i will refer to the end quotation mark for proper string storage
            if (str_token[0] == JJSASM_TAG_CHAR){
                if (str_token[1] == '\0'){
                    perror("Tokenize encountered malformed tag");
                    goto TOKENIZE_FAILURE;
                }
                type_e = JJSASM_TYPE_TAG;
            }
            else if (str_token[0] == '\"'){
                // See int quote_i above
                while (str_token[quote_i] != '\"'){
                    if (str_token[quote_i] == '\0'){
                        perror("Tokenize encountered malformed string");
                        goto TOKENIZE_FAILURE;
                    }
                    quote_i++;
                }

                type_e = JJSASM_TYPE_STRING;
                // Remember to add handling for string quotation marks
            }

            if (type_e == JJSASM_TYPE_TAG)
                data = (uintptr_t)(str_token + sizeof(char));
            else if (type_e == JJSASM_TYPE_STRING){
                char *curr_str = malloc(sizeof(char) * (quote_i + 1));
                strncpy(curr_str, str_token + sizeof(char), quote_i);
                curr_str[quote_i-1] = '\0';
                data = (uintptr_t)curr_str;
            }
            else {
                type_e = jjsasm_parsenum(str_token);
                switch (type_e) {
                    case JJSASM_TYPE_INTEGER:
                        data = read_z64_as_u64(atoll(str_token));
                        break;
                    case JJSASM_TYPE_DECIMAL:
                        data = read_d_as_u64(atof(str_token));
                        break;
                    default:
                        perror("Tokenize encountered unknown argument while parsing");
                        goto TOKENIZE_FAILURE;
                }
            }

            // Add type and data to the ith argument

            argv[i].type_e = type_e;
            argv[i].data = data;
        }

        // Build instruction and its respective token

        jjsasm_instr_t *instr_t = malloc(sizeof(jjsasm_instr_t));
        instr_t->instr_e = instr_e;
        instr_t->argc = argc;
        instr_t->argv = argv;

        token_t.token_e = JJSASM_TOKEN_INSTRUCTION;
        token_t.data = (void *)instr_t;

        // May have jumped to here from handling of
        // comments and labels above the instruction stuff
        NEXT_TOKEN: // goto
        vector_push(vec, (void *)&token_t);
        str_token = strtok(NULL, JJSASM_STR_LINEBREAK);
    }

    return vec;

    TOKENIZE_FAILURE: // goto
    jjsasm_print_token_vector(vec);
    for (int i = 0; i < vec->size; i++){
        jjsasm_token_t *token_t = (jjsasm_token_t *)vector_get(vec, i);
        if (token_t->token_e == JJSASM_TOKEN_INSTRUCTION){
            jjsasm_arg_t *argv = ((jjsasm_instr_t *)token_t->data)->argv;
            if (argv != NULL) free(argv);
        }
    }
    vector_free(vec);
    return NULL;
}
// end