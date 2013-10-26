#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unicode.h"

#ifdef HEADER
#define UTF8_MAX_LENGTH 4
#endif /* def HEADER */

/* Convert a UTF-8 encoded character in "input" into a number. This
   function returns the unicode value of the UTF-8 character if
   successful, and -1 if not successful. "end_ptr" is set to the next
   character after the read character on success. "end_ptr" is set to
   the start of input on failure. "end_ptr" may not be null. */

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
    return -1;
}

    /*
    fprintf (stderr, "Badly-formed UTF-8 byte %X\n", input[0]);
    */
/* BKB 2010-04-06 11:33:55 */

/* Input: a Unicode code point, "ucs2". 

   Output: UTF-8 characters in buffer "utf8". 

   Return value: the number of bytes written into "utf8", or -1 if
   there was an error.

   This adds a zero byte to the end of the string. It assumes that the
   buffer "utf8" has at least four bytes of space to write to. */

int ucs2_to_utf8 (int ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        utf8[1] = '\0';
        return 1;
    }
    if (ucs2 >= 0x80  && ucs2 < 0x800) {
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        utf8[2] = '\0';
        return 2;
    }
    if (ucs2 >= 0x800 && ucs2 < 0xFFFF) {
	if (ucs2 >= 0xD800 && ucs2 <= 0xDFFF) {
	    /* Ill-formed. */
	    return -1;
	}
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        utf8[3] = '\0';
        return 3;
    }
    return -1;
}

/* Given a count of Unicode characters "n_chars", return the number of
   bytes. */

int unicode_chars_to_bytes (const unsigned char * utf8, int n_chars)
{
    int i;
    const unsigned char * p = utf8;
    int len = strlen ((const char *) utf8);
    if (len == 0 && n_chars != 0) {
        fprintf (stderr, "Error: request for n_chars of empty string.\n");
        exit (1);
    }
    for (i = 0; i < n_chars; i++) {
        int ucs2 = utf8_to_ucs2 (p, & p);
        if (ucs2 == -1) {
            fprintf (stderr, "Error: not UTF-8.\n");
            exit (1);
        }
    }
    return p - utf8;
}

int unicode_count_chars (const unsigned char * utf8)
{
    int chars = 0;
    const unsigned char * p = utf8;
    int len = strlen ((const char *) utf8);
    if (len == 0) {
        return 0;
    }
    while (p - utf8 < len) {
        int ucs2;
        ucs2 = utf8_to_ucs2 (p, & p);
        if (ucs2 == -1) {
            return -1;
        }
        chars++;
        if (*p == '\0') {
            return chars;
        }
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
    printf ("%d = 7?\n", unicode_count_chars (utf8));
    return 0;
}

#endif /* def TEST */
