LEX = flex
YACC = bison

CFLAGS = -ggdb3 -Og -Wall -Wextra -pedantic -fwrapv -ansi -Wno-long-long

all: generators

generators: lexical.c lexical.h syntactic.c syntactic.h

lexical.c lexical.h: lexical.l syntactic.h
	$(LEX) --outfile=lexical.c --header-file=lexical.h $<

syntactic.c syntactic.h: syntactic.y
	$(YACC) --output=syntactic.c --header=syntactic.h --debug $<

clownassembler: main.c dictionary.c lexical.c semantic.c syntactic.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
