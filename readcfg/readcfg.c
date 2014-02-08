#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readcfg.h"
#include "fgets_dynamic.h"

// Static helpers' prototypes
static char *trimwhitespace(char *str);
static void split(char *buf, char**key, char**val);

/* -------------------------------------------------------------------------- */

/* Open a config file. Populates a linked
 * list of cfg_records.
 */
struct Config *
cfg_init(char* filename)
{
    size_t buf_len=256;
    char *buf = malloc(buf_len);
    if (!buf) return NULL;
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        return NULL;
    }
    struct Config *first_record = NULL;
    struct Config *previous_record = NULL;
    while (!feof(fp)) {
        if (fgets_dynamic(&buf, &buf_len, fp) == NULL) {
            if (feof(fp)) break;
            // out of memory on a realloc:
            return NULL;
        }
        char *trimmed_substring = trimwhitespace(buf);
        if (*trimmed_substring == '\0' || *trimmed_substring == '#') {
            // empty line or commented out
            continue;
        }
        char *key = NULL;
        char *value = NULL;
        split(buf, &key, &value);
        if (key == NULL) {
            // a blank, or commented line so ignore
            continue;
        }
        struct Config *new_record = malloc(sizeof(struct Config));
        if (!new_record) return NULL;
        if (first_record == NULL) {
            first_record = new_record;
        }
        new_record->key = malloc(strlen(key) + 1);
        if (!new_record->key) return NULL;
        strcpy(new_record->key, key);
        new_record->value = malloc(strlen(value) + 1);
        if (!new_record->value) return NULL;
        strcpy(new_record->value, value);
        new_record->next = NULL;
        if (previous_record != NULL) {
            previous_record->next = new_record;
        }
        previous_record = new_record;
    }
    free(buf);
    return first_record;
}

/* -------------------------------------------------------------------------- */

// Get the value of a key from the structure:
// (caller must NOT free the result as it's a pointer into the linked list)
char *
cfg_get_string(struct Config *cfg, char *key, char *default_value)
{
    if (cfg == NULL) {
        return NULL;
    }
    char *upper_key = strdup(key);
    unsigned char *t = (unsigned char *)upper_key;
    while (*t) {
        *t = toupper(*t);
        t++;
    }

    struct Config *current_record = cfg;

    char *rc = NULL;
    do {
        if (strcmp(current_record->key, upper_key) == 0) {
            rc = current_record->value;
            break;
        }
        if (current_record->next == NULL) {
            break;
        }
        current_record = current_record->next;
    } while (1);
    free(upper_key);
    if (rc == NULL) {
        return default_value;
    } else {
        return rc;
    }
}

/* -------------------------------------------------------------------------- */

// Get a long from the config structure
// Note all errors result in return of the default value
// until I implement proper error handling
long 
cfg_get_long(struct Config *cfg, char *key, int default_value)
{
    if (cfg == NULL) {
        return default_value;
    }
    char *value = cfg_get_string(cfg, key, "");
    if (!value) {
        return default_value;
    }
    return strtol(value, NULL, 0); 
}

/* -------------------------------------------------------------------------- */

// Free all memory allocated
void 
cfg_free(struct Config *cfg)
{
    if (cfg == NULL) return;
    struct Config *next = NULL;
    do {
        free(cfg->key);
        free(cfg->value);
        next = cfg->next;
        free(cfg);
        if (next == NULL) break;
        cfg = next;
    } while(1);
}


/* -------------------------------------------------------------------------- */

/*
 * Helper Functions
 */

/*
 * Note: This function returns a pointer to a SUBSTRING of the original string.
 * If the given string was allocated dynamically, the caller must not overwrite
 * that pointer with the returned value, since the original pointer must be
 * deallocated using the same allocator with which it was allocated.  The return
 * value must NOT be deallocated using free() etc.
 */
static char *trimwhitespace(char *str)
{
    char *end;
    // Trim leading space
    while(isspace((unsigned char) *str)) {
        str++;
    }
    if(*str == 0) {
        return str;
    }
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char) *end)) {
        end--;
    }
    // Write new null terminator
    *(end+1) = 0;
    return str;
}

/* -------------------------------------------------------------------------- */

static void split(char *buf, char**key, char**val)
{
    char *p = strchr(buf, '=');
    if (p == NULL) {
        return;
    }
    *p = '\0';
    *key = trimwhitespace(buf);
    char *tmp=*key;
    // Upper case the key
    while(*tmp){
        *tmp = toupper((unsigned char) *tmp);
        tmp++;
    }
    p++;
    *val = trimwhitespace(p);
}

/* -------------------------------------------------------------------------- */

void cfg_foreach(struct Config *cfg, void (*callback)(char *key, char *value))
{
    if (cfg == NULL) {
        return;
    }
    struct Config *t = cfg;
    while (t != NULL) {
        callback(t->key, t->value);
        t = t->next;
    }
}

/* -------------------------------------------------------------------------- */
