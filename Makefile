OBJS=unicode.o unicode-character-class.o
CFLAGS=-g -Wall
CFUNCTIONS=/home/ben/software/install/bin/cfunctions

all:	libunicode.a unicode.h docinstall

docinstall:	unicode.c
	cd doc;make;cp -f unicode.3 /home/ben/software/install/man/man3/

libunicode.a:	$(OBJS)
	ar rcs $@ $(OBJS)

unicode.o:	unicode.c unicode.h
	$(CC) $(CFLAGS) -c -o unicode.o unicode.c

unicode.h:	unicode.c
	$(CFUNCTIONS) unicode.c

unicode-character-class.o:	unicode-character-class.c unicode-character-class.h unicode.h
	$(CC) $(CFLAGS) -c -o $@ unicode-character-class.c

unicode-character-class.h:	unicode-character-class.c
	$(CFUNCTIONS) unicode-character-class.c

test:	unicode-test unicode-character-class-test
	prove --nocolor ./unicode-test ./unicode-character-class-test

unicode-test:	unicode.c unicode.h c-tap-test.h
	cc -g -Wall -DTEST unicode.c -o unicode-test

unicode-character-class-test:	unicode-character-class.c unicode.o unicode-character-class.h unicode.h
	$(CC) $(CFLAGS) -o $@ -D TEST unicode-character-class.c unicode.o

CTT=/home/ben/projects/c-tap-test

c-tap-test.h: $(CTT)/$@
	if [ -f $@ ]; then chmod 0644 $@; fi
	cp $(CTT)/$@ $@
	chmod 0444 $@

clean:
	-rm -f $(OBJS) \
	libunicode.a \
	unicode.h \
	unicode-test \
	unicode-character-class-test \
	unicode-character-class.h \
	c-tap-test.h

