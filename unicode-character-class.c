#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "unicode.h"
#include "unicode-character-class.h"

/* Return a true or false value depending on whether ucs is in the
   range of kana. */

int32_t ucs2_is_kana (int32_t ucs)
{
    if (ucs >= 0x3000 && ucs <= 0x3100) {
        return 1;
    }
    return 0;
}

/* Are all the characters in "utf8" kana characters? */

int32_t utf8_is_kana_chars (const uint8_t * utf8, int32_t len)
{
    const uint8_t * p;
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

int32_t utf16_is_kanji (int32_t utf16)
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
    for (i = 0; i < sizeof (test) / sizeof (uint8_t *); i++) {
	OK (utf8_is_kana_chars ((const uint8_t *) test[i], 0) == expect[i],
	    counter, "%s %d", test[i], expect[i]);
    }
    OK (utf16_is_kanji (0x6050), counter, "OK");
    OK (! utf16_is_kanji (0x60), counter, "OK");
    printf ("1..%d\n", counter);
    return 0;
}

#endif /* TEST */
