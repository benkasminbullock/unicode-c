#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "unicode.h"

/* Return a true or false value depending on whether ucs is in the
   range of kana. */

int ucs2_is_kana (int ucs)
{
    if (ucs >= 0x3000 && ucs <= 0x3100) {
        return 1;
    }
    return 0;
}

/* Are all the characters in "utf8" kana characters? */

int utf8_is_kana_chars (const unsigned char * utf8, int len)
{
    const unsigned char * p;
    if (len == 0) {
        len = strlen (utf8);
    }
    for (p = utf8; p - utf8 < len; ) {
        int ucs2;
        
        ucs2 = utf8_to_ucs2 (p, & p);
        if (ucs2 == -1) {
            fprintf (stderr, "Byte %d of %s is not UTF-8\n", p - utf8,
                     utf8);
            exit (1);
        }
        if (! ucs2_is_kana (ucs2)) {
            return 0;
        }
    }
    return 1;
}

#ifdef TEST

int main ()
{
    int i;
    unsigned char * test[] = {"ひらがなカタカナスゲー",
                              "かな This is not all kana ",
                              "漢字"};
    for (i = 0; i < sizeof (test) / sizeof (unsigned char *); i++) {
        printf ("%s: %d\n", test[i], utf8_is_kana_chars (test[i], 0));
    }
}

#endif
