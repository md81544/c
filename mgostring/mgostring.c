#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "mgostring.h"

/*---------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------*/

bool mgostring_add(struct MgoString *str, const char *new){
    unsigned long new_len = strlen(str->data) + strlen(new);
    if (new_len > str->length) {
        char *new_data = realloc(str->data, new_len + 1);
        if (new_data == NULL) {
            return false;
        }
        str->data = new_data;
        str->length = new_len;
        #ifdef DEBUG
            printf("*** realloc, str->length is now %lu ***\n", str->length);
        #endif
    }
    strcat(str->data, new);
    return true;
}

/*---------------------------------------------------------------------------*/

char *mgostring_c_str(struct MgoString *str){
    return str->data;
}

/*---------------------------------------------------------------------------*/

void mgostring_rtrim(struct MgoString *str){
    char *non_space = str->data + (strlen(str->data) - 1);
    while (isspace(*non_space)) {
        *non_space = '\0';
        non_space--;
    }
}

/*---------------------------------------------------------------------------*/

void mgostring_ltrim(struct MgoString *str){
    char *non_space = str->data;
    unsigned long len = strlen(str->data);
    while (isspace(*non_space)) non_space++;
    memmove(str->data, non_space, len + 1);
}

/*---------------------------------------------------------------------------*/

void mgostring_trim(struct MgoString *str){
    mgostring_ltrim(str);
    mgostring_rtrim(str);
}

/*---------------------------------------------------------------------------*/

int mgostring_replace(struct MgoString *str, const char *from, const char *to) {
    // Returns the number of replacements made
    //
    // early out if strstr returns NULL:
    if (strstr(str->data, from) == NULL) return 0;
    char *copy = strdup(str->data);
    if (copy == NULL) return 0;
    int num_changes = 0;
    strcpy(str->data,"");
    char *copy_from = copy;
    char *up_to = NULL;
    do {
        up_to = strstr(copy_from, from);
        if (up_to == NULL) break;
        num_changes++;
        *up_to = '\0';
        mgostring_add(str, copy_from);
        mgostring_add(str, to);
        copy_from = up_to + strlen(from);
    } while(true);
    mgostring_add(str, copy_from);
    free(copy);
    return num_changes;
}

/*---------------------------------------------------------------------------*/

void mgostring_destruct(struct MgoString *str){
    free(str->data);
    free(str);
}

/*---------------------------------------------------------------------------*/
