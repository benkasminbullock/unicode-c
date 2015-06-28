OBJS=unicode.o unicode-character-class.o
CFLAGS=-g -Wall

all:	libunicode.a unicode.h

libunicode.a:	$(OBJS)
	ar rcs $@ $(OBJS)

unicode.o:	unicode.c unicode.h
	$(CC) $(CFLAGS) -c -o unicode.o unicode.c

unicode.h:	unicode.c unicode-character-class.c
	cfunctions -g unicode -n unicode.c unicode-character-class.c

unicode-character-class.o:	unicode-character-class.c unicode.h
	$(CC) $(CFLAGS) -c -o $@ unicode-character-class.c


test:	unicode-test unicode-character-class-test
	prove ./unicode-test
	./unicode-character-class-test

unicode-test:	unicode.c unicode.h
	cc -g -Wall -DTEST unicode.c -o unicode-test

unicode-character-class-test:	unicode-character-class.c unicode.o
	$(CC) $(CFLAGS) -o $@ -D TEST unicode-character-class.c unicode.o

clean:
	-rm -f $(OBJS) libunicode.a unicode.h unicode-test unicode-character-class-test

