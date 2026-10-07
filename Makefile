LEX = flex
YACC = bison

ifeq ($(DEBUG),1)
 CFLAGS = -ansi -Wno-long-long -Wall -Wextra -pedantic -Wc++-compat -ggdb3 -Og -fwrapv -Wshift-overflow
 YFLAGS = --debug
else
 CFLAGS = -ansi -Wno-long-long -Wall -Wextra -pedantic -Wc++-compat -O2 -DNDEBUG
endif

ifeq ($(COUNTEREXAMPLES),1)
 YFLAGS += -Wcounterexamples
endif

all: generators

generators: lexical.cpp lexical.h syntactic.cpp syntactic.h

lexical.cpp lexical.h: lexical.l syntactic.h
	$(LEX) --outfile=lexical.cpp --header-file=lexical.h $<

syntactic.cpp syntactic.h: syntactic.y
	$(YACC) --output=syntactic.cpp --defines=syntactic.h $(YFLAGS) $<

assemblers: clownassembler clownassembler_asm68k

clownassembler_asm68k: frontend-asm68k.cpp io.cpp lexical.cpp options.cpp p2bin.cpp semantic.cpp strcmpci.cpp string-stack.cpp substitute.cpp syntactic.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

clownassembler: frontend-custom.cpp io.cpp lexical.cpp options.cpp p2bin.cpp semantic.cpp strcmpci.cpp string-stack.cpp substitute.cpp syntactic.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
