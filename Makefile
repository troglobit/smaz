EXEC   = smaz test
CFLAGS = -O2 -Wall -W -ansi -pedantic

all: $(EXEC)

test: test.c smaz.c
	gcc -o $@ $(CFLAGS) $^

smaz: main.c smaz.c
	gcc -o $@ $(CFLAGS) $^

fuzz: fuzz.c smaz.c
	afl-gcc -o $@ $(CFLAGS) $^

check: test
	./$^

clean:
	$(RM) $(EXEC)

distclean: clean
	$(RM) *.o *~
