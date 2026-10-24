LEX = flex
YACC = bison

ifeq ($(COUNTEREXAMPLES),1)
 YFLAGS += -Wcounterexamples
endif

all: generators

generators: lexical.cpp lexical.h syntactic.cpp syntactic.h

lexical.cpp lexical.h: lexical.l syntactic.h
	$(LEX) --outfile=lexical.cpp --header-file=lexical.h $<

syntactic.cpp syntactic.h: syntactic.y
	$(YACC) --output=syntactic.cpp --defines=syntactic.h $(YFLAGS) $<
