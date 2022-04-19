# clownassembler

## Introduction

This is clownassembler, an assembler for Motorola 68000 assembly language. It
mimics the behaviour of SN 68k (also known as 'asm68k').


## Usage

Being a reimplementation of SN 68k, documentation for that assembler applies
to this assembler as well:

http://antime.kapsi.fi/sega/files/SATMAN.pdf


## Compiling

This assembler is written in portable ANSI C (a.k.a. C89), and so should be
able to be built with virtually any C compiler.

Before the assembler can be compiled, the lexer and parser must be generated.
Install Flex, Bison, and Make, and then run `make` in this directory to
generate them.

Once that is done, the assembler can be built with `make assemblers`.
Alternatively, the assembler can be built using CMake.

Two copies of the assembler will be produced: `clownassembler` and
`clownassembler_asm68k`. The latter provides the same command line interface as
SN 68k, while the former provides a custom, simpler, interface.


## Licensing

This is Free Software, made available under the terms of the AGPLv3+. See the
'LICENCE.txt' file for more information.
