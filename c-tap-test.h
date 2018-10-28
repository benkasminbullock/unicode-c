/* TAP format macros. */

static int tap_count;
static int tap_todo;
static int tap_fail;

#define ENDLINE {				\
	if (tap_todo) {				\
	    printf (" # TODO\n");		\
	}					\
	else {					\
	    printf ("\n");			\
	}					\
    }

#define TAP_TEST(x) {					\
	tap_count++;					\
	if (! (x)) {					\
	    if (! tap_todo) {				\
		tap_fail++;				\
	    }						\
	    printf ("not ");				\
	}						\
	printf ("ok %d - %s", tap_count, #x);		\
	ENDLINE;					\
    }

#define TAP_TEST_EQUAL(x,y) {					\
	int xval = (int) x;					\
	int yval = (int) y;					\
	tap_count++;						\
	if (! (xval == yval)) {					\
	    if (! tap_todo) {					\
		tap_fail++;					\
	    }							\
	    printf ("not ");					\
	}							\
	printf ("ok %d - %s (%d) == %s (%d)", tap_count,	\
		#x, xval, #y, yval);				\
	ENDLINE;						\
    }

#define TAP_TEST_MSG(x,msg,args...) {			\
	tap_count++;					\
	if (! (x)) {					\
	    if (! tap_todo) {				\
		tap_fail++;				\
	    }						\
	    printf ("not ");				\
	}						\
	printf ("ok %d - ", tap_count);			\
	printf (msg, ## args);				\
	ENDLINE;					\
    }

#define TODO tap_todo = 1
#define END_TODO tap_todo = 0

#define TAP_PLAN {				\
	printf ("1..%d\n", tap_count);		\
    }

