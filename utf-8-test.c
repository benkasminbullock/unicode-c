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
    TAP_TEST (valid_utf8 ((const uint8_t *) kuhn_1, strlen (kuhn_1)));
}

static void
utf8_to_ucs2_expect (const char * in, int expect)
{
    const uint8_t * end;
    int32_t valid;
    size_t len;

    TAP_TEST_EQUAL (utf8_to_ucs2 ((uint8_t *) in, & end), expect);

    len = strlen (in);
    if (len > 0) {
	valid = valid_utf8 ((const unsigned char *) in, len);
	if (expect > 0) {
	    TAP_TEST (valid == UTF8_VALID);
	}
	else {
	    TAP_TEST (valid == UTF8_INVALID);
	}
    }
    if (expect > 0) {
	/* If the call succeeds, end is pointing after the UTF-8
	   "rune", which is the end of the string in all of our cases,
	   so check that was done correctly. */
	TAP_TEST (*end == '\0');
    }
    else {
	int error;
	/* Check the documentation promise that end is the same as in
	   if the call fails. */
	TAP_TEST (end = (uint8_t *) in);
	if (len > 0) {
	    utf8_info_t info;
	    error = validate_utf8 ((const unsigned char *) in, len, & info);
	    TAP_TEST_EQUAL (error, expect);
	}
    }
}

