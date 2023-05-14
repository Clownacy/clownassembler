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

To build with Make, run `make assemblers` in this directory. Alternatively,
the assembler can be built using CMake.

Two copies of the assembler will be produced: `clownassembler` and
`clownassembler_asm68k`. The latter provides the same command line interface as
SN 68k, while the former provides a custom, simpler, interface.

To regenerate the lexer and parser, install Flex and Bison and then run `make`.


## Licensing

This is Free Software, made available under the terms of the AGPLv3+. See the
'LICENCE.txt' file for more information.
