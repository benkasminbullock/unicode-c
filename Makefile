OBJS=unicode.o unicode-character-class.o
CFLAGS=-g -Wall -O

all:	libunicode.a unicode.h
	cd doc;make

install: all docinstall

docinstall:	unicode.c
	cd doc;make;make install

libunicode.a:	$(OBJS)
	ar rcs $@ $(OBJS)

unicode.o:	unicode.c unicode.h
	$(CC) $(CFLAGS) -c -o unicode.o unicode.c

unicode-character-class.o:	unicode-character-class.c unicode-character-class.h unicode.h
	$(CC) $(CFLAGS) -c -o $@ unicode-character-class.c

TESTS=unicode-test unicode-character-class-test utf-8-test
DOTTESTS=./unicode-test ./unicode-character-class-test ./utf-8-test

test:	$(TESTS)
	prove --nocolor $(DOTTESTS)

unicode-test:	unicode.c unicode.h c-tap-test.h
	$(CC) $(CFLAGS) -DTEST unicode.c -o unicode-test

unicode-character-class-test:	unicode-character-class.c unicode.o unicode-character-class.h unicode.h
	$(CC) $(CFLAGS) -o $@ -D TEST unicode-character-class.c unicode.o

utf-8-test: utf-8-test.c utf-8-test-data.c unicode.h c-tap-test.h unicode.c
	$(CC) $(CFLAGS) -o $@ utf-8-test.c unicode.c

clean:
	-rm -f $(OBJS) \
	libunicode.a \
	unicode-character-class-test \
	unicode-test \
	utf-8-test \
	ZZZZZ
