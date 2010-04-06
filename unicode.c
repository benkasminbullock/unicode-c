#include <stdio.h>
#include <stdlib.h>
#include "unicode.h"

/* Convert a UTF-8 encoded character in "input" into a number. This
   function returns the unicode value of the UTF-8 character if
   successful, and -1 if not successful. "end_ptr" is set to the next
   character after the read character on success. or the start of
   input on failure. "end_ptr" may not be null. */

int utf8_to_ucs2 (const unsigned char * input, const unsigned char ** end_ptr)
{
    *end_ptr = input;
    if (input[0] == 0)
        return -1;
    if (input[0] < 0x80) {
        * end_ptr = input + 1;
        return input[0];
    }
    if ((input[0] & 0xE0) == 0xE0) {
        if (input[1] == 0 || input[2] == 0)
            return -1;
        * end_ptr = input + 3;
        return
            (input[0] & 0x0F)<<12 |
            (input[1] & 0x3F)<<6  |
            (input[2] & 0x3F);
    }
    if ((input[0] & 0xC0) == 0xC0) {
        if (input[1] == 0)
            return -1;
        * end_ptr = input + 2;
        return
            (input[0] & 0x1F)<<6  |
            (input[1] & 0x3F);
    }
    /*
    fprintf (stderr, "Badly-formed UTF-8 byte %X\n", input[0]);
    */
    return -1;
}

#ifdef TEST

int main ()
{
    const char * utf8 = "漢数字ÔÕÖＸ";
    const unsigned char * start = (const unsigned char *) utf8;
    while (1) {
        int unicode;
        const unsigned char * end;
        unicode = utf8_to_ucs2 (start, & end);
        if (unicode == -1)
            break;
        printf ("%s is %04X, length is %d\n", start, unicode, end - start);
        start = end;
    }
    return 0;
}

#endif /* def TEST */
