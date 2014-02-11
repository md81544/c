#ifndef MGOSTRING_H
#define MGOSTRING_H

#include <stdbool.h>

/* A simple "string" implementation - with automatically expanding buffer.
 * Note that the "constructor" allows you to specify how much extra space
 * to allocate initially - this is just to avoid unnecessary reallocs if 
 * you know you're going to be adding lots to the string. Otherwise it's 
 * fine to specify zero and let it realloc automatically.
 */


struct MgoString {
    char *data;
    unsigned long length;
};

struct MgoString *mgostring_construct(const char *starter, unsigned long extra_space);
bool   mgostring_add(struct MgoString *str, const char *new);
char  *mgostring_c_str(struct MgoString *str);
void   mgostring_rtrim(struct MgoString *str);
void   mgostring_ltrim(struct MgoString *str);
void   mgostring_trim(struct MgoString *str);
int    mgostring_replace(struct MgoString *str, const char *from, const char *to);
void   mgostring_destruct(struct MgoString *str);

#endif
