/* ReadCfg example usage
 * MCD 2014-01-31
 */

#include <stdio.h>
#include <stdlib.h>

#include "readcfg.h"

void callback(char*k, char*v){
    printf("%s = \"%s\"\n", k, v);
}

int main(int argc, char *argv[])
{
    // Initialise the cfg structure (this sets up a linked list
    // of key / value pairs)
    struct Config *cfg = cfg_init("settings.cfg");

    // Search for a key. Note that the return value is a pointer
    // in the cfg structure so should NOT be freed (it is freed
    // eventually by cfg_free.);
    char *value = cfg_get_string(cfg, "barney", "The default value");
    printf("\"%s\"\n", value);

    // Search for a non-existent key to show default value being returned
    char *value2 = cfg_get_string(cfg, "non-existent", "A default value");
    printf("\"%s\"\n", value2);

    // Get a number
    long num = cfg_get_long(cfg, "NuMbEr", 69);
    printf("Number is %ld\n", num);

    // Dump everything - showing the cfg_foreach() function
    printf("DEBUG dump all:\n");
    cfg_foreach(cfg, callback);

    // Finally tear down the cfg structure. This will invalidate any 
    // strings we've been returned, so we should make copies of 
    // anything we want to keep first.
    cfg_free(cfg);

    return 0;
}
