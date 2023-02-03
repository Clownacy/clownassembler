
; This lot is in RAM

	rsset	0			; start of RAM

ram_strt	rs.l	$100			; allow 1K for the stack, this should be plenty
						; for any BASIC program that doesn't do something
						; silly, it could even be much less.
ram_base
LAB_WARM	rs.w	1			; BASIC warm start entry point
Wrmjpv	rs.l	1			; BASIC warm start jump vector

Usrjmp	rs.w	1			; USR function JMP address
Usrjpv	rs.l	1			; USR function JMP vector

; system dependant i/o vectors
; these are in RAM and are set at start-up

V_INPT	rs.w	1			; non halting scan input device entry point
V_INPTv	rs.l	1			; non halting scan input device jump vector

V_OUTP	rs.w	1			; send byte to output device entry point
V_OUTPv	rs.l	1			; send byte to output device jump vector

V_LOAD	rs.w	1			; load BASIC program entry point
V_LOADv	rs.l	1			; load BASIC program jump vector

V_SAVE	rs.w	1			; save BASIC program entry point
V_SAVEv	rs.l	1			; save BASIC program jump vector

V_CTLC	rs.w	1			; save CTRL-C check entry point
V_CTLCv	rs.l	1			; save CTRL-C check jump vector

Itemp		rs.l	1			; temporary integer	(for GOTO etc)

Smeml		rs.l	1			; start of memory		(start of program)

; the program is stored as a series of lines each line having the following format
;
;		rs.l	1			; pointer to the next line or $00000000 if [EOT]
;		rs.l	1			; line number
;		rs.b	n			; program bytes
;		dc.b	$00			; [EOL] marker, there will be a second $00 byte, if
;						; needed, to pad the line to an even number of bytes

Sfncl		rs.l	1			; start of functions	(end of Program)

; the functions are stored as function name, function execute pointer and function
; variable name
;
;		rs.l	1			; name
;		rs.l	1			; execute pointer
;		rs.l	1			; function variable

Svarl		rs.l	1			; start of variables	(end of functions)

; the variables are stored as variable name, variable value
;
;		rs.l	1			; name
;		rs.l	1			; packed float or integer value

Sstrl		rs.l	1			; start of strings	(end of variables)

; the strings are stored as string name, string pointer and string length
;
;		rs.l	1			; name
;		rs.l	1			; string pointer
;		rs.w	1			; string length

Sarryl	rs.l	1			; start of arrays		(end of strings)

; the arrays are stored as array name, array size, array dimensions count, array
; dimensions upper bounds and array elements
;
;		rs.l	1			; name
;		rs.l	1			; size including this header
;		rs.w	1			; dimensions count
;		rs.w	1			; 1st dimension upper bound
;		rs.w	1			; 2nd dimension upper bound
;		...				; ...
;		rs.w	1			; nth dimension upper bound
;
; then (i1+1);(i2+1)...;(in+1) of either ..
;
;		rs.l	1			; packed float or integer value
;
; .. if float or integer, or ..
;
;		rs.l	1			; string pointer
;		rs.w	1			; string length
;
; .. if string

Earryl	rs.l	1			; end of arrays		(start of free mem)
Sstorl	rs.l	1			; string storage		(moving down)
Ememl		rs.l	1			; end of memory		(upper bound of RAM)
Sutill	rs.l	1			; string utility ptr
Clinel	rs.l	1			; current line		(Basic line number)
Blinel	rs.l	1			; break line		(Basic line number)

Cpntrl	rs.l	1			; continue pointer
Dlinel	rs.l	1			; current DATA line
Dptrl		rs.l	1			; DATA pointer
Rdptrl	rs.l	1			; read pointer
Varname	rs.l	1			; current var name
Cvaral	rs.l	1			; current var address
Lvarpl	rs.l	1			; variable pointer for LET and FOR/NEXT

des_sk_e	rs.l	6			; descriptor stack end address
des_sk					; descriptor stack start address
						; use a4 for the descriptor pointer
		rs.w	1			
Ibuffs	rs.l	$40			; start of input buffer
Ibuffe
						; end of input buffer

FAC1_m	rs.l	1			; FAC1 mantissa1
FAC1_e	rs.w	1			; FAC1 exponent
FAC1_s	EQU	FAC1_e+1		; FAC1 sign (b7)
		rs.w	1			

FAC2_m	rs.l	1			; FAC2 mantissa1
FAC2_e	rs.l	1			; FAC2 exponent
FAC2_s	EQU	FAC2_e+1		; FAC2 sign (b7)
FAC_sc	EQU	FAC2_e+2		; FAC sign comparison, Acc#1 vs #2
flag		EQU	FAC2_e+3		; flag byte for divide routine

PRNlword	rs.l	1			; PRNG seed long word

ut1_pl	rs.l	1			; utility pointer 1

Asptl		rs.l	1			; array size/pointer
Astrtl	rs.l	1			; array start pointer

numexp	EQU	Astrtl		; string to float number exponent count
expcnt	EQU	Astrtl+1		; string to float exponent count

expneg	EQU	Astrtl+3		; string to float eval exponent -ve flag

func_l	rs.l	1			; function pointer


						; these two need to be a word aligned pair !
Defdim	rs.w	1			; default DIM flag
cosout	EQU	Defdim		; flag which CORDIC output (re-use byte)
Dtypef	EQU	Defdim+1		; data type flag, $80=string, $40=integer, $00=float


Binss		rs.l	4			; number to bin string start (32 chrs)

Decss		rs.l	1			; number to decimal string start (16 chrs)
		rs.w	1			;
Usdss		rs.w	1			; unsigned decimal string start (10 chrs)

Hexss		rs.l	2			; number to hex string start (8 chrs)

BHsend	rs.w	1			; bin/decimal/hex string end


prstk		rs.b	1			; stacked function index

tpower	rs.b	1			; remember CORDIC power

Asrch		rs.b	1			; scan-between-quotes flag, alt search character

Dimcnt	rs.b	1			; # of dimensions

Breakf	rs.b	1			; break flag, $00=END else=break
Oquote	rs.b	1			; open quote flag (Flag: DATA; LIST; memory)
Gclctd	rs.b	1			; garbage collected flag
Sufnxf	rs.b	1			; subscript/FNX flag, 1xxx xxx = FN(0xxx xxx)
Imode		rs.b	1			; input mode flag, $00=INPUT, $98=READ

Cflag		rs.b	1			; comparison evaluation flag

TabSiz	rs.b	1			; TAB step size

comp_f	rs.b	1			; compare function flag, bits 0,1 and 2 used
						; bit 2 set if >
						; bit 1 set if =
						; bit 0 set if <

Nullct	rs.b	1			; nulls output after each line
TPos		rs.b	1			; BASIC terminal position byte
TWidth	rs.b	1			; BASIC terminal width byte
Iclim		rs.b	1			; input column limit
ccflag	rs.b	1			; CTRL-C check flag
ccbyte	rs.b	1			; CTRL-C last received byte
ccnull	rs.b	1			; CTRL-C last received byte 'life' timer

; these variables for simulator load/save routines

file_byte	rs.b	1			; load/save data byte
file_id	rs.l	1			; load/save file ID

		rs.w	1			; dummy even value and zero pad byte

prg_strt

	;ORG	*

ram_addr	EQU	$80000		; RAM start address
ram_size	EQU	$80000		; RAM size

