#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "c-tap-test.h"
#include "unicode.h"

#include "utf-8-test-data.c"

static void
utf_8_test_1 ()
{
    TAP_TEST (strcmp ((const char *) kuhn_1, "κόσμε") == 0);
}

static void
utf8_to_ucs2_expect (const char * in, int expect)
{
    const uint8_t * end;

    TAP_TEST_EQUAL (utf8_to_ucs2 ((uint8_t *) in, & end), expect);
    if (expect > 0) {
	TAP_TEST (*end == '\0');
    }
}

static void
utf_8_test_2 ()
{
    /* 2.1 */

    /* We don't allow nul bytes in our UTF-8 sequence. */
    utf8_to_ucs2_expect (kuhn_2_1_1, UNICODE_EMPTY_INPUT);
    utf8_to_ucs2_expect (kuhn_2_1_2, expect_2_1_2);
    utf8_to_ucs2_expect (kuhn_2_1_3, expect_2_1_3);
    utf8_to_ucs2_expect (kuhn_2_1_4, expect_2_1_4);
    /* We don't allow five or six byte sequences. */
    utf8_to_ucs2_expect (kuhn_2_1_5, UTF8_BAD_LEADING_BYTE);
    utf8_to_ucs2_expect (kuhn_2_1_6, UTF8_BAD_LEADING_BYTE);

    /* 2.2 */

    utf8_to_ucs2_expect (kuhn_2_2_1, expect_2_2_1);
    utf8_to_ucs2_expect (kuhn_2_2_2, expect_2_2_2);
    utf8_to_ucs2_expect (kuhn_2_2_3, expect_2_2_3);
    /* For input, we only go up to UNICODE_MAXIMUM, so the leading
       bytes in the following three tests can't occur in what we allow
       for. */
    utf8_to_ucs2_expect (kuhn_2_2_4, UTF8_BAD_LEADING_BYTE);
    utf8_to_ucs2_expect (kuhn_2_2_5, UTF8_BAD_LEADING_BYTE);
    utf8_to_ucs2_expect (kuhn_2_2_6, UTF8_BAD_LEADING_BYTE);

    /* 2.3 */

    utf8_to_ucs2_expect (kuhn_2_3_1, expect_2_3_1);
    utf8_to_ucs2_expect (kuhn_2_3_2, expect_2_3_2);
    utf8_to_ucs2_expect (kuhn_2_3_3, expect_2_3_3);
    /* This tests the boundary, but the final two bytes are 0xFF 0xFF,
       so it's not a valid character. */
    utf8_to_ucs2_expect (kuhn_2_3_4, UNICODE_NOT_CHARACTER);
    /* This is one more than the maximum allowed. */
    utf8_to_ucs2_expect (kuhn_2_3_5, UNICODE_TOO_BIG);
}

static void
allbadfirst (const char * in, int len)
{
    int i;
    for (i = 0; i < len; i++) {
	utf8_to_ucs2_expect (in+i, UTF8_BAD_LEADING_BYTE);
    }
}

static void
utf_8_test_3 ()
{
    /* All of these are just bad leading bytes stuck together. */

    allbadfirst (kuhn_3_1_1, 1);
    allbadfirst (kuhn_3_1_2, 1);
    allbadfirst (kuhn_3_1_3, 2);
    allbadfirst (kuhn_3_1_4, 3);
    allbadfirst (kuhn_3_1_5, 4);
    allbadfirst (kuhn_3_1_6, 5);
    allbadfirst (kuhn_3_1_7, 6);
    allbadfirst (kuhn_3_1_8, 7);

}

int main ()
{
    utf_8_test_1 ();
    utf_8_test_2 ();
    utf_8_test_3 ();
    TAP_PLAN;
    return 0;
}
