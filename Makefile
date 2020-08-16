EXEC    = smaz_test smaz_tool

all: $(EXEC)

smaz_test: smaz_test.c smaz.c
	gcc -o smaz_test -O2 -Wall -W -ansi -pedantic $^

smaz_tool: smaz.c smaz_tool.c
	gcc -o smaz_tool -O2 -Wall -W -ansi -pedantic $^

check: smaz_test
	./smaz_test

clean:
	$(RM) $(EXEC)

distclean: clean
	$(RM) *.o *~
