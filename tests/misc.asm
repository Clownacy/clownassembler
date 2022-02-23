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

  ; More blank lines to test support for trailing blank statements