#define EXPECT(x, y)						\
    printf ("# Testing %s for value %s (%d).\n", #x, #y, y);	\
    utf8_to_ucs2_expect (x, y);

static void
utf_8_test_2 ()
{
    /* 2.1 */

    /* We don't allow nul bytes in our UTF-8 sequence. */
    EXPECT (kuhn_2_1_1, UNICODE_EMPTY_INPUT);
    EXPECT (kuhn_2_1_2, expect_2_1_2);
    EXPECT (kuhn_2_1_3, expect_2_1_3);
    EXPECT (kuhn_2_1_4, expect_2_1_4);
    /* We don't allow five or six byte sequences. */
    EXPECT (kuhn_2_1_5, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_2_1_6, UTF8_BAD_LEADING_BYTE);

    /* 2.2 */

    EXPECT (kuhn_2_2_1, expect_2_2_1);
    EXPECT (kuhn_2_2_2, expect_2_2_2);
    /* Something ending in FFFF is not valid. */
    EXPECT (kuhn_2_2_3, UNICODE_NOT_CHARACTER);
    /* For input, we only go up to UNICODE_MAXIMUM, so the leading
       bytes in the following three tests can't occur in what we allow
       for. */
    EXPECT (kuhn_2_2_4, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_2_2_5, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_2_2_6, UTF8_BAD_LEADING_BYTE);

    /* 2.3 */

    EXPECT (kuhn_2_3_1, expect_2_3_1);
    EXPECT (kuhn_2_3_2, expect_2_3_2);
    EXPECT (kuhn_2_3_3, expect_2_3_3);
    /* This tests the boundary, but the final two bytes are 0xFF 0xFF,
       so it's not a valid character. */
    EXPECT (kuhn_2_3_4, UNICODE_NOT_CHARACTER);
    /* This is one more than the maximum allowed. */
    EXPECT (kuhn_2_3_5, UNICODE_TOO_BIG);
}

static void
allbadfirst (const char * in, int len)
{
    int i;
    for (i = 0; i < len; i++) {
//	printf ("%s:%d: OK %X\n", __FILE__, __LINE__, (unsigned char) in[i]);

	EXPECT (in+i, UTF8_BAD_LEADING_BYTE);
    }
}


static void
utf_8_test_3_1 ()
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

/* Tests for bad continuation bytes. Some of these fail to be valid
   for our Unicode-only purposes anyway. */

static void
utf_8_test_3_3 ()
{
    /* We don't handle 0xC0... cases. */
    EXPECT (kuhn_3_3_1, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_3_3_2, UTF8_BAD_CONTINUATION_BYTE);
    EXPECT (kuhn_3_3_3, UTF8_BAD_CONTINUATION_BYTE);
    /* We don't handle these long cases. */
    EXPECT (kuhn_3_3_4, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_3_3_5, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_3_3_6, UTF8_BAD_CONTINUATION_BYTE);
    EXPECT (kuhn_3_3_7, UTF8_BAD_CONTINUATION_BYTE);
    EXPECT (kuhn_3_3_8, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_3_3_9, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_3_3_10, UTF8_BAD_LEADING_BYTE);
}

static void
utf_8_test_3_5 ()
{
    EXPECT (kuhn_3_5_1, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_3_5_2, UTF8_BAD_LEADING_BYTE);
    allbadfirst (kuhn_3_5_3, 4);
}

static void
utf_8_test_3 ()
{
    utf_8_test_3_1 ();
    utf_8_test_3_3 ();
    utf_8_test_3_5 ();
}

/* Non-shortest sequences. */

static void
utf_8_test_4_1 ()
{
    /* We don't allow the leading byte 0xC0 anyway. */
    EXPECT (kuhn_4_1_1, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_4_1_2, UTF8_BAD_CONTINUATION_BYTE);
    EXPECT (kuhn_4_1_3, UTF8_BAD_CONTINUATION_BYTE);
    /* These are five and six bytes so they are rejected at the first
       byte. */
    EXPECT (kuhn_4_1_4, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_4_1_5, UTF8_BAD_LEADING_BYTE);
}

static void
utf_8_test_4_2 ()
{
    /* We don't allow the leading byte 0xC1 anyway. */
    EXPECT (kuhn_4_2_1, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_4_2_2, UTF8_BAD_CONTINUATION_BYTE);
    EXPECT (kuhn_4_2_3, UTF8_BAD_CONTINUATION_BYTE);
    /* These are five and six bytes so they are rejected at the first
       byte. */
    EXPECT (kuhn_4_2_4, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_4_2_5, UTF8_BAD_LEADING_BYTE);
}

static void
utf_8_test_4_3 ()
{
    /* We don't allow the leading byte 0xC0 anyway. */
    EXPECT (kuhn_4_3_1, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_4_3_2, UTF8_BAD_CONTINUATION_BYTE);
    EXPECT (kuhn_4_3_3, UTF8_BAD_CONTINUATION_BYTE);
    /* These are five and six bytes so they are rejected at the first
       byte. */
    EXPECT (kuhn_4_3_4, UTF8_BAD_LEADING_BYTE);
    EXPECT (kuhn_4_3_5, UTF8_BAD_LEADING_BYTE);
}

static void
utf_8_test_4 ()
{
    utf_8_test_4_1 ();
    utf_8_test_4_2 ();
    utf_8_test_4_3 ();
}

static void
utf_8_test_5_1 ()
{
    EXPECT (kuhn_5_1_1, UNICODE_SURROGATE_PAIR);
    EXPECT (kuhn_5_1_2, UNICODE_SURROGATE_PAIR);
    EXPECT (kuhn_5_1_3, UNICODE_SURROGATE_PAIR);
    EXPECT (kuhn_5_1_4, UNICODE_SURROGATE_PAIR);
    EXPECT (kuhn_5_1_5, UNICODE_SURROGATE_PAIR);
    EXPECT (kuhn_5_1_6, UNICODE_SURROGATE_PAIR);
    EXPECT (kuhn_5_1_7, UNICODE_SURROGATE_PAIR);
}

static void
utf_8_test_5_2 ()
{
    int i;

    /* These all have two items in them. */

    for (i = 0; i < 6; i += 3) {
	EXPECT (kuhn_5_2_1 + i, UNICODE_SURROGATE_PAIR);
	EXPECT (kuhn_5_2_2 + i, UNICODE_SURROGATE_PAIR);
	EXPECT (kuhn_5_2_3 + i, UNICODE_SURROGATE_PAIR);
	EXPECT (kuhn_5_2_4 + i, UNICODE_SURROGATE_PAIR);
	EXPECT (kuhn_5_2_5 + i, UNICODE_SURROGATE_PAIR);
	EXPECT (kuhn_5_2_6 + i, UNICODE_SURROGATE_PAIR);
    }
}

static void
utf_8_test_5_3 ()
{
    int i;
    int l;
    EXPECT (kuhn_5_3_1, UNICODE_NOT_CHARACTER);
    EXPECT (kuhn_5_3_2, UNICODE_NOT_CHARACTER);
    l = strlen (kuhn_5_3_3);
    for (i = 0; i < l; i += 3) {
	printf ("# i = %d, %X %X %X\n", i, (uint8_t) kuhn_5_3_3[i],
		(uint8_t) kuhn_5_3_3[i+1], (uint8_t) kuhn_5_3_3[i+2]);
	EXPECT (kuhn_5_3_3 + i, UNICODE_NOT_CHARACTER);
    }
}

static void
utf_8_test_5 ()
{
    utf_8_test_5_1 ();
    utf_8_test_5_2 ();
    utf_8_test_5_3 ();
}

int main ()
{
    utf_8_test_1 ();
    utf_8_test_2 ();
    utf_8_test_3 ();
    utf_8_test_4 ();
    utf_8_test_5 ();
    TAP_PLAN;
    return 0;
}
