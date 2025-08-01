	; Amazing test

	; TODO: Test that starting a file an ORG directive does not cause the output file to begin with a bunch of padding!

  
; A line with just a colonless label
AmazingTest 

; A line with just an instruction
	move.w d0,d1 ; Absolutely NOT add.w d1,d0

; A line with a colonless label and an instruction
Lbl	add.w d1,d0 ; Absolutely NOT move.w d0,d1

; A line with a coloned label and an instruction
FunkyLabel:	move.w #$2700,sr ; A literal and the status register

; A line with just a coloned label
Label:
	move.w #0,ccr ; A literal and the condition code register
  
  
  ; A lot of blank lines to test that empty statements are properly supported
  
  
  ; A line with an address operand
	move.w d0,($00000000).w
	;adda.w d0,a1 ; Not implemented yet


	; sr instructions
	move.w	d0,sr
	move.w	($FFFF8000).w,sr

	; usp instructions
	move.l	a0,usp
	move.l	usp,a0

	; Testing the various effective address modes
	move.l d0,d0
	move.l a0,d0
	move.l (a0),d0
	move.l (a0)+,d0
	move.l -(a0),d0
	move.l 20(a0),d0
	move.l (a0,d0.w),d0
	move.l 20(a0,d0.w),d0
	move.l (a0,a1.w),d0
	move.l 20(a0,a1.w),d0
	move.l 20(pc),d0
	move.l (pc,d0.w),d0
	move.l 20(pc,d0.w),d0
	move.l (pc,a0.w),d0
	move.l 20(pc,a0.w),d0

	; Testing ori
	ori.w	#$FFFF,sr
	ori.b	#$FF,ccr
	ori.l	#$FFFFFFFF,d0

	; Testing andi
	andi.w	#$FFFF,sr
	andi.b	#$FF,ccr
	andi.l	#$FFFFFFFF,d0

	subi.w	#1,d0
	addi.w	#1,d0

	; Testing eori
	eori.w	#$FFFF,sr
	eori.b	#$FF,ccr
	eori.l	#$FFFFFFFF,d0

	; Testing cmpi
	cmpi.w	#1,d0
	cmpi.w	#1,(a0)
	cmpi.w	#1,(a0)+
	cmpi.w	#1,-(a0)
	cmpi.w	#1,$10(a0)
	cmpi.w	#1,(a0,d0.w)
