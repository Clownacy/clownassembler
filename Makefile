CFLAGS = -ggdb3 -Og -Wall -Wextra -pedantic -fwrapv

all: generators

generators: lexical.c lexical.h syntactical.c syntactical.h

lexical.c lexical.h: spl.l syntactical.h
	flex --outfile=lexical.c --header-file=lexical.h $<

syntactical.c syntactical.h: spl.y
	bison --output=syntactical.c --header=syntactical.h --debug $<

clownassembler: main.c lexical.c syntactical.c symbols.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
