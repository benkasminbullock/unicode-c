test:	unicode-test
	./unicode-test

unicode-test:	unicode.c
	cc -g -Wall -DTEST unicode.c -o unicode-test