DummyDummyDummy:
	; Testing both modes of BTST/BCHG/BCLR/BSET
	btst.l	#0,d0
	btst.b	#0,(a0)
	bchg.l	#0,d0
	bchg.b	#0,(a0)
	bclr.l	#0,d0
	bclr.b	#0,(a0)
	bset.l	#0,d0
	bset.b	#0,(a0)
	btst.l	d0,d0
	btst.b	d0,(a0)
	bchg.l	d0,d0
	bchg.b	d0,(a0)
	bclr.l	d0,d0
	bclr.b	d0,(a0)
	bset.l	d0,d0
	bset.b	d0,(a0)
	bset.b	d0,#$AA ; What a wacky instruction...

	movep.l	d0,0(a0)
	movep.w	10(a0),d0

	movea.l	#$FFFF,a0
	movea.w	d0,a0

	negx.w	d0
	clr.w	d0
	neg.w	d0
	not.w	d0

	tst.w	d0

	ext.w	d0
	ext.l	d1

	nbcd	($FFFF8000).w

	pea.l	($FFFFFFFF).w

	illegal

	tas.b	d0

	trap	#4

	link	a0,#12
	unlk	a0

	reset
	nop
	stop	#$2700
	rte
	rts
	trapv
	rtr

	jsr (a0)
	jmp (0).w

	movem.w	a1/d0-d2/d4-a1/d3,-(sp)
	movem.w	(sp)+,a1/d0-d2/d4-a1/d3
	movem.w	d0-a7,-(sp)
	movem.w d0,-(sp)

	lea	10(a0),a0

	chk.w	#12,d0

	divu.w	#2,d0
	divs.w	#2,d0
	mulu.w	#2,d0
	muls.w	#2,d0

	addq.w	#1,d0
	addq.l	#8,d0
	subq.w	#1,d0
	subq.l	#8,d0

	st.b	($FFFF81D0).w
	sf.b	($FFFF81D0).w
	shi.b	($FFFF81D0).w
	sls.b	($FFFF81D0).w
	scc.b	($FFFF81D0).w
	scs.b	($FFFF81D0).w
	sne.b	($FFFF81D0).w
	seq.b	($FFFF81D0).w
	svc.b	($FFFF81D0).w
	svs.b	($FFFF81D0).w
	spl.b	($FFFF81D0).w
	smi.b	($FFFF81D0).w
	sge.b	($FFFF81D0).w
	slt.b	($FFFF81D0).w
	sgt.b	($FFFF81D0).w
	sle.b	($FFFF81D0).w

	dbf	d0,0

	bra.s	$180
	bra.w	0
	bsr.s	$180
	bsr.w	0
	bhi.s	$180
	bls.s	$180
	bcc.s	$180
	bcs.s	$180
	bne.s	$180
	beq.s	$180
	bvc.s	$180
	bvs.s	$180
	bpl.s	$180
	bmi.s	$180
	bge.s	$180
	blt.s	$180
	bgt.s	$180
	ble.s	$180
	bhi.w	0
	bls.w	0
	bcc.w	0
	bcs.w	0
	bne.w	0
	beq.w	0
	bvc.w	0
	bvs.w	0
	bpl.w	0
	bmi.w	0
	bge.w	0
	blt.w	0
	bgt.w	0
	ble.w	0

	moveq	#0,d0
	moveq	#$FFFFFFFF,d1
	moveq	#$FFFFFF80,d2

	sbcd	d0,d1
	sbcd	-(a0),-(a1)
	abcd	d0,d1
	abcd	-(a0),-(a1)
	subx.w	d0,d1
	subx.w	-(a0),-(a1)
	addx.w	d0,d1
	addx.w	-(a0),-(a1)

	or.w	d0,d0
	sub.w	d0,d0
	eor.w	d0,d0
	and.w	d0,d0
	add.w	d0,d0

	suba.w	d0,a0
	cmpa.w	d0,a0
	adda.w	d0,a0

	cmpm.w	(a0)+,(a1)+

	cmp.w	(0).w,d0

	exg	d0,d1
	exg	a0,a1
	exg	d0,a0
	exg	a0,d0

	asl.w	#1,d0
	asl.w	d0,d0
	asr.w	#1,d0
	asr.w	d0,d0
	lsl.w	#1,d0
	lsl.w	d0,d0
	lsr.w	#1,d0
	lsr.w	d0,d0
	roxl.w	#1,d0
	roxl.w	d0,d0
	roxr.w	#1,d0
	roxr.w	d0,d0
	rol.w	#1,d0
	rol.w	d0,d0
	ror.w	#1,d0
	ror.w	d0,d0

	asl.w	(a0)
	asr.w	(a0)
	lsl.w	(a0)
	lsr.w	(a0)
	roxl.w	(a0)
	roxr.w	(a0)
	rol.w	(a0)
	ror.w	(a0)

	; Testing the symbol table
TestLabel:
	bra.s	TestLabel


	; Testing fix-ups
	bra.w	DeclareAfterUseLabel
DeclareAfterUseLabel:

	; Some actual regular code
CoolFunction:
	moveq	#0,d2
	tst.w	d1
	beq.s	CoolFunction_Exit

CoolFunction_Loop:
	add.w	d0,d2
	dbf	d1,CoolFunction_Loop

CoolFunction_Exit:
	rts

	move.w	#%0101010101010101,d0

	; Testing out arithmetic in literals
	move.w	#1+1,d0       ; 2
	move.w	#1+(2*3),d0   ; 7
	move.w	#(2*3)+1,d0   ; 7
	move.w	#-1,d0        ; $FFFF
	move.w	#1+2-3*4/5,d0 ; 3
	move.w	#1--1,d0      ; 2

	; The Macro Assembler AS would fail to assemble this line (it would mistake the `(1*2)` for an absolute address)
	move.w	(1*2)+1(a0,d0.w),d0

	dc.b	0,1,2,3,4,5
	dc.w	0,1,2,3,4,5
	dc.l	0,1,2,3,4,5
	even

	; Test local labels.
