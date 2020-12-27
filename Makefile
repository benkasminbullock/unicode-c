OBJS=unicode.o unicode-character-class.o
CFLAGS=-g -Wall -O

all:	libunicode.a unicode.h
	cd doc;make

install: all docinstall

docinstall:	unicode.c
	cd doc;make;cp -f unicode.3 /home/ben/software/install/man/man3/

libunicode.a:	$(OBJS)
	ar rcs $@ $(OBJS)

unicode.o:	unicode.c unicode.h
	$(CC) $(CFLAGS) -c -o unicode.o unicode.c

unicode-character-class.o:	unicode-character-class.c unicode-character-class.h unicode.h
	$(CC) $(CFLAGS) -c -o $@ unicode-character-class.c

test:	unicode-test unicode-character-class-test
	prove --nocolor ./unicode-test ./unicode-character-class-test

unicode-test:	unicode.c unicode.h c-tap-test.h
	$(CC) $(CFLAGS) -DTEST unicode.c -o unicode-test

unicode-character-class-test:	unicode-character-class.c unicode.o unicode-character-class.h unicode.h
	$(CC) $(CFLAGS) -o $@ -D TEST unicode-character-class.c unicode.o

clean:
	-rm -f $(OBJS) \
	libunicode.a \
	unicode-character-class-test \
	unicode-test \
	ZZZZZ
