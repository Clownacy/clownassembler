LEX = flex
YACC = bison

ifeq ($(DEBUG),1)
 CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -ggdb3 -Og -fwrapv -Wshift-overflow
 YFLAGS = --debug
else
 CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -O2 -DNDEBUG
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

clownassembler_asm68k: frontend-asm68k.cpp lexical.cpp p2bin.cpp semantic.cpp substitute.cpp syntactic.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

clownassembler: frontend-custom.cpp lexical.cpp p2bin.cpp semantic.cpp substitute.cpp syntactic.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
