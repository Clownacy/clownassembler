	ori.b	#$FF,ccr
	ori	#$FF,ccr
	ori.w	#$FFFF,sr
	ori	#$FFFF,sr
	ori.b	#$FF,d0
	ori.b	#$FF,(a0)
	ori.b	#$FF,(a0)+
	ori.b	#$FF,-(a0)
	ori.b	#$FF,$7FFF(a0)
	ori.b	#$FF,$7F(a0,d0.w)
	ori.b	#$FF,($FFFFFFFF).w
	ori.b	#$FF,($FFFFFFFF).l
	ori.w	#$FFFF,d0
	ori.w	#$FFFF,(a0)
	ori.w	#$FFFF,(a0)+
	ori.w	#$FFFF,-(a0)
	ori.w	#$FFFF,$7FFF(a0)
	ori.w	#$FFFF,$7F(a0,d0.w)
	ori.w	#$FFFF,($FFFFFFFF).w
	ori.w	#$FFFF,($FFFFFFFF).l
	ori.l	#$FFFFFFFF,d0
	ori.l	#$FFFFFFFF,(a0)
	ori.l	#$FFFFFFFF,(a0)+
	ori.l	#$FFFFFFFF,-(a0)
	ori.l	#$FFFFFFFF,$7FFF(a0)
	ori.l	#$FFFFFFFF,$7F(a0,d0.w)
	ori.l	#$FFFFFFFF,($FFFFFFFF).w
	ori.l	#$FFFFFFFF,($FFFFFFFF).l

	andi.b	#$FF,ccr
	andi	#$FF,ccr
	andi.w	#$FFFF,sr
	andi	#$FFFF,sr
	andi.b	#$FF,d0
	andi.b	#$FF,(a0)
	andi.b	#$FF,(a0)+
	andi.b	#$FF,-(a0)
	andi.b	#$FF,$7FFF(a0)
	andi.b	#$FF,$7F(a0,d0.w)
	andi.b	#$FF,($FFFFFFFF).w
	andi.b	#$FF,($FFFFFFFF).l
	andi.w	#$FFFF,d0
	andi.w	#$FFFF,(a0)
	andi.w	#$FFFF,(a0)+
	andi.w	#$FFFF,-(a0)
	andi.w	#$FFFF,$7FFF(a0)
	andi.w	#$FFFF,$7F(a0,d0.w)
	andi.w	#$FFFF,($FFFFFFFF).w
	andi.w	#$FFFF,($FFFFFFFF).l
	andi.l	#$FFFFFFFF,d0
	andi.l	#$FFFFFFFF,(a0)
	andi.l	#$FFFFFFFF,(a0)+
	andi.l	#$FFFFFFFF,-(a0)
	andi.l	#$FFFFFFFF,$7FFF(a0)
	andi.l	#$FFFFFFFF,$7F(a0,d0.w)
	andi.l	#$FFFFFFFF,($FFFFFFFF).w
	andi.l	#$FFFFFFFF,($FFFFFFFF).l

	subi.b	#$FF,d0
	subi.b	#$FF,(a0)
	subi.b	#$FF,(a0)+
	subi.b	#$FF,-(a0)
	subi.b	#$FF,$7FFF(a0)
	subi.b	#$FF,$7F(a0,d0.w)
	subi.b	#$FF,($FFFFFFFF).w
	subi.b	#$FF,($FFFFFFFF).l
	subi.w	#$FFFF,d0
	subi.w	#$FFFF,(a0)
	subi.w	#$FFFF,(a0)+
	subi.w	#$FFFF,-(a0)
	subi.w	#$FFFF,$7FFF(a0)
	subi.w	#$FFFF,$7F(a0,d0.w)
	subi.w	#$FFFF,($FFFFFFFF).w
	subi.w	#$FFFF,($FFFFFFFF).l
	subi.l	#$FFFFFFFF,d0
	subi.l	#$FFFFFFFF,(a0)
	subi.l	#$FFFFFFFF,(a0)+
	subi.l	#$FFFFFFFF,-(a0)
	subi.l	#$FFFFFFFF,$7FFF(a0)
	subi.l	#$FFFFFFFF,$7F(a0,d0.w)
	subi.l	#$FFFFFFFF,($FFFFFFFF).w
	subi.l	#$FFFFFFFF,($FFFFFFFF).l

	addi.b	#$FF,d0
	addi.b	#$FF,(a0)
	addi.b	#$FF,(a0)+
	addi.b	#$FF,-(a0)
	addi.b	#$FF,$7FFF(a0)
	addi.b	#$FF,$7F(a0,d0.w)
	addi.b	#$FF,($FFFFFFFF).w
	addi.b	#$FF,($FFFFFFFF).l
	addi.w	#$FFFF,d0
	addi.w	#$FFFF,(a0)
	addi.w	#$FFFF,(a0)+
	addi.w	#$FFFF,-(a0)
	addi.w	#$FFFF,$7FFF(a0)
	addi.w	#$FFFF,$7F(a0,d0.w)
	addi.w	#$FFFF,($FFFFFFFF).w
	addi.w	#$FFFF,($FFFFFFFF).l
	addi.l	#$FFFFFFFF,d0
	addi.l	#$FFFFFFFF,(a0)
	addi.l	#$FFFFFFFF,(a0)+
	addi.l	#$FFFFFFFF,-(a0)
	addi.l	#$FFFFFFFF,$7FFF(a0)
	addi.l	#$FFFFFFFF,$7F(a0,d0.w)
	addi.l	#$FFFFFFFF,($FFFFFFFF).w
	addi.l	#$FFFFFFFF,($FFFFFFFF).l

	eori.b	#$FF,ccr
	eori	#$FF,ccr
	eori.w	#$FFFF,sr
	eori	#$FFFF,sr
	eori.b	#$FF,d0
	eori.b	#$FF,(a0)
	eori.b	#$FF,(a0)+
	eori.b	#$FF,-(a0)
	eori.b	#$FF,$7FFF(a0)
	eori.b	#$FF,$7F(a0,d0.w)
	eori.b	#$FF,($FFFFFFFF).w
	eori.b	#$FF,($FFFFFFFF).l
	eori.w	#$FFFF,d0
	eori.w	#$FFFF,(a0)
	eori.w	#$FFFF,(a0)+
	eori.w	#$FFFF,-(a0)
	eori.w	#$FFFF,$7FFF(a0)
	eori.w	#$FFFF,$7F(a0,d0.w)
	eori.w	#$FFFF,($FFFFFFFF).w
	eori.w	#$FFFF,($FFFFFFFF).l
	eori.l	#$FFFFFFFF,d0
	eori.l	#$FFFFFFFF,(a0)
	eori.l	#$FFFFFFFF,(a0)+
	eori.l	#$FFFFFFFF,-(a0)
	eori.l	#$FFFFFFFF,$7FFF(a0)
	eori.l	#$FFFFFFFF,$7F(a0,d0.w)
	eori.l	#$FFFFFFFF,($FFFFFFFF).w
	eori.l	#$FFFFFFFF,($FFFFFFFF).l

	cmpi.b	#$FF,d0
	cmpi.b	#$FF,(a0)
	cmpi.b	#$FF,(a0)+
	cmpi.b	#$FF,-(a0)
	cmpi.b	#$FF,$7FFF(a0)
	cmpi.b	#$FF,$7F(a0,d0.w)
	cmpi.b	#$FF,($FFFFFFFF).w
	cmpi.b	#$FF,($FFFFFFFF).l
	cmpi.b	#$FF,*(pc)
	cmpi.b	#$FF,*(pc,d0.w)
	cmpi.w	#$FFFF,d0
	cmpi.w	#$FFFF,(a0)
	cmpi.w	#$FFFF,(a0)+
	cmpi.w	#$FFFF,-(a0)
	cmpi.w	#$FFFF,$7FFF(a0)
	cmpi.w	#$FFFF,$7F(a0,d0.w)
	cmpi.w	#$FFFF,($FFFFFFFF).w
	cmpi.w	#$FFFF,($FFFFFFFF).l
	cmpi.w	#$FFFF,*(pc)
	cmpi.w	#$FFFF,*(pc,d0.w)
	cmpi.l	#$FFFFFFFF,d0
	cmpi.l	#$FFFFFFFF,(a0)
	cmpi.l	#$FFFFFFFF,(a0)+
	cmpi.l	#$FFFFFFFF,-(a0)
	cmpi.l	#$FFFFFFFF,$7FFF(a0)
	cmpi.l	#$FFFFFFFF,$7F(a0,d0.w)
	cmpi.l	#$FFFFFFFF,($FFFFFFFF).w
	cmpi.l	#$FFFFFFFF,($FFFFFFFF).l
	cmpi.l	#$FFFFFFFF,*(pc)
	cmpi.l	#$FFFFFFFF,*(pc,d0.w)

	btst.l	d0,d0
	btst.b	d0,(a0)
	btst.b	d0,(a0)+
	btst.b	d0,-(a0)
	btst.b	d0,$7FFF(a0)
	btst.b	d0,$7F(a0,d0.w)
	btst.b	d0,($FFFFFFFF).w
	btst.b	d0,($FFFFFFFF).l
	btst.b	d0,#$55
	btst.b	d0,*(pc)
	btst.b	d0,*(pc,d0.w)

	btst	d0,d0
	btst	d0,(a0)
	btst	d0,(a0)+
	btst	d0,-(a0)
	btst	d0,$7FFF(a0)
	btst	d0,$7F(a0,d0.w)
	btst	d0,($FFFFFFFF).w
	btst	d0,($FFFFFFFF).l
	btst	d0,#$55
	btst	d0,*(pc)
	btst	d0,*(pc,d0.w)

	btst.l	#0,d0
	btst.b	#0,(a0)
	btst.b	#0,(a0)+
	btst.b	#0,-(a0)
	btst.b	#0,$7FFF(a0)
	btst.b	#0,$7F(a0,d0.w)
	btst.b	#0,($FFFFFFFF).w
	btst.b	#0,($FFFFFFFF).l
	btst.b	#0,*(pc)
	btst.b	#0,*(pc,d0.w)

	btst	#0,d0
	btst	#0,(a0)
	btst	#0,(a0)+
	btst	#0,-(a0)
	btst	#0,$7FFF(a0)
	btst	#0,$7F(a0,d0.w)
	btst	#0,($FFFFFFFF).w
	btst	#0,($FFFFFFFF).l
	btst	#0,*(pc)
	btst	#0,*(pc,d0.w)

	bchg.l	d0,d0
	bchg.b	d0,(a0)
	bchg.b	d0,(a0)+
	bchg.b	d0,-(a0)
	bchg.b	d0,$7FFF(a0)
	bchg.b	d0,$7F(a0,d0.w)
	bchg.b	d0,($FFFFFFFF).w
	bchg.b	d0,($FFFFFFFF).l

	bchg	d0,d0
	bchg	d0,(a0)
	bchg	d0,(a0)+
	bchg	d0,-(a0)
	bchg	d0,$7FFF(a0)
	bchg	d0,$7F(a0,d0.w)
	bchg	d0,($FFFFFFFF).w
	bchg	d0,($FFFFFFFF).l

	bchg.l	#0,d0
	bchg.b	#0,(a0)
	bchg.b	#0,(a0)+
	bchg.b	#0,-(a0)
	bchg.b	#0,$7FFF(a0)
	bchg.b	#0,$7F(a0,d0.w)
	bchg.b	#0,($FFFFFFFF).w
	bchg.b	#0,($FFFFFFFF).l

	bchg	#0,d0
	bchg	#0,(a0)
	bchg	#0,(a0)+
	bchg	#0,-(a0)
	bchg	#0,$7FFF(a0)
	bchg	#0,$7F(a0,d0.w)
	bchg	#0,($FFFFFFFF).w
	bchg	#0,($FFFFFFFF).l

	bclr.l	d0,d0
	bclr.b	d0,(a0)
	bclr.b	d0,(a0)+
	bclr.b	d0,-(a0)
	bclr.b	d0,$7FFF(a0)
	bclr.b	d0,$7F(a0,d0.w)
	bclr.b	d0,($FFFFFFFF).w
	bclr.b	d0,($FFFFFFFF).l

	bclr	d0,d0
	bclr	d0,(a0)
	bclr	d0,(a0)+
	bclr	d0,-(a0)
	bclr	d0,$7FFF(a0)
	bclr	d0,$7F(a0,d0.w)
	bclr	d0,($FFFFFFFF).w
	bclr	d0,($FFFFFFFF).l

	bclr.l	#0,d0
	bclr.b	#0,(a0)
	bclr.b	#0,(a0)+
	bclr.b	#0,-(a0)
	bclr.b	#0,$7FFF(a0)
	bclr.b	#0,$7F(a0,d0.w)
	bclr.b	#0,($FFFFFFFF).w
	bclr.b	#0,($FFFFFFFF).l

	bclr	#0,d0
	bclr	#0,(a0)
	bclr	#0,(a0)+
	bclr	#0,-(a0)
	bclr	#0,$7FFF(a0)
	bclr	#0,$7F(a0,d0.w)
	bclr	#0,($FFFFFFFF).w
	bclr	#0,($FFFFFFFF).l

	bset.l	d0,d0
	bset.b	d0,(a0)
	bset.b	d0,(a0)+
	bset.b	d0,-(a0)
	bset.b	d0,$7FFF(a0)
	bset.b	d0,$7F(a0,d0.w)
	bset.b	d0,($FFFFFFFF).w
	bset.b	d0,($FFFFFFFF).l

	bset	d0,d0
	bset	d0,(a0)
	bset	d0,(a0)+
	bset	d0,-(a0)
	bset	d0,$7FFF(a0)
	bset	d0,$7F(a0,d0.w)
	bset	d0,($FFFFFFFF).w
	bset	d0,($FFFFFFFF).l

	bset.l	#0,d0
	bset.b	#0,(a0)
	bset.b	#0,(a0)+
	bset.b	#0,-(a0)
	bset.b	#0,$7FFF(a0)
	bset.b	#0,$7F(a0,d0.w)
	bset.b	#0,($FFFFFFFF).w
	bset.b	#0,($FFFFFFFF).l

	bset	#0,d0
	bset	#0,(a0)
	bset	#0,(a0)+
	bset	#0,-(a0)
	bset	#0,$7FFF(a0)
	bset	#0,$7F(a0,d0.w)
	bset	#0,($FFFFFFFF).w
	bset	#0,($FFFFFFFF).l

	movep.w	$7FFF(a0),d0
	movep.w	d0,$7FFF(a0)
	movep.l	$7FFF(a0),d0
	movep.l	d0,$7FFF(a0)
		
	movea.w	d0,a0
	movea.w	a0,a0
	movea.w	(a0),a0
	movea.w	(a0)+,a0
	movea.w	-(a0),a0
	movea.w	$7FFF(a0),a0
	movea.w	$7F(a0,d0.w),a0
	movea.w	($FFFFFFFF).w,a0
	movea.w	($FFFFFFFF).l,a0
	movea.w	*(pc),a0
	movea.w	*(pc,d0.w),a0
	movea.l	d0,a0
	movea.l	a0,a0
	movea.l	(a0),a0
	movea.l	(a0)+,a0
	movea.l	-(a0),a0
	movea.l	$7FFF(a0),a0
	movea.l	$7F(a0,d0.w),a0
	movea.l	($FFFFFFFF).w,a0
	movea.l	($FFFFFFFF).l,a0
	movea.l	*(pc),a0
	movea.l	*(pc,d0.w),a0

	move.b	d0,d0
	move.b	d0,(a0)
	move.b	d0,(a0)+
	move.b	d0,-(a0)
	move.b	d0,$7FFF(a0)
	move.b	d0,$7F(a0,d0.w)
	move.b	d0,($FFFFFFFF).w
	move.b	d0,($FFFFFFFF).l
	move.b	(a0),d0
	move.b	(a0),(a0)
	move.b	(a0),(a0)+
	move.b	(a0),-(a0)
	move.b	(a0),$7FFF(a0)
	move.b	(a0),$7F(a0,d0.w)
	move.b	(a0),($FFFFFFFF).w
	move.b	(a0),($FFFFFFFF).l
	move.b	(a0)+,d0
	move.b	(a0)+,(a0)
	move.b	(a0)+,(a0)+
	move.b	(a0)+,-(a0)
	move.b	(a0)+,$7FFF(a0)
	move.b	(a0)+,$7F(a0,d0.w)
	move.b	(a0)+,($FFFFFFFF).w
	move.b	(a0)+,($FFFFFFFF).l
	move.b	-(a0),d0
	move.b	-(a0),(a0)
	move.b	-(a0),(a0)+
	move.b	-(a0),-(a0)
	move.b	-(a0),$7FFF(a0)
	move.b	-(a0),$7F(a0,d0.w)
	move.b	-(a0),($FFFFFFFF).w
	move.b	-(a0),($FFFFFFFF).l
	move.b	$7FFF(a0),d0
	move.b	$7FFF(a0),(a0)
	move.b	$7FFF(a0),(a0)+
	move.b	$7FFF(a0),-(a0)
	move.b	$7FFF(a0),$7FFF(a0)
	move.b	$7FFF(a0),$7F(a0,d0.w)
	move.b	$7FFF(a0),($FFFFFFFF).w
	move.b	$7FFF(a0),($FFFFFFFF).l
	move.b	$7F(a0,d0.w),d0
	move.b	$7F(a0,d0.w),(a0)
	move.b	$7F(a0,d0.w),(a0)+
	move.b	$7F(a0,d0.w),-(a0)
	move.b	$7F(a0,d0.w),$7FFF(a0)
	move.b	$7F(a0,d0.w),$7F(a0,d0.w)
	move.b	$7F(a0,d0.w),($FFFFFFFF).w
	move.b	$7F(a0,d0.w),($FFFFFFFF).l
	move.b	($FFFFFFFF).w,d0
	move.b	($FFFFFFFF).w,(a0)
	move.b	($FFFFFFFF).w,(a0)+
	move.b	($FFFFFFFF).w,-(a0)
	move.b	($FFFFFFFF).w,$7FFF(a0)
	move.b	($FFFFFFFF).w,$7F(a0,d0.w)
	move.b	($FFFFFFFF).w,($FFFFFFFF).w
	move.b	($FFFFFFFF).w,($FFFFFFFF).l
	move.b	($FFFFFFFF).l,d0
	move.b	($FFFFFFFF).l,(a0)
	move.b	($FFFFFFFF).l,(a0)+
	move.b	($FFFFFFFF).l,-(a0)
	move.b	($FFFFFFFF).l,$7FFF(a0)
	move.b	($FFFFFFFF).l,$7F(a0,d0.w)
	move.b	($FFFFFFFF).l,($FFFFFFFF).w
	move.b	($FFFFFFFF).l,($FFFFFFFF).l
	move.b	#$FF,d0
	move.b	#$FF,(a0)
	move.b	#$FF,(a0)+
	move.b	#$FF,-(a0)
	move.b	#$FF,$7FFF(a0)
	move.b	#$FF,$7F(a0,d0.w)
	move.b	#$FF,($FFFFFFFF).w
	move.b	#$FF,($FFFFFFFF).l
	move.b	*(pc),d0
	move.b	*(pc),(a0)
	move.b	*(pc),(a0)+
	move.b	*(pc),-(a0)
	move.b	*(pc),$7FFF(a0)
	move.b	*(pc),$7F(a0,d0.w)
	move.b	*(pc),($FFFFFFFF).w
	move.b	*(pc),($FFFFFFFF).l
	move.b	*(pc,d0.w),d0
	move.b	*(pc,d0.w),(a0)
	move.b	*(pc,d0.w),(a0)+
	move.b	*(pc,d0.w),-(a0)
	move.b	*(pc,d0.w),$7FFF(a0)
	move.b	*(pc,d0.w),$7F(a0,d0.w)
	move.b	*(pc,d0.w),($FFFFFFFF).w
	move.b	*(pc,d0.w),($FFFFFFFF).l
	move.w	d0,d0
	move.w	d0,(a0)
	move.w	d0,(a0)+
	move.w	d0,-(a0)
	move.w	d0,$7FFF(a0)
	move.w	d0,$7F(a0,d0.w)
	move.w	d0,($FFFFFFFF).w
	move.w	d0,($FFFFFFFF).l
	move.w	a0,d0
	move.w	a0,(a0)
	move.w	a0,(a0)+
	move.w	a0,-(a0)
	move.w	a0,$7FFF(a0)
	move.w	a0,$7F(a0,d0.w)
	move.w	a0,($FFFFFFFF).w
	move.w	a0,($FFFFFFFF).l
	move.w	(a0),d0
	move.w	(a0),(a0)
	move.w	(a0),(a0)+
	move.w	(a0),-(a0)
	move.w	(a0),$7FFF(a0)
	move.w	(a0),$7F(a0,d0.w)
	move.w	(a0),($FFFFFFFF).w
	move.w	(a0),($FFFFFFFF).l
	move.w	(a0)+,d0
	move.w	(a0)+,(a0)
	move.w	(a0)+,(a0)+
	move.w	(a0)+,-(a0)
	move.w	(a0)+,$7FFF(a0)
	move.w	(a0)+,$7F(a0,d0.w)
	move.w	(a0)+,($FFFFFFFF).w
	move.w	(a0)+,($FFFFFFFF).l
	move.w	-(a0),d0
	move.w	-(a0),(a0)
	move.w	-(a0),(a0)+
	move.w	-(a0),-(a0)
	move.w	-(a0),$7FFF(a0)
	move.w	-(a0),$7F(a0,d0.w)
	move.w	-(a0),($FFFFFFFF).w
	move.w	-(a0),($FFFFFFFF).l
	move.w	$7FFF(a0),d0
	move.w	$7FFF(a0),(a0)
	move.w	$7FFF(a0),(a0)+
	move.w	$7FFF(a0),-(a0)
	move.w	$7FFF(a0),$7FFF(a0)
	move.w	$7FFF(a0),$7F(a0,d0.w)
	move.w	$7FFF(a0),($FFFFFFFF).w
	move.w	$7FFF(a0),($FFFFFFFF).l
	move.w	$7F(a0,d0.w),d0
	move.w	$7F(a0,d0.w),(a0)
	move.w	$7F(a0,d0.w),(a0)+
	move.w	$7F(a0,d0.w),-(a0)
	move.w	$7F(a0,d0.w),$7FFF(a0)
	move.w	$7F(a0,d0.w),$7F(a0,d0.w)
	move.w	$7F(a0,d0.w),($FFFFFFFF).w
	move.w	$7F(a0,d0.w),($FFFFFFFF).l
	move.w	($FFFFFFFF).w,d0
	move.w	($FFFFFFFF).w,(a0)
	move.w	($FFFFFFFF).w,(a0)+
	move.w	($FFFFFFFF).w,-(a0)
	move.w	($FFFFFFFF).w,$7FFF(a0)
	move.w	($FFFFFFFF).w,$7F(a0,d0.w)
	move.w	($FFFFFFFF).w,($FFFFFFFF).w
	move.w	($FFFFFFFF).w,($FFFFFFFF).l
	move.w	($FFFFFFFF).l,d0
	move.w	($FFFFFFFF).l,(a0)
	move.w	($FFFFFFFF).l,(a0)+
	move.w	($FFFFFFFF).l,-(a0)
	move.w	($FFFFFFFF).l,$7FFF(a0)
	move.w	($FFFFFFFF).l,$7F(a0,d0.w)
	move.w	($FFFFFFFF).l,($FFFFFFFF).w
	move.w	($FFFFFFFF).l,($FFFFFFFF).l
	move.w	#$FFFF,d0
	move.w	#$FFFF,(a0)
	move.w	#$FFFF,(a0)+
	move.w	#$FFFF,-(a0)
	move.w	#$FFFF,$7FFF(a0)
	move.w	#$FFFF,$7F(a0,d0.w)
	move.w	#$FFFF,($FFFFFFFF).w
	move.w	#$FF,($FFFFFFFF).l
	move.w	*(pc),d0
	move.w	*(pc),(a0)
	move.w	*(pc),(a0)+
	move.w	*(pc),-(a0)
	move.w	*(pc),$7FFF(a0)
	move.w	*(pc),$7F(a0,d0.w)
	move.w	*(pc),($FFFFFFFF).w
	move.w	*(pc),($FFFFFFFF).l
	move.w	*(pc,d0.w),d0
	move.w	*(pc,d0.w),(a0)
	move.w	*(pc,d0.w),(a0)+
	move.w	*(pc,d0.w),-(a0)
	move.w	*(pc,d0.w),$7FFF(a0)
	move.w	*(pc,d0.w),$7F(a0,d0.w)
	move.w	*(pc,d0.w),($FFFFFFFF).w
	move.w	*(pc,d0.w),($FFFFFFFF).l
	move.l	d0,d0
	move.l	d0,(a0)
	move.l	d0,(a0)+
	move.l	d0,-(a0)
	move.l	d0,$7FFF(a0)
	move.l	d0,$7F(a0,d0.w)
	move.l	d0,($FFFFFFFF).w
	move.l	d0,($FFFFFFFF).l
	move.l	a0,d0
	move.l	a0,(a0)
	move.l	a0,(a0)+
	move.l	a0,-(a0)
	move.l	a0,$7FFF(a0)
	move.l	a0,$7F(a0,d0.w)
	move.l	a0,($FFFFFFFF).w
	move.l	a0,($FFFFFFFF).l
	move.l	(a0),d0
	move.l	(a0),(a0)
	move.l	(a0),(a0)+
	move.l	(a0),-(a0)
	move.l	(a0),$7FFF(a0)
	move.l	(a0),$7F(a0,d0.w)
	move.l	(a0),($FFFFFFFF).w
	move.l	(a0),($FFFFFFFF).l
	move.l	(a0)+,d0
	move.l	(a0)+,(a0)
	move.l	(a0)+,(a0)+
	move.l	(a0)+,-(a0)
	move.l	(a0)+,$7FFF(a0)
	move.l	(a0)+,$7F(a0,d0.w)
	move.l	(a0)+,($FFFFFFFF).w
	move.l	(a0)+,($FFFFFFFF).l
	move.l	-(a0),d0
	move.l	-(a0),(a0)
	move.l	-(a0),(a0)+
	move.l	-(a0),-(a0)
	move.l	-(a0),$7FFF(a0)
	move.l	-(a0),$7F(a0,d0.w)
	move.l	-(a0),($FFFFFFFF).w
	move.l	-(a0),($FFFFFFFF).l
	move.l	$7FFF(a0),d0
	move.l	$7FFF(a0),(a0)
	move.l	$7FFF(a0),(a0)+
	move.l	$7FFF(a0),-(a0)
	move.l	$7FFF(a0),$7FFF(a0)
	move.l	$7FFF(a0),$7F(a0,d0.w)
	move.l	$7FFF(a0),($FFFFFFFF).w
	move.l	$7FFF(a0),($FFFFFFFF).l
	move.l	$7F(a0,d0.w),d0
	move.l	$7F(a0,d0.w),(a0)
	move.l	$7F(a0,d0.w),(a0)+
	move.l	$7F(a0,d0.w),-(a0)
	move.l	$7F(a0,d0.w),$7FFF(a0)
	move.l	$7F(a0,d0.w),$7F(a0,d0.w)
	move.l	$7F(a0,d0.w),($FFFFFFFF).w
	move.l	$7F(a0,d0.w),($FFFFFFFF).l
	move.l	($FFFFFFFF).w,d0
	move.l	($FFFFFFFF).w,(a0)
	move.l	($FFFFFFFF).w,(a0)+
	move.l	($FFFFFFFF).w,-(a0)
	move.l	($FFFFFFFF).w,$7FFF(a0)
	move.l	($FFFFFFFF).w,$7F(a0,d0.w)
	move.l	($FFFFFFFF).w,($FFFFFFFF).w
	move.l	($FFFFFFFF).w,($FFFFFFFF).l
	move.l	($FFFFFFFF).l,d0
	move.l	($FFFFFFFF).l,(a0)
	move.l	($FFFFFFFF).l,(a0)+
	move.l	($FFFFFFFF).l,-(a0)
	move.l	($FFFFFFFF).l,$7FFF(a0)
	move.l	($FFFFFFFF).l,$7F(a0,d0.w)
	move.l	($FFFFFFFF).l,($FFFFFFFF).w
	move.l	($FFFFFFFF).l,($FFFFFFFF).l
	move.l	#$FFFFFFFF,d0
	move.l	#$FFFFFFFF,(a0)
	move.l	#$FFFFFFFF,(a0)+
	move.l	#$FFFFFFFF,-(a0)
	move.l	#$FFFFFFFF,$7FFF(a0)
	move.l	#$FFFFFFFF,$7F(a0,d0.w)
	move.l	#$FFFFFFFF,($FFFFFFFF).w
	move.l	#$FFFFFFFF,($FFFFFFFF).l
	move.l	*(pc),d0
	move.l	*(pc),(a0)
	move.l	*(pc),(a0)+
	move.l	*(pc),-(a0)
	move.l	*(pc),$7FFF(a0)
	move.l	*(pc),$7F(a0,d0.w)
	move.l	*(pc),($FFFFFFFF).w
	move.l	*(pc),($FFFFFFFF).l
	move.l	*(pc,d0.w),d0
	move.l	*(pc,d0.w),(a0)
	move.l	*(pc,d0.w),(a0)+
	move.l	*(pc,d0.w),-(a0)
	move.l	*(pc,d0.w),$7FFF(a0)
	move.l	*(pc,d0.w),$7F(a0,d0.w)
	move.l	*(pc,d0.w),($FFFFFFFF).w
	move.l	*(pc,d0.w),($FFFFFFFF).l
	move.w	sr,d0
	move.w	sr,(a0)
	move.w	sr,(a0)+
	move.w	sr,-(a0)
	move.w	sr,$7FFF(a0)
	move.w	sr,$7F(a0,d0.w)
	move.w	sr,($FFFFFFFF).w
	move.w	sr,($FFFFFFFF).l
	move.w	d0,ccr
	move.w	(a0),ccr
	move.w	(a0)+,ccr
	move.w	-(a0),ccr
	move.w	$7FFF(a0),ccr
	move.w	$7F(a0,d0.w),ccr
	move.w	($FFFFFFFF).w,ccr
	move.w	($FFFFFFFF).l,ccr
	move.w	#$FFFF,ccr
	move.w	*(pc),ccr
	move.w	*(pc,d0.w),ccr
	move.w	d0,sr
	move.w	(a0),sr
	move.w	(a0)+,sr
	move.w	-(a0),sr
	move.w	$7FFF(a0),sr
	move.w	$7F(a0,d0.w),sr
	move.w	($FFFFFFFF).w,sr
	move.w	($FFFFFFFF).l,sr
	move.w	#$FFFF,sr
	move.w	*(pc),sr
	move.w	*(pc,d0.w),sr

	negx.b	d0
	negx.b	(a0)
	negx.b	(a0)+
	negx.b	-(a0)
	negx.b	$7FFF(a0)
	negx.b	$7F(a0,d0.w)
	negx.b	($FFFFFFFF).w
	negx.b	($FFFFFFFF).l
	negx.w	d0
	negx.w	(a0)
	negx.w	(a0)+
	negx.w	-(a0)
	negx.w	$7FFF(a0)
	negx.w	$7F(a0,d0.w)
	negx.w	($FFFFFFFF).w
	negx.w	($FFFFFFFF).l
	negx.l	d0
	negx.l	(a0)
	negx.l	(a0)+
	negx.l	-(a0)
	negx.l	$7FFF(a0)
	negx.l	$7F(a0,d0.w)
	negx.l	($FFFFFFFF).w
	negx.l	($FFFFFFFF).l

	clr.b	d0
	clr.b	(a0)
	clr.b	(a0)+
	clr.b	-(a0)
	clr.b	$7FFF(a0)
	clr.b	$7F(a0,d0.w)
	clr.b	($FFFFFFFF).w
	clr.b	($FFFFFFFF).l
	clr.w	d0
	clr.w	(a0)
	clr.w	(a0)+
	clr.w	-(a0)
	clr.w	$7FFF(a0)
	clr.w	$7F(a0,d0.w)
	clr.w	($FFFFFFFF).w
	clr.w	($FFFFFFFF).l
	clr.l	d0
	clr.l	(a0)
	clr.l	(a0)+
	clr.l	-(a0)
	clr.l	$7FFF(a0)
	clr.l	$7F(a0,d0.w)
	clr.l	($FFFFFFFF).w
	clr.l	($FFFFFFFF).l

	neg.b	d0
	neg.b	(a0)
	neg.b	(a0)+
	neg.b	-(a0)
	neg.b	$7FFF(a0)
	neg.b	$7F(a0,d0.w)
	neg.b	($FFFFFFFF).w
	neg.b	($FFFFFFFF).l
	neg.w	d0
	neg.w	(a0)
	neg.w	(a0)+
	neg.w	-(a0)
	neg.w	$7FFF(a0)
	neg.w	$7F(a0,d0.w)
	neg.w	($FFFFFFFF).w
	neg.w	($FFFFFFFF).l
	neg.l	d0
	neg.l	(a0)
	neg.l	(a0)+
	neg.l	-(a0)
	neg.l	$7FFF(a0)
	neg.l	$7F(a0,d0.w)
	neg.l	($FFFFFFFF).w
	neg.l	($FFFFFFFF).l

	not.b	d0
	not.b	(a0)
	not.b	(a0)+
	not.b	-(a0)
	not.b	$7FFF(a0)
	not.b	$7F(a0,d0.w)
	not.b	($FFFFFFFF).w
	not.b	($FFFFFFFF).l
	not.w	d0
	not.w	(a0)
	not.w	(a0)+
	not.w	-(a0)
	not.w	$7FFF(a0)
	not.w	$7F(a0,d0.w)
	not.w	($FFFFFFFF).w
	not.w	($FFFFFFFF).l
	not.l	d0
	not.l	(a0)
	not.l	(a0)+
	not.l	-(a0)
	not.l	$7FFF(a0)
	not.l	$7F(a0,d0.w)
	not.l	($FFFFFFFF).w
	not.l	($FFFFFFFF).l

	ext.w	d0
	ext.l	d0

	nbcd.b	d0
	nbcd.b	(a0)
	nbcd.b	(a0)+
	nbcd.b	-(a0)
	nbcd.b	$7FFF(a0)
	nbcd.b	$7F(a0,d0.w)
	nbcd.b	($FFFFFFFF).w
	nbcd.b	($FFFFFFFF).l
	nbcd	d0
	nbcd	(a0)
	nbcd	(a0)+
	nbcd	-(a0)
	nbcd	$7FFF(a0)
	nbcd	$7F(a0,d0.w)
	nbcd	($FFFFFFFF).w
	nbcd	($FFFFFFFF).l

	swap.w	d0
	swap	d0

	pea.l	(a0)
	pea.l	$7FFF(a0)
	pea.l	$7F(a0,d0.w)
	pea.l	($FFFFFFFF).w
	pea.l	($FFFFFFFF).l
	pea.l	*(pc)
	pea.l	*(pc,d0.w)
	pea	(a0)
	pea	$7FFF(a0)
	pea	$7F(a0,d0.w)
	pea	($FFFFFFFF).w
	pea	($FFFFFFFF).l
	pea	*(pc)
	pea	*(pc,d0.w)

	illegal

	tas.b	d0
	tas.b	(a0)
	tas.b	(a0)+
	tas.b	-(a0)
	tas.b	$7FFF(a0)
	tas.b	$7F(a0,d0.w)
	tas.b	($FFFFFFFF).w
	tas.b	($FFFFFFFF).l
	tas	d0
	tas	(a0)
	tas	(a0)+
	tas	-(a0)
	tas	$7FFF(a0)
	tas	$7F(a0,d0.w)
	tas	($FFFFFFFF).w
	tas	($FFFFFFFF).l

	tst.b	d0
	tst.b	(a0)
	tst.b	(a0)+
	tst.b	-(a0)
	tst.b	$7FFF(a0)
	tst.b	$7F(a0,d0.w)
	tst.b	($FFFFFFFF).w
	tst.b	($FFFFFFFF).l
	tst.w	d0
	tst.w	(a0)
	tst.w	(a0)+
	tst.w	-(a0)
	tst.w	$7FFF(a0)
	tst.w	$7F(a0,d0.w)
	tst.w	($FFFFFFFF).w
	tst.w	($FFFFFFFF).l
	tst.l	d0
	tst.l	(a0)
	tst.l	(a0)+
	tst.l	-(a0)
	tst.l	$7FFF(a0)
	tst.l	$7F(a0,d0.w)
	tst.l	($FFFFFFFF).w
	tst.l	($FFFFFFFF).l

	trap	#2

	link.w	a0,#4
	link	a0,#4

	unlk	a0

	move.l	usp,a0
	move.l	a0,usp
	move	usp,a0
	move	a0,usp

	reset

	nop

	stop	#$2700

	rte

	rts

	trapv

	rtr

	jsr	(a0)
	jsr	$7FFF(a0)
	jsr	$7F(a0,d0.w)
	jsr	($FFFFFFFF).w
	jsr	($FFFFFFFF).l
	jsr	*(pc)
	jsr	*(pc,d0.w)

	jmp	(a0)
	jmp	$7FFF(a0)
	jmp	$7F(a0,d0.w)
	jmp	($FFFFFFFF).w
	jmp	($FFFFFFFF).l
	jmp	*(pc)
	jmp	*(pc,d0.w)

	movem.w	d0-a7,(a0)
	movem.w	d0-a7,-(a0)
	movem.w	d0-a7,$7FFF(a0)
	movem.w	d0-a7,$7F(a0,d0.w)
	movem.w	d0-a7,($FFFFFFFF).w
	movem.w	d0-a7,($FFFFFFFF).l
	movem.w	(a0),d0-a7
	movem.w	(a0)+,d0-a7
	movem.w	$7FFF(a0),d0-a7
	movem.w	$7F(a0,d0.w),d0-a7
	movem.w	($FFFFFFFF).w,d0-a7
	movem.w	($FFFFFFFF).l,d0-a7
	movem.w	*(pc),d0-a7
	movem.w	*(pc,d0.w),d0-a7
	movem.l	d0-a7,(a0)
	movem.l	d0-a7,-(a0)
	movem.l	d0-a7,$7FFF(a0)
	movem.l	d0-a7,$7F(a0,d0.w)
	movem.l	d0-a7,($FFFFFFFF).w
	movem.l	d0-a7,($FFFFFFFF).l
	movem.l	(a0),d0-a7
	movem.l	(a0)+,d0-a7
	movem.l	$7FFF(a0),d0-a7
	movem.l	$7F(a0,d0.w),d0-a7
	movem.l	($FFFFFFFF).w,d0-a7
	movem.l	($FFFFFFFF).l,d0-a7
	movem.l	*(pc),d0-a7
	movem.l	*(pc,d0.w),d0-a7

	lea.l	(a0),a0
	lea.l	$7FFF(a0),a0
	lea.l	$7F(a0,d0.w),a0
	lea.l	($FFFFFFFF).w,a0
	lea.l	($FFFFFFFF).l,a0
	lea.l	*(pc),a0
	lea.l	*(pc,d0.w),a0
	lea	(a0),a0
	lea	$7FFF(a0),a0
	lea	$7F(a0,d0.w),a0
	lea	($FFFFFFFF).w,a0
	lea	($FFFFFFFF).l,a0
	lea	*(pc),a0
	lea	*(pc,d0.w),a0

	chk.w	d0,d0
	chk.w	(a0),d0
	chk.w	(a0)+,d0
	chk.w	-(a0),d0
	chk.w	$7FFF(a0),d0
	chk.w	$7F(a0,d0.w),d0
	chk.w	($FFFFFFFF).w,d0
	chk.w	($FFFFFFFF).l,d0
	chk.w	#$FFFF,d0
	chk.w	*(pc),d0
	chk.w	*(pc,d0.w),d0
	chk	d0,d0
	chk	(a0),d0
	chk	(a0)+,d0
	chk	-(a0),d0
	chk	$7FFF(a0),d0
	chk	$7F(a0,d0.w),d0
	chk	($FFFFFFFF).w,d0
	chk	($FFFFFFFF).l,d0
	chk	#$FFFF,d0
	chk	*(pc),d0
	chk	*(pc,d0.w),d0

	addq.b	#1,d0
	addq.b	#1,(a0)
	addq.b	#1,(a0)+
	addq.b	#1,-(a0)
	addq.b	#1,$7FFF(a0)
	addq.b	#1,$7F(a0,d0.w)
	addq.b	#1,($FFFFFFFF).w
	addq.b	#1,($FFFFFFFF).l
	addq.w	#1,d0
	addq.w	#1,a0
	addq.w	#1,(a0)
	addq.w	#1,(a0)+
	addq.w	#1,-(a0)
	addq.w	#1,$7FFF(a0)
	addq.w	#1,$7F(a0,d0.w)
	addq.w	#1,($FFFFFFFF).w
	addq.w	#1,($FFFFFFFF).l
	addq.l	#1,d0
	addq.l	#1,a0
	addq.l	#1,(a0)
	addq.l	#1,(a0)+
	addq.l	#1,-(a0)
	addq.l	#1,$7FFF(a0)
	addq.l	#1,$7F(a0,d0.w)
	addq.l	#1,($FFFFFFFF).w
	addq.l	#1,($FFFFFFFF).l

	subq.b	#1,d0
	subq.b	#1,(a0)
	subq.b	#1,(a0)+
	subq.b	#1,-(a0)
	subq.b	#1,$7FFF(a0)
	subq.b	#1,$7F(a0,d0.w)
	subq.b	#1,($FFFFFFFF).w
	subq.b	#1,($FFFFFFFF).l
	subq.w	#1,d0
	subq.w	#1,a0
	subq.w	#1,(a0)
	subq.w	#1,(a0)+
	subq.w	#1,-(a0)
	subq.w	#1,$7FFF(a0)
	subq.w	#1,$7F(a0,d0.w)
	subq.w	#1,($FFFFFFFF).w
	subq.w	#1,($FFFFFFFF).l
	subq.l	#1,d0
	subq.l	#1,a0
	subq.l	#1,(a0)
	subq.l	#1,(a0)+
	subq.l	#1,-(a0)
	subq.l	#1,$7FFF(a0)
	subq.l	#1,$7F(a0,d0.w)
	subq.l	#1,($FFFFFFFF).w
	subq.l	#1,($FFFFFFFF).l

	st.b	d0
	st.b	(a0)
	st.b	(a0)+
	st.b	-(a0)
	st.b	$7FFF(a0)
	st.b	$7F(a0,d0.w)
	st.b	($FFFFFFFF).w
	st.b	($FFFFFFFF).l
	st	d0
	st	(a0)
	st	(a0)+
	st	-(a0)
	st	$7FFF(a0)
	st	$7F(a0,d0.w)
	st	($FFFFFFFF).w
	st	($FFFFFFFF).l

	sf.b	d0
	sf.b	(a0)
	sf.b	(a0)+
	sf.b	-(a0)
	sf.b	$7FFF(a0)
	sf.b	$7F(a0,d0.w)
	sf.b	($FFFFFFFF).w
	sf.b	($FFFFFFFF).l
	sf	d0
	sf	(a0)
	sf	(a0)+
	sf	-(a0)
	sf	$7FFF(a0)
	sf	$7F(a0,d0.w)
	sf	($FFFFFFFF).w
	sf	($FFFFFFFF).l

	shi.b	d0
	shi.b	(a0)
	shi.b	(a0)+
	shi.b	-(a0)
	shi.b	$7FFF(a0)
	shi.b	$7F(a0,d0.w)
	shi.b	($FFFFFFFF).w
	shi.b	($FFFFFFFF).l
	shi	d0
	shi	(a0)
	shi	(a0)+
	shi	-(a0)
	shi	$7FFF(a0)
	shi	$7F(a0,d0.w)
	shi	($FFFFFFFF).w
	shi	($FFFFFFFF).l

	sls.b	d0
	sls.b	(a0)
	sls.b	(a0)+
	sls.b	-(a0)
	sls.b	$7FFF(a0)
	sls.b	$7F(a0,d0.w)
	sls.b	($FFFFFFFF).w
	sls.b	($FFFFFFFF).l
	sls	d0
	sls	(a0)
	sls	(a0)+
	sls	-(a0)
	sls	$7FFF(a0)
	sls	$7F(a0,d0.w)
	sls	($FFFFFFFF).w
	sls	($FFFFFFFF).l

	scc.b	d0
	scc.b	(a0)
	scc.b	(a0)+
	scc.b	-(a0)
	scc.b	$7FFF(a0)
	scc.b	$7F(a0,d0.w)
	scc.b	($FFFFFFFF).w
	scc.b	($FFFFFFFF).l
	scc	d0
	scc	(a0)
	scc	(a0)+
	scc	-(a0)
	scc	$7FFF(a0)
	scc	$7F(a0,d0.w)
	scc	($FFFFFFFF).w
	scc	($FFFFFFFF).l

	scs.b	d0
	scs.b	(a0)
	scs.b	(a0)+
	scs.b	-(a0)
	scs.b	$7FFF(a0)
	scs.b	$7F(a0,d0.w)
	scs.b	($FFFFFFFF).w
	scs.b	($FFFFFFFF).l
	scs	d0
	scs	(a0)
	scs	(a0)+
	scs	-(a0)
	scs	$7FFF(a0)
	scs	$7F(a0,d0.w)
	scs	($FFFFFFFF).w
	scs	($FFFFFFFF).l

	sne.b	d0
	sne.b	(a0)
	sne.b	(a0)+
	sne.b	-(a0)
	sne.b	$7FFF(a0)
	sne.b	$7F(a0,d0.w)
	sne.b	($FFFFFFFF).w
	sne.b	($FFFFFFFF).l
	sne	d0
	sne	(a0)
	sne	(a0)+
	sne	-(a0)
	sne	$7FFF(a0)
	sne	$7F(a0,d0.w)
	sne	($FFFFFFFF).w
	sne	($FFFFFFFF).l

	seq.b	d0
	seq.b	(a0)
	seq.b	(a0)+
	seq.b	-(a0)
	seq.b	$7FFF(a0)
	seq.b	$7F(a0,d0.w)
	seq.b	($FFFFFFFF).w
	seq.b	($FFFFFFFF).l
	seq	d0
	seq	(a0)
	seq	(a0)+
	seq	-(a0)
	seq	$7FFF(a0)
	seq	$7F(a0,d0.w)
	seq	($FFFFFFFF).w
	seq	($FFFFFFFF).l

	svc.b	d0
	svc.b	(a0)
	svc.b	(a0)+
	svc.b	-(a0)
	svc.b	$7FFF(a0)
	svc.b	$7F(a0,d0.w)
	svc.b	($FFFFFFFF).w
	svc.b	($FFFFFFFF).l
	svc	d0
	svc	(a0)
	svc	(a0)+
	svc	-(a0)
	svc	$7FFF(a0)
	svc	$7F(a0,d0.w)
	svc	($FFFFFFFF).w
	svc	($FFFFFFFF).l

	svs.b	d0
	svs.b	(a0)
	svs.b	(a0)+
	svs.b	-(a0)
	svs.b	$7FFF(a0)
	svs.b	$7F(a0,d0.w)
	svs.b	($FFFFFFFF).w
	svs.b	($FFFFFFFF).l
	svs	d0
	svs	(a0)
	svs	(a0)+
	svs	-(a0)
	svs	$7FFF(a0)
	svs	$7F(a0,d0.w)
	svs	($FFFFFFFF).w
	svs	($FFFFFFFF).l

	spl.b	d0
	spl.b	(a0)
	spl.b	(a0)+
	spl.b	-(a0)
	spl.b	$7FFF(a0)
	spl.b	$7F(a0,d0.w)
	spl.b	($FFFFFFFF).w
	spl.b	($FFFFFFFF).l
	spl	d0
	spl	(a0)
	spl	(a0)+
	spl	-(a0)
	spl	$7FFF(a0)
	spl	$7F(a0,d0.w)
	spl	($FFFFFFFF).w
	spl	($FFFFFFFF).l

	smi.b	d0
	smi.b	(a0)
	smi.b	(a0)+
	smi.b	-(a0)
	smi.b	$7FFF(a0)
	smi.b	$7F(a0,d0.w)
	smi.b	($FFFFFFFF).w
	smi.b	($FFFFFFFF).l
	smi	d0
	smi	(a0)
	smi	(a0)+
	smi	-(a0)
	smi	$7FFF(a0)
	smi	$7F(a0,d0.w)
	smi	($FFFFFFFF).w
	smi	($FFFFFFFF).l

	sge.b	d0
	sge.b	(a0)
	sge.b	(a0)+
	sge.b	-(a0)
	sge.b	$7FFF(a0)
	sge.b	$7F(a0,d0.w)
	sge.b	($FFFFFFFF).w
	sge.b	($FFFFFFFF).l
	sge	d0
	sge	(a0)
	sge	(a0)+
	sge	-(a0)
	sge	$7FFF(a0)
	sge	$7F(a0,d0.w)
	sge	($FFFFFFFF).w
	sge	($FFFFFFFF).l

	slt.b	d0
	slt.b	(a0)
	slt.b	(a0)+
	slt.b	-(a0)
	slt.b	$7FFF(a0)
	slt.b	$7F(a0,d0.w)
	slt.b	($FFFFFFFF).w
	slt.b	($FFFFFFFF).l
	slt	d0
	slt	(a0)
	slt	(a0)+
	slt	-(a0)
	slt	$7FFF(a0)
	slt	$7F(a0,d0.w)
	slt	($FFFFFFFF).w
	slt	($FFFFFFFF).l

	sgt.b	d0
	sgt.b	(a0)
	sgt.b	(a0)+
	sgt.b	-(a0)
	sgt.b	$7FFF(a0)
	sgt.b	$7F(a0,d0.w)
	sgt.b	($FFFFFFFF).w
	sgt.b	($FFFFFFFF).l
	sgt	d0
	sgt	(a0)
	sgt	(a0)+
	sgt	-(a0)
	sgt	$7FFF(a0)
	sgt	$7F(a0,d0.w)
	sgt	($FFFFFFFF).w
	sgt	($FFFFFFFF).l

	sle.b	d0
	sle.b	(a0)
	sle.b	(a0)+
	sle.b	-(a0)
	sle.b	$7FFF(a0)
	sle.b	$7F(a0,d0.w)
	sle.b	($FFFFFFFF).w
	sle.b	($FFFFFFFF).l
	sle	d0
	sle	(a0)
	sle	(a0)+
	sle	-(a0)
	sle	$7FFF(a0)
	sle	$7F(a0,d0.w)
	sle	($FFFFFFFF).w
	sle	($FFFFFFFF).l

	shs.b	d0
	shs.b	(a0)
	shs.b	(a0)+
	shs.b	-(a0)
	shs.b	$7FFF(a0)
	shs.b	$7F(a0,d0.w)
	shs.b	($FFFFFFFF).w
	shs.b	($FFFFFFFF).l
	shs	d0
	shs	(a0)
	shs	(a0)+
	shs	-(a0)
	shs	$7FFF(a0)
	shs	$7F(a0,d0.w)
	shs	($FFFFFFFF).w
	shs	($FFFFFFFF).l

	slo.b	d0
	slo.b	(a0)
	slo.b	(a0)+
	slo.b	-(a0)
	slo.b	$7FFF(a0)
	slo.b	$7F(a0,d0.w)
	slo.b	($FFFFFFFF).w
	slo.b	($FFFFFFFF).l
	slo	d0
	slo	(a0)
	slo	(a0)+
	slo	-(a0)
	slo	$7FFF(a0)
	slo	$7F(a0,d0.w)
	slo	($FFFFFFFF).w
	slo	($FFFFFFFF).l

	dbt.w	d0,*
	dbf.w	d0,*
	dbhi.w	d0,*
	dbls.w	d0,*
	dbcc.w	d0,*
	dbcs.w	d0,*
	dbne.w	d0,*
	dbeq.w	d0,*
	dbvc.w	d0,*
	dbvs.w	d0,*
	dbpl.w	d0,*
	dbmi.w	d0,*
	dbge.w	d0,*
	dblt.w	d0,*
	dbgt.w	d0,*
	dble.w	d0,*
	dbhs.w	d0,*
	dblo.w	d0,*
	dbt	d0,*
	dbf	d0,*
	dbhi	d0,*
	dbls	d0,*
	dbcc	d0,*
	dbcs	d0,*
	dbne	d0,*
	dbeq	d0,*
	dbvc	d0,*
	dbvs	d0,*
	dbpl	d0,*
	dbmi	d0,*
	dbge	d0,*
	dblt	d0,*
	dbgt	d0,*
	dble	d0,*
	dbhs	d0,*
	dblo	d0,*

	bra.s	*
	bra.w	*

	bsr.s	*
	bsr.w	*

	bhi.s	*
	bls.s	*
	bcc.s	*
	bcs.s	*
	bne.s	*
	beq.s	*
	bvc.s	*
	bvs.s	*
	bpl.s	*
	bmi.s	*
	bge.s	*
	blt.s	*
	bgt.s	*
	ble.s	*
	bhs.s	*
	blo.s	*
	bhi.w	*
	bls.w	*
	bcc.w	*
	bcs.w	*
	bne.w	*
	beq.w	*
	bvc.w	*
	bvs.w	*
	bpl.w	*
	bmi.w	*
	bge.w	*
	blt.w	*
	bgt.w	*
	ble.w	*
	bhs.w	*
	blo.w	*

	moveq.l	#0,d0
	moveq	#0,d0

	divu.w	d0,d0
	divu.w	(a0),d0
	divu.w	(a0)+,d0
	divu.w	-(a0),d0
	divu.w	$7FFF(a0),d0
	divu.w	$7F(a0,d0.w),d0
	divu.w	($FFFFFFFF).w,d0
	divu.w	($FFFFFFFF).l,d0
	divu.w	#$FFFF,d0
	divu.w	*(pc),d0
	divu.w	*(pc,d0.w),d0
	divu	d0,d0
	divu	(a0),d0
	divu	(a0)+,d0
	divu	-(a0),d0
	divu	$7FFF(a0),d0
	divu	$7F(a0,d0.w),d0
	divu	($FFFFFFFF).w,d0
	divu	($FFFFFFFF).l,d0
	divu	#$FFFF,d0
	divu	*(pc),d0
	divu	*(pc,d0.w),d0

	divs.w	d0,d0
	divs.w	(a0),d0
	divs.w	(a0)+,d0
	divs.w	-(a0),d0
	divs.w	$7FFF(a0),d0
	divs.w	$7F(a0,d0.w),d0
	divs.w	($FFFFFFFF).w,d0
	divs.w	($FFFFFFFF).l,d0
	divs.w	#$FFFF,d0
	divs.w	*(pc),d0
	divs.w	*(pc,d0.w),d0
	divs	d0,d0
	divs	(a0),d0
	divs	(a0)+,d0
	divs	-(a0),d0
	divs	$7FFF(a0),d0
	divs	$7F(a0,d0.w),d0
	divs	($FFFFFFFF).w,d0
	divs	($FFFFFFFF).l,d0
	divs	#$FFFF,d0
	divs	*(pc),d0
	divs	*(pc,d0.w),d0

	sbcd.b	d0,d0
	sbcd.b	-(a0),-(a0)
	sbcd	d0,d0
	sbcd	-(a0),-(a0)

	or.b	d0,d0
	or.b	(a0),d0
	or.b	(a0)+,d0
	or.b	-(a0),d0
	or.b	$7FFF(a0),d0
	or.b	$7F(a0,d0.w),d0
	or.b	($FFFFFFFF).w,d0
	or.b	($FFFFFFFF).l,d0
	or.b	*(pc),d0
	or.b	*(pc,d0.w),d0
	or.b	d0,(a0)
	or.b	d0,(a0)+
	or.b	d0,-(a0)
	or.b	d0,$7FFF(a0)
	or.b	d0,$7F(a0,d0.w)
	or.b	d0,($FFFFFFFF).w
	or.b	d0,($FFFFFFFF).l
	or.w	d0,d0
	or.w	(a0),d0
	or.w	(a0)+,d0
	or.w	-(a0),d0
	or.w	$7FFF(a0),d0
	or.w	$7F(a0,d0.w),d0
	or.w	($FFFFFFFF).w,d0
	or.w	($FFFFFFFF).l,d0
	or.w	*(pc),d0
	or.w	*(pc,d0.w),d0
	or.w	d0,(a0)
	or.w	d0,(a0)+
	or.w	d0,-(a0)
	or.w	d0,$7FFF(a0)
	or.w	d0,$7F(a0,d0.w)
	or.w	d0,($FFFFFFFF).w
	or.w	d0,($FFFFFFFF).l
	or.l	d0,d0
	or.l	(a0),d0
	or.l	(a0)+,d0
	or.l	-(a0),d0
	or.l	$7FFF(a0),d0
	or.l	$7F(a0,d0.w),d0
	or.l	($FFFFFFFF).w,d0
	or.l	($FFFFFFFF).l,d0
	or.l	*(pc),d0
	or.l	*(pc,d0.w),d0
	or.l	d0,(a0)
	or.l	d0,(a0)+
	or.l	d0,-(a0)
	or.l	d0,$7FFF(a0)
	or.l	d0,$7F(a0,d0.w)
	or.l	d0,($FFFFFFFF).w
	or.l	d0,($FFFFFFFF).l

	sub.b	d0,d0
	sub.b	(a0),d0
	sub.b	(a0)+,d0
	sub.b	-(a0),d0
	sub.b	$7FFF(a0),d0
	sub.b	$7F(a0,d0.w),d0
	sub.b	($FFFFFFFF).w,d0
	sub.b	($FFFFFFFF).l,d0
	sub.b	*(pc),d0
	sub.b	*(pc,d0.w),d0
	sub.b	d0,(a0)
	sub.b	d0,(a0)+
	sub.b	d0,-(a0)
	sub.b	d0,$7FFF(a0)
	sub.b	d0,$7F(a0,d0.w)
	sub.b	d0,($FFFFFFFF).w
	sub.b	d0,($FFFFFFFF).l
	sub.w	d0,d0
	sub.w	(a0),d0
	sub.w	(a0)+,d0
	sub.w	-(a0),d0
	sub.w	$7FFF(a0),d0
	sub.w	$7F(a0,d0.w),d0
	sub.w	($FFFFFFFF).w,d0
	sub.w	($FFFFFFFF).l,d0
	sub.w	*(pc),d0
	sub.w	*(pc,d0.w),d0
	sub.w	d0,(a0)
	sub.w	d0,(a0)+
	sub.w	d0,-(a0)
	sub.w	d0,$7FFF(a0)
	sub.w	d0,$7F(a0,d0.w)
	sub.w	d0,($FFFFFFFF).w
	sub.w	d0,($FFFFFFFF).l
	sub.l	d0,d0
	sub.l	(a0),d0
	sub.l	(a0)+,d0
	sub.l	-(a0),d0
	sub.l	$7FFF(a0),d0
	sub.l	$7F(a0,d0.w),d0
	sub.l	($FFFFFFFF).w,d0
	sub.l	($FFFFFFFF).l,d0
	sub.l	*(pc),d0
	sub.l	*(pc,d0.w),d0
	sub.l	d0,(a0)
	sub.l	d0,(a0)+
	sub.l	d0,-(a0)
	sub.l	d0,$7FFF(a0)
	sub.l	d0,$7F(a0,d0.w)
	sub.l	d0,($FFFFFFFF).w
	sub.l	d0,($FFFFFFFF).l

	subx.b	d0,d0
	subx.b	-(a0),-(a0)
	subx.w	d0,d0
	subx.w	-(a0),-(a0)
	subx.l	d0,d0
	subx.l	-(a0),-(a0)

	suba.w	d0,a0
	suba.w	a0,a0
	suba.w	(a0),a0
	suba.w	(a0)+,a0
	suba.w	-(a0),a0
	suba.w	$7FFF(a0),a0
	suba.w	$7F(a0,d0.w),a0
	suba.w	($FFFFFFFF).w,a0
	suba.w	($FFFFFFFF).l,a0
	suba.w	#$7FFF,a0
	suba.w	*(pc),a0
	suba.w	*(pc,d0.w),a0
	suba.l	d0,a0
	suba.l	a0,a0
	suba.l	(a0),a0
	suba.l	(a0)+,a0
	suba.l	-(a0),a0
	suba.l	$7FFF(a0),a0
	suba.l	$7F(a0,d0.w),a0
	suba.l	($FFFFFFFF).w,a0
	suba.l	($FFFFFFFF).l,a0
	suba.l	#$7FFF,a0
	suba.l	*(pc),a0
	suba.l	*(pc,d0.w),a0

	eor.b	d0,d0
	eor.b	d0,(a0)
	eor.b	d0,(a0)+
	eor.b	d0,-(a0)
	eor.b	d0,$7FFF(a0)
	eor.b	d0,$7F(a0,d0.w)
	eor.b	d0,($FFFFFFFF).w
	eor.b	d0,($FFFFFFFF).l
	eor.w	d0,d0
	eor.w	d0,(a0)
	eor.w	d0,(a0)+
	eor.w	d0,-(a0)
	eor.w	d0,$7FFF(a0)
	eor.w	d0,$7F(a0,d0.w)
	eor.w	d0,($FFFFFFFF).w
	eor.w	d0,($FFFFFFFF).l
	eor.l	d0,d0
	eor.l	d0,(a0)
	eor.l	d0,(a0)+
	eor.l	d0,-(a0)
	eor.l	d0,$7FFF(a0)
	eor.l	d0,$7F(a0,d0.w)
	eor.l	d0,($FFFFFFFF).w
	eor.l	d0,($FFFFFFFF).l

	cmpm.b	(a0)+,(a1)+
	cmpm.w	(a0)+,(a1)+
	cmpm.l	(a0)+,(a1)+

	cmp.b	d0,d0
	cmp.b	(a0),d0
	cmp.b	(a0)+,d0
	cmp.b	-(a0),d0
	cmp.b	$7FFF(a0),d0
	cmp.b	$7F(a0,d0.w),d0
	cmp.b	($FFFFFFFF).w,d0
	cmp.b	($FFFFFFFF).l,d0
	cmp.b	*(pc),d0
	cmp.b	*(pc,d0.w),d0
	cmp.w	d0,d0
	cmp.w	(a0),d0
	cmp.w	(a0)+,d0
	cmp.w	-(a0),d0
	cmp.w	$7FFF(a0),d0
	cmp.w	$7F(a0,d0.w),d0
	cmp.w	($FFFFFFFF).w,d0
	cmp.w	($FFFFFFFF).l,d0
	cmp.w	*(pc),d0
	cmp.w	*(pc,d0.w),d0
	cmp.l	d0,d0
	cmp.l	(a0),d0
	cmp.l	(a0)+,d0
	cmp.l	-(a0),d0
	cmp.l	$7FFF(a0),d0
	cmp.l	$7F(a0,d0.w),d0
	cmp.l	($FFFFFFFF).w,d0
	cmp.l	($FFFFFFFF).l,d0
	cmp.l	*(pc),d0
	cmp.l	*(pc,d0.w),d0

	cmpa.w	d0,a0
	cmpa.w	a0,a0
	cmpa.w	(a0),a0
	cmpa.w	(a0)+,a0
	cmpa.w	-(a0),a0
	cmpa.w	$7FFF(a0),a0
	cmpa.w	$7F(a0,d0.w),a0
	cmpa.w	($FFFFFFFF).w,a0
	cmpa.w	($FFFFFFFF).l,a0
	cmpa.w	#$FFFF,a0
	cmpa.w	*(pc),a0
	cmpa.w	*(pc,d0.w),a0
	cmpa.l	d0,a0
	cmpa.l	a0,a0
	cmpa.l	(a0),a0
	cmpa.l	(a0)+,a0
	cmpa.l	-(a0),a0
	cmpa.l	$7FFF(a0),a0
	cmpa.l	$7F(a0,d0.w),a0
	cmpa.l	($FFFFFFFF).w,a0
	cmpa.l	($FFFFFFFF).l,a0
	cmpa.l	#$FFFF,a0
	cmpa.l	*(pc),a0
	cmpa.l	*(pc,d0.w),a0

	mulu.w	d0,d0
	mulu.w	(a0),d0
	mulu.w	(a0)+,d0
	mulu.w	-(a0),d0
	mulu.w	$7FFF(a0),d0
	mulu.w	$7F(a0,d0.w),d0
	mulu.w	($FFFFFFFF).w,d0
	mulu.w	($FFFFFFFF).l,d0
	mulu.w	#$FFFF,d0
	mulu.w	*(pc),d0
	mulu.w	*(pc,d0.w),d0
	mulu	d0,d0
	mulu	(a0),d0
	mulu	(a0)+,d0
	mulu	-(a0),d0
	mulu	$7FFF(a0),d0
	mulu	$7F(a0,d0.w),d0
	mulu	($FFFFFFFF).w,d0
	mulu	($FFFFFFFF).l,d0
	mulu	#$FFFF,d0
	mulu	*(pc),d0
	mulu	*(pc,d0.w),d0

	muls.w	d0,d0
	muls.w	(a0),d0
	muls.w	(a0)+,d0
	muls.w	-(a0),d0
	muls.w	$7FFF(a0),d0
	muls.w	$7F(a0,d0.w),d0
	muls.w	($FFFFFFFF).w,d0
	muls.w	($FFFFFFFF).l,d0
	muls.w	#$FFFF,d0
	muls.w	*(pc),d0
	muls.w	*(pc,d0.w),d0
	muls	d0,d0
	muls	(a0),d0
	muls	(a0)+,d0
	muls	-(a0),d0
	muls	$7FFF(a0),d0
	muls	$7F(a0,d0.w),d0
	muls	($FFFFFFFF).w,d0
	muls	($FFFFFFFF).l,d0
	muls	#$FFFF,d0
	muls	*(pc),d0
	muls	*(pc,d0.w),d0

	abcd.b	d0,d0
	abcd.b	-(a0),-(a0)
	abcd	d0,d0
	abcd	-(a0),-(a0)

	exg.l	d0,d0
	exg.l	a0,a0
	exg.l	d0,a0
	exg.l	a0,d0
	exg	d0,d0
	exg	a0,a0
	exg	d0,a0
	exg	a0,d0

	and.b	d0,d0
	and.b	(a0),d0
	and.b	(a0)+,d0
	and.b	-(a0),d0
	and.b	$7FFF(a0),d0
	and.b	$7F(a0,d0.w),d0
	and.b	($FFFFFFFF).w,d0
	and.b	($FFFFFFFF).l,d0
	and.b	*(pc),d0
	and.b	*(pc,d0.w),d0
	and.b	d0,(a0)
	and.b	d0,(a0)+
	and.b	d0,-(a0)
	and.b	d0,$7FFF(a0)
	and.b	d0,$7F(a0,d0.w)
	and.b	d0,($FFFFFFFF).w
	and.b	d0,($FFFFFFFF).l
	and.w	d0,d0
	and.w	(a0),d0
	and.w	(a0)+,d0
	and.w	-(a0),d0
	and.w	$7FFF(a0),d0
	and.w	$7F(a0,d0.w),d0
	and.w	($FFFFFFFF).w,d0
	and.w	($FFFFFFFF).l,d0
	and.w	*(pc),d0
	and.w	*(pc,d0.w),d0
	and.w	d0,(a0)
	and.w	d0,(a0)+
	and.w	d0,-(a0)
	and.w	d0,$7FFF(a0)
	and.w	d0,$7F(a0,d0.w)
	and.w	d0,($FFFFFFFF).w
	and.w	d0,($FFFFFFFF).l
	and.l	d0,d0
	and.l	(a0),d0
	and.l	(a0)+,d0
	and.l	-(a0),d0
	and.l	$7FFF(a0),d0
	and.l	$7F(a0,d0.w),d0
	and.l	($FFFFFFFF).w,d0
	and.l	($FFFFFFFF).l,d0
	and.l	*(pc),d0
	and.l	*(pc,d0.w),d0
	and.l	d0,(a0)
	and.l	d0,(a0)+
	and.l	d0,-(a0)
	and.l	d0,$7FFF(a0)
	and.l	d0,$7F(a0,d0.w)
	and.l	d0,($FFFFFFFF).w
	and.l	d0,($FFFFFFFF).l

	add.b	d0,d0
	add.b	(a0),d0
	add.b	(a0)+,d0
	add.b	-(a0),d0
	add.b	$7FFF(a0),d0
	add.b	$7F(a0,d0.w),d0
	add.b	($FFFFFFFF).w,d0
	add.b	($FFFFFFFF).l,d0
	add.b	*(pc),d0
	add.b	*(pc,d0.w),d0
	add.b	d0,(a0)
	add.b	d0,(a0)+
	add.b	d0,-(a0)
	add.b	d0,$7FFF(a0)
	add.b	d0,$7F(a0,d0.w)
	add.b	d0,($FFFFFFFF).w
	add.b	d0,($FFFFFFFF).l
	add.w	d0,d0
	add.w	(a0),d0
	add.w	(a0)+,d0
	add.w	-(a0),d0
	add.w	$7FFF(a0),d0
	add.w	$7F(a0,d0.w),d0
	add.w	($FFFFFFFF).w,d0
	add.w	($FFFFFFFF).l,d0
	add.w	*(pc),d0
	add.w	*(pc,d0.w),d0
	add.w	d0,(a0)
	add.w	d0,(a0)+
	add.w	d0,-(a0)
	add.w	d0,$7FFF(a0)
	add.w	d0,$7F(a0,d0.w)
	add.w	d0,($FFFFFFFF).w
	add.w	d0,($FFFFFFFF).l
	add.l	d0,d0
	add.l	(a0),d0
	add.l	(a0)+,d0
	add.l	-(a0),d0
	add.l	$7FFF(a0),d0
	add.l	$7F(a0,d0.w),d0
	add.l	($FFFFFFFF).w,d0
	add.l	($FFFFFFFF).l,d0
	add.l	*(pc),d0
	add.l	*(pc,d0.w),d0
	add.l	d0,(a0)
	add.l	d0,(a0)+
	add.l	d0,-(a0)
	add.l	d0,$7FFF(a0)
	add.l	d0,$7F(a0,d0.w)
	add.l	d0,($FFFFFFFF).w
	add.l	d0,($FFFFFFFF).l

	addx.b	d0,d0
	addx.b	-(a0),-(a0)
	addx.w	d0,d0
	addx.w	-(a0),-(a0)
	addx.l	d0,d0
	addx.l	-(a0),-(a0)

	adda.w	d0,a0
	adda.w	a0,a0
	adda.w	(a0),a0
	adda.w	(a0)+,a0
	adda.w	-(a0),a0
	adda.w	$7FFF(a0),a0
	adda.w	$7F(a0,d0.w),a0
	adda.w	($FFFFFFFF).w,a0
	adda.w	($FFFFFFFF).l,a0
	adda.w	#$7FFF,a0
	adda.w	*(pc),a0
	adda.w	*(pc,d0.w),a0
	adda.l	d0,a0
	adda.l	a0,a0
	adda.l	(a0),a0
	adda.l	(a0)+,a0
	adda.l	-(a0),a0
	adda.l	$7FFF(a0),a0
	adda.l	$7F(a0,d0.w),a0
	adda.l	($FFFFFFFF).w,a0
	adda.l	($FFFFFFFF).l,a0
	adda.l	#$7FFF,a0
	adda.l	*(pc),a0
	adda.l	*(pc,d0.w),a0

	asl.b	d0,d0
	asl.b	#1,d0
	asl.w	d0,d0
	asl.w	#1,d0
	asl.l	d0,d0
	asl.l	#1,d0
	asl.w	(a0)
	asl.w	(a0)+
	asl.w	-(a0)
	asl.w	$7FFF(a0)
	asl.w	$7F(a0,d0.w)
	asl.w	($FFFFFFFF).w
	asl.w	($FFFFFFFF).l
	asl	(a0)
	asl	(a0)+
	asl	-(a0)
	asl	$7FFF(a0)
	asl	$7F(a0,d0.w)
	asl	($FFFFFFFF).w
	asl	($FFFFFFFF).l

	asr.b	d0,d0
	asr.b	#1,d0
	asr.w	d0,d0
	asr.w	#1,d0
	asr.l	d0,d0
	asr.l	#1,d0
	asr.w	(a0)
	asr.w	(a0)+
	asr.w	-(a0)
	asr.w	$7FFF(a0)
	asr.w	$7F(a0,d0.w)
	asr.w	($FFFFFFFF).w
	asr.w	($FFFFFFFF).l
	asr	(a0)
	asr	(a0)+
	asr	-(a0)
	asr	$7FFF(a0)
	asr	$7F(a0,d0.w)
	asr	($FFFFFFFF).w
	asr	($FFFFFFFF).l

	lsl.b	d0,d0
	lsl.b	#1,d0
	lsl.w	d0,d0
	lsl.w	#1,d0
	lsl.l	d0,d0
	lsl.l	#1,d0
	lsl.w	(a0)
	lsl.w	(a0)+
	lsl.w	-(a0)
	lsl.w	$7FFF(a0)
	lsl.w	$7F(a0,d0.w)
	lsl.w	($FFFFFFFF).w
	lsl.w	($FFFFFFFF).l
	lsl	(a0)
	lsl	(a0)+
	lsl	-(a0)
	lsl	$7FFF(a0)
	lsl	$7F(a0,d0.w)
	lsl	($FFFFFFFF).w
	lsl	($FFFFFFFF).l

	lsr.b	d0,d0
	lsr.b	#1,d0
	lsr.w	d0,d0
	lsr.w	#1,d0
	lsr.l	d0,d0
	lsr.l	#1,d0
	lsr.w	(a0)
	lsr.w	(a0)+
	lsr.w	-(a0)
	lsr.w	$7FFF(a0)
	lsr.w	$7F(a0,d0.w)
	lsr.w	($FFFFFFFF).w
	lsr.w	($FFFFFFFF).l
	lsr	(a0)
	lsr	(a0)+
	lsr	-(a0)
	lsr	$7FFF(a0)
	lsr	$7F(a0,d0.w)
	lsr	($FFFFFFFF).w
	lsr	($FFFFFFFF).l

	roxl.b	d0,d0
	roxl.b	#1,d0
	roxl.w	d0,d0
	roxl.w	#1,d0
	roxl.l	d0,d0
	roxl.l	#1,d0
	roxl.w	(a0)
	roxl.w	(a0)+
	roxl.w	-(a0)
	roxl.w	$7FFF(a0)
	roxl.w	$7F(a0,d0.w)
	roxl.w	($FFFFFFFF).w
	roxl.w	($FFFFFFFF).l
	roxl	(a0)
	roxl	(a0)+
	roxl	-(a0)
	roxl	$7FFF(a0)
	roxl	$7F(a0,d0.w)
	roxl	($FFFFFFFF).w
	roxl	($FFFFFFFF).l

	roxr.b	d0,d0
	roxr.b	#1,d0
	roxr.w	d0,d0
	roxr.w	#1,d0
	roxr.l	d0,d0
	roxr.l	#1,d0
	roxr.w	(a0)
	roxr.w	(a0)+
	roxr.w	-(a0)
	roxr.w	$7FFF(a0)
	roxr.w	$7F(a0,d0.w)
	roxr.w	($FFFFFFFF).w
	roxr.w	($FFFFFFFF).l
	roxr	(a0)
	roxr	(a0)+
	roxr	-(a0)
	roxr	$7FFF(a0)
	roxr	$7F(a0,d0.w)
	roxr	($FFFFFFFF).w
	roxr	($FFFFFFFF).l

	rol.b	d0,d0
	rol.b	#1,d0
	rol.w	d0,d0
	rol.w	#1,d0
	rol.l	d0,d0
	rol.l	#1,d0
	rol.w	(a0)
	rol.w	(a0)+
	rol.w	-(a0)
	rol.w	$7FFF(a0)
	rol.w	$7F(a0,d0.w)
	rol.w	($FFFFFFFF).w
	rol.w	($FFFFFFFF).l
	rol	(a0)
	rol	(a0)+
	rol	-(a0)
	rol	$7FFF(a0)
	rol	$7F(a0,d0.w)
	rol	($FFFFFFFF).w
	rol	($FFFFFFFF).l

	ror.b	d0,d0
	ror.b	#1,d0
	ror.w	d0,d0
	ror.w	#1,d0
	ror.l	d0,d0
	ror.l	#1,d0
	ror.w	(a0)
	ror.w	(a0)+
	ror.w	-(a0)
	ror.w	$7FFF(a0)
	ror.w	$7F(a0,d0.w)
	ror.w	($FFFFFFFF).w
	ror.w	($FFFFFFFF).l
	ror	(a0)
	ror	(a0)+
	ror	-(a0)
	ror	$7FFF(a0)
	ror	$7F(a0,d0.w)
	ror	($FFFFFFFF).w
	ror	($FFFFFFFF).l
