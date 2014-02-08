#ifndef MGOSTRING_H
#define MGOSTRING_H

/* A simple "string" implementation - with automatically expanding buffer.
 * Note that the "constructor" allows you to specify how much extra space
 * to allocate initially - this is just to avoid unnecessary reallocs if 
 * you know you're going to be adding lots to the string. Otherwise it's 
 * fine to specify zero and let it realloc automatically
 */

/* requires the following includes (avoided here to avoid dependencies)
 * stdlib.h
 * stdbool.h
 * string.h
 * ctype.h
 */

/* TODO:
 * replace
 */

struct MgoString {
    char *data;
    unsigned long length;
};

struct MgoString *mgostring_construct(const char *starter, unsigned long extra_space) {
    struct MgoString *rc = malloc(sizeof(struct MgoString));
    if (rc==NULL) return NULL;
    unsigned long initial_len = strlen(starter) + extra_space;
    rc->data = malloc(initial_len + 1);
    if (rc->data==NULL) {
        free(rc);
        return NULL;
    }
    strcpy(rc->data, starter);
    rc->length = initial_len;
    return rc;
}

bool mgostring_add(struct MgoString *str, const char *new){
    unsigned long new_len = strlen(str->data) + strlen(new);
    if (new_len > str->length) {
        #ifdef DEBUG
            printf("*** realloc ***\n");
        #endif
        char *new_data = realloc(str->data, new_len + 1);
        if (new_data == NULL) {
            return false;
        }
        str->data = new_data;
    }
    strcat(str->data, new);
    return true;
}

char *mgostring_c_str(struct MgoString *str){
    return str->data;
}

void mgostring_rtrim(struct MgoString *str){
    char *non_space = str->data + (strlen(str->data) - 1);
    while (isspace(*non_space)) {
        *non_space = '\0';
        non_space--;
    }
}

void mgostring_ltrim(struct MgoString *str){
    char *non_space = str->data;
    unsigned long len = strlen(str->data);
    while (isspace(*non_space)) non_space++;
    memmove(str->data, non_space, len + 1);
}

void mgostring_trim(struct MgoString *str){
    mgostring_ltrim(str);
    mgostring_rtrim(str);
}

void mgostring_destruct(struct MgoString *str){
    free(str->data);
    free(str);
}


#endif