Object:
	moveq	#0,d0
	move.b	2(a0),d0
	add.w	d0,d0
	move.w	@Offsets(pc,d0.w),d0
	jmp	@Offsets(pc,d0.w)

@Offsets:
	dc.w	@Offset1-Object@Offsets
	dc.w	Object@Offset2-@Offsets

@Offset1:
	rts

@Offset2:
	rts

	bra.s	*
	bra.s	1*(**1) ; Amazingly, this actually works as intended

	dc.l	*,*,*
	dc.l	@,@,@

	; Test operators
	dc.b	1<<1, 4>>1, 3&2, 7^5, 0!2, 0|2, 1=1, 1==1, 1<>0, 1!=0, 1>0, 0<1, 1&&1, 1||1, 0+1, 2-1, 1*1, 1/1, 7%2
	even

	; Testing REPT
Delta:	rept 8
	dc.b	*-Delta
	endr

	; Check that multiple REPTs in the same file work
	rept 2
	dc.l	$FEEDBEEF
	endr

	; Testing case-insensitivity
	MOVE.B	D0,D0
	mOvE.b	(A0,d0.W),(a1,D1.l)

	; Testing macros
TheGreatestMacro macro named_parameter_1, named_parameter_2
	dc.l	*
	move.\0	\1,\2
	move.\0	named_parameter_1,named_parameter_2
	dc.l	narg
	endm

	TheGreatestMacro.b (a0,d0.w),(a1,d1.l)
	TheGreatestMacro.w #1000,(0).l,#$DEAD

TheGreatestMacro2 MACRO opcode
	\opcode
	ENDM

	TheGreatestMacro2 illegal

	; Testing character literals
	dc.b	'S',"E",'G',"A"
	dc.w	'SE',"GA"
	dc.l	'SEGA',"AGES"
	dc.b	"This is a dc.b holding way more than a single byte."
	even

	; Testing binclude/incbin
	binclude "binary.bin"
	dc.l	'STRT'
	incbin "binary.bin",2
	dc.l	'AGAI'
	incbin "binary.bin",1,1

	; Testing DCB
	dcb.b	4,$11
	dcb.w	4,$22
	dcb.l	4,$44
	even

	; Testing EQU
TimesToRept equ 2
TimesToNotRept equ 1

	rept TimesToRept
	dc.l	'TWO2'
	endr

	; Testing IF/ELSEIF/ELSE/ENDC/ENDIF
	if TimesToRept=1
		if TimesToNotRept=2
			dc.l	'NOPE'
		endif

		dc.l	'FLS1'
	elseif TimesToNotRept=1
		dc.l	'TRUE'
	else
		dc.l	'FLS2'
	endc

	IF 0
	ELSE
	ENDIF

	if 1
		dc.l 'GOOD'
	else
		dc.l 'OHNO'
	endif

	if 1
		dc.l 'GOOD'
	elseif 2
		dc.l 'BAD1'
	else
		dc.l 'BAD2'
	endif

	if 1
		dc.l 'GOOD'
	elseif 0
		dc.l 'BAD1'
	else
		dc.l 'BAD2'
	endif

	if 1
		dc.l 'GOOD'
	else
		if 0
			dc.l 'BAD1'
		else
			dc.l 'BAD2'
		endif
	endif

	if 1
		if 0
			dc.l 'BAD1'
		else
			dc.l 'GOOD'
		endif
	else
		dc.l 'BAD2'
	endif

	; Testing multi-line statements
	move.b &
	d0 &
	, &
	d0

	; Testing RS/RSSET/RSRESET
		rsset $200
Struct_A_A:	rs.w 1
Struct_A_B:	rs.b 3
Struct_A_C:	rs.b 3
Struct_A_D:	rs.l 2

		rsreset
