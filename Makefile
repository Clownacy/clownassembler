CFLAGS = -ggdb3 -Og -Wall -Wextra -pedantic -fwrapv

all: generators

generators: lexer.c lexer.h parser.c parser.h

lexer.c lexer.h: spl.l parser.h
	flex --outfile=lexer.c --header-file=lexer.h $<

parser.c parser.h: spl.y
	bison --output=parser.c --header=parser.h --debug $<

clownassembler: main.c lexer.c parser.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
