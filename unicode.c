#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/* BKB 2010-04-06 11:33:55 */
/* Assumes that the buffer has at least four bytes. */

int ucs2_to_utf8 (int ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        return 1;
    }
    if (ucs2 >= 0x80  && ucs2 < 0x800) {
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        return 2;
    }
    if (ucs2 >= 0x800 && ucs2 < 0xFFFF) {
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        return 3;
    }
    return -1;
}

#ifdef TEST

void print_bytes (const unsigned char * bytes)
{
    int i;
    for (i = 0; i < strlen ((const char *) bytes); i++) {
        fprintf (stderr, "%02X", bytes[i]);
    }
    fprintf (stderr, "\n");
}


void test_ucs2_to_utf8 (const unsigned char * input)
{
    /* Buffer to print utf8 out into. */
    unsigned char buffer[0x100];
    /* Offset into buffer. */
    unsigned char * offset;
    const unsigned char * start = input;

    offset = buffer;
    while (1) {
        int unicode;
        int bytes;
        const unsigned char * end;
        unicode = utf8_to_ucs2 (start, & end);
        if (unicode == -1)
            break;
        start = end;
        bytes = ucs2_to_utf8 (unicode, offset);
        if (bytes == -1) {
            fprintf (stderr, "Failure\n");
            break;
        }
        offset += bytes;
#if 0
        printf ("%X %d\n", unicode, bytes);
#endif
    }
    * offset = '\0';
    if (strcmp ((const char *) buffer, (const char *) input) != 0) {
        fprintf (stderr, "Failure: input %s resulted in output %s\n",
                 input, buffer);
        print_bytes (input);
        print_bytes (buffer);
    } else {
        printf ("OK\n");
    }
}

int main ()
{
    const unsigned char * utf8 = (unsigned char *) "漢数字ÔÕÖＸ";
    const unsigned char * start = utf8;
    while (*start) {
        int unicode;
        const unsigned char * end;
        unicode = utf8_to_ucs2 (start, & end);
        if (unicode == -1 || unicode == 0)
            break;
        printf ("%s is %04X, length is %d\n", start, unicode, end - start);
        start = end;
    }
    test_ucs2_to_utf8 (utf8);
    return 0;
}

#endif /* def TEST */
