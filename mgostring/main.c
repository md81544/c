/* Testing */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "mgostring.h"


int main(int argc, char *argv[])
{
    struct MgoString *s;
    s = mgostring_construct("    Hello", 0);
    mgostring_add(s, " test");
    mgostring_add(s, ", ");
    mgostring_add(s, "the cat sat on the mat. All your base are belong to us.         ");
    mgostring_trim(s);
    printf("\"%s\"\n", mgostring_c_str(s));
    mgostring_destruct(s);
}

