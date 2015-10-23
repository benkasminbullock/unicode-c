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
        len = strlen ((const char *) utf8);
    }
    for (p = utf8; p - utf8 < len; ) {
        int ucs2;
        
        ucs2 = utf8_to_ucs2 (p, & p);
        if (ucs2 < 0) {
	    /* The input is incorrectly formatted in some way. */
	    return 0;
        }
        if (! ucs2_is_kana (ucs2)) {
            return 0;
        }
    }
    return 1;
}

int utf16_is_kanji (int utf16)
{
    if (utf16 < 0x4e00 || utf16 > 0x9fff) {
	return 0;
    }
    return 1;
}

#ifdef TEST

#define OK(test, counter, message, ...) {	\
	counter++;				\
	if (test) {				\
	    printf ("ok %d - ", counter);	\
	}					\
	else {					\
	    printf ("not ok %d - ", counter);	\
	}					\
	printf (message, ## __VA_ARGS__);	\
	printf (".\n");				\
    }

int main ()
{
    int counter = 0;
    int i;
    int expect[] = {1, 0, 0};
    char * test[] = {"ひらがなカタカナスゲー",
		     "かな This is not all kana ",
		     "漢字"};
    for (i = 0; i < sizeof (test) / sizeof (unsigned char *); i++) {
	OK (utf8_is_kana_chars ((unsigned char *) test[i], 0) == expect[i],
	    counter, "%s %d", test[i], expect[i]);
    }
    printf ("1..%d\n", counter);
    return 0;
}

#endif
