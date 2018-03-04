install: unicode.3
	cp -f unicode.3 /home/ben/software/install/man/man3/unicode.3

unicode.3: unicode.3.tmpl make-man.pl doc.json
	./make-man.pl

doc.json: ../unicode.c make-doc.pl
	./make-doc.pl

clean:
	rm -f unicode.3 doc.json
