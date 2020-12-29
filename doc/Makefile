all: unicode.3 unicode.html ../docs/index.html

../docs/index.html: unicode.html
	if [ -f $@ ]; then chmod 0644 $@; fi
	cp -f unicode.html $@
	chmod 0444 $@

unicode.3: unicode.3.tmpl make-man.pl doc.json
	./make-man.pl

unicode.html: make-html.pl doc.json
	./make-html.pl

doc.json: ../unicode.c make-doc.pl
	./make-doc.pl

install: all
	cp -f unicode.3 /home/ben/software/install/man/man3/unicode.3
	cp -f unicode.html /usr/local/www/data/unicode.html

clean:
	rm -f unicode.3 doc.json unicode.html
