all: unicode.3 unicode.html

unicode.3: unicode.3.tmpl make-man.pl doc.json
	./make-man.pl

unicode.html: make-html.pl doc.json see-also.html.tmpl
	./make-html.pl

doc.json: ../unicode.c make-doc.pl
	./make-doc.pl

install: all
	cp -f unicode.3 /home/ben/software/install/man/man3/unicode.3
	cp -f unicode.html /usr/local/www/data/unicode.html

clean:
	rm -f unicode.3 doc.json unicode.html
