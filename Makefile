EXEC = smaz smaz_test

all: $(EXEC)

smaz_test: smaz_test.c smaz.c
	gcc -o smaz_test -O2 -Wall -W -ansi -pedantic $^

smaz: main.c smaz.c
	gcc -o smaz -O2 -Wall -W -ansi -pedantic $^

fuzz: fuzz.c smaz.c
	afl-gcc -o $@ -O2 -Wall -W -ansi -pedantic $^

check: smaz_test
	./smaz_test

clean:
	$(RM) $(EXEC)

distclean: clean
	$(RM) *.o *~