Struct_B_A:	rs.b 1
Struct_B_B:	rs.b 2
Struct_B_C:	rs.b 3
Struct_B_D:	rs.b 4

		rsreset $100
Struct_C_A:	rs.b 1
Struct_C_B:	rs.b 2
Struct_C_C:	rs.b 3
Struct_C_D:	rs.b 4

Struct_C_Size EQU __rs
	dc.w	Struct_C_Size

	; Testing variables
variable = 0
	; The use of FutureLabel will cause this statement to become a fix-up.
	; If variables aren't accounted for properly, then this statement will
	; re-evaluate with 'variable' set to 1 instead of 0.
	dc.b	variable+(FutureLabel-FutureLabel) ; Should evaluate to 0
variable set 1
	dc.b	variable ; Should evaluate to 1

FutureLabel:


	; Testing strlen
	dc.b	strlen("Four")
i set 0
	while i<strlen("DRR" 	)
	dc.b i
i set i+1
	endw

	; Testing numerical notations

	dc.b	16

	; 68k
	dc.b	$10
	dc.b	%10000

	; Z80
	dc.b	10h
	dc.b	10000b
	dc.b	10H
	dc.b	10000B

	; C
	dc.b	0x10
	dc.b	0b10000
	dc.b	0X10
	dc.b	0B10000
	even

	; Test integer truncation: should result in '0x76543210'.
	move.l	#$FEDCBA9876543210,d0

	; Test '\*' macro.
StarTest macro *
\* = *
\*_wowza = *
	endm

Test_a: StarTest
	dc.w	0
Test_b: StarTest
	dc.w Test_b_wowza - Test_a_wowza
	dc.w Test_b - Test_a

	; Test '\@' macro.
AtTest macro value
	moveq	#4-1,d1
loop\@:
	moveq	#0,d0
	tst.w	value
	beq.s	skipper\@
	moveq	#1,d0
skipper\@:
	dbf	d1,loop\@
	endm

	AtTest d2
	AtTest (a2)
	AtTest (0).w

	; Test '\#' macro.
TellMe macro
	dc.b "variable is \#variable"
	endm

variable set 10
	TellMe
variable set 11
	TellMe
variable set 12
	TellMe

	; Test '\$' macro.
TellMeHex macro
	dc.b "variable is 0\$variable\h"
	endm

variable set 10
	TellMeHex
variable set 11
	TellMeHex
variable set 12
	TellMeHex
	even

	; Test obj
	dc.l	*
	dc.l	*

	obj $200
	dc.l	*
	dc.l	*
	objend

	dc.l	*
	dc.l	*

	; Test org
	dc.b "watch me skip 4 bytes"
	org *+4
	dc.b "wow"

	; Test an edgecase involving a combination of obj and org
LetsGetWacky:
	obj LetsGetWacky+1
	org LetsGetWacky
	if *<>LetsGetWacky
		inform 2,"Wrong ORG address"
	endif
	dc.l *
	objend
	if *<>LetsGetWacky+3
		inform 2,"Wrong OBJEND address"
	endif
	dc.l *

	; Test DEF
	if def(Spaghetti)
	dc.l	'NOPE'
	else
	dc.l	'GOOD'
	endif

Spaghetti = $BEEF

	if !def(Spaghetti)
	dc.l	'NOPE'
	else
	dc.l	'GOOD'
	endif

	; Test dotted labels
