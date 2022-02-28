	; Amazing test
  
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
	cmpi.w	#1,$10(pc)
	cmpi.w	#1,$10(pc,d0.w)

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
	stop
	rte
	rts
	trapv
	rtr

	jsr (a0)
	jmp (0).w

	movem.w	a1/d0-d2/d4-a1/d3,-(sp)
	movem.w	(sp)+,a1/d0-d2/d4-a1/d3
	movem.w	d0-a7,-(sp)

	lea	10(a0),a0

	chk.w	#12,d0

	divu.w	#2,d0
	divs.w	#2,d0
	mulu.w	#2,d0
	muls.w	#2,d0

  ; More blank lines to test support for trailing blank statements
