#ifndef READCFG_H
#define READCFG_h

/* Notes:
 * All keys are stored upper case so searching is case insensitive.
 * All keys AND data have whitespace trimmed from either end
 */

struct Config {
    char *key;
    char *value;
    struct Config *next;
};

// Open a config file. Populates a linked list 
// of Config items. Returns a pointer to the root node.
struct Config *cfg_init(char* filename);

// Get the value of a key from the structure:
// (Caller must NOT free the result - it points into the structure)
char *cfg_get_string(struct Config *cfg, char *key, char *dflt);

// Get a long integer value from the structure. 
long cfg_get_long(struct Config *cfg, char *key, int default_value);

// Free all memory associated
void cfg_free(struct Config *cfg);

// Dump all keys (for debug)
void cfg_foreach(struct Config *cfg, void (*callback)(char *key, char *value));

#endif