Swag.Legit:
	dc.l	Swag.Legit

	; Test alternate syntaxes
	cnop 0,2
	pea	Swag.Legit.w
	pea	Swag.Legit.l

	; Test incorrect code that is usually automatically corrected by assemblers
	cmp.w	#1,d0
	cmp.w	a0,a1
	cmp.w	(a0)+,(a1)+
	or.w	#$FFFF,d0
	eor.w	#$FFFF,d0
	and.w	#$FFFF,d0
	add.w	#$FFFF,d0
	sub.w	#$FFFF,d0
	add.w	#$FFFF,a0
	sub.w	#$FFFF,a0
	lsl.w	d0
	lsr.w	d0
	asl.w	d0
	asr.w	d0
	rol.w	d0
	ror.w	d0
	roxl.w	d0
	roxr.w	d0
	and.w	#1,sr
	eor.w	#1,sr
	or.w	#1,sr
	and.b	#1,ccr
	eor.b	#1,ccr
	or.b	#1,ccr
	move.l	d0/d1,-(sp)
	move.l	(sp)+,d0/d1

	; Vladikcomper tests (invalid but correctable operand and instruction combinations)
	cmp.l	a1, a2
	cmp.w	#$4EF9, (a2)+
	and.w	#$FF, d1
	add.w	(a3), a3
	lsr.w	d0

	; Nested REPTs (Sik, #17).
	rept 2
	rept 3
	dc.b	6
	endr
	endr

	; EQUS and SUBSTR.
StringVar EQUS "stringvar!"
	dc.b "\StringVar"

StringVar2 SUBSTR 1,5,"\StringVar"
	dc.b "\StringVar2"

Message equs "A short Sample String"
Part1 substr 9,14,"\Message"
Part2 substr 16,,"\Message"
Part3 substr ,7,"\Message"
Part4 substr ,,"\Message"

	dc.b '-'
	dc.b "\Part1"
	dc.b '-'
	dc.b "\Part2"
	dc.b '-'
	dc.b "\Part3"
	dc.b '-'
	dc.b "\Part4"
	dc.b '-'

	; Test nested WHILE loops.
	while (* & 3) != 0
	dc.b * & 3
	endw

	dc.b $AA

	while (* & 1) != 0
	while (* & 3) != 0
	dc.b * & 3
	endw
	endw

	dc.b $AA

	while (* & 3) != 0
	while (* & 1) != 0
	dc.b * & 3
	endw
	while (* & 2) != 0
	dc.b * & 3
	endw
	endw

	; Test SHIFT, as well as REPT nested with a MACRO.
Hell macro named
	rept narg
	dc.b "\named"
	dc.b "\1"
	dc.b "\_"
	dc.b narg
	shift
	endr
    endm

	Hell hath,no,fury

	; Test SHIFT, as well as WHILE nested with a MACRO.
Hell2 macro named
	while var<>narg
var = var+1
	dc.b "\named"
	dc.b "\1"
	dc.b "\_"
	dc.b narg
	shift
	endw
    endm

var = 0
	Hell2 hath,no,fury

	; Test some listing quirks.
    rept 6
	dc.l *
    endr

	; Test some of asm68k's bizarre string-substitution quirks.
SUBSTITUTE EQUS "dc.b"
SUBSTITUTE2 EQUS "Hell"

	\SUBSTITUTE2 SUBSTITUTE,\SUBSTITUTE,\SUBSTITUTE\

	; Despite sharing a name, the macro redefining this constant does not cause an error.
shadowedByMacro EQU 0

shadowTheConstant macro shadowedByMacro
	dc.w shadowedByMacro
	endm

	shadowTheConstant 1

shadowedByMacro2 EQUS '"NO"'

shadowTheConstant2 macro shadowedByMacro2
	dc.b shadowedByMacro2
	endm

	shadowTheConstant2 "OK"

	; Macro parameter double-definition, which SN 68k allows.
doulbleParamMacro macro param, param
	dc.w	param
	endm

	doulbleParamMacro $DEAD, $BEEF

	; SUBSTR with start before end, and DC.B with empty string.
emptysubstr substr 2,,"0"
	dc.b "\emptysubstr" ; oops, all empty

	; Conversion of non-string macro arguments to lowercase in case-sensitive mode.
paramlowercase macro param
	dc.b strcmp("aaa",\param)
		endm
paramlowercase2 macro param
	dc.b strcmp("aaa","\param")
		endm

	opt c+
	paramlowercase "AAA"
	paramlowercase2 AAA

	opt c-
	paramlowercase "AAA"
	paramlowercase2 AAA

  ; More blank lines to test support for trailing blank statements


