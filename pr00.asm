	.inesprg PRGNUM
	.ineschr CHRNUM
	.inesmir 1
	.inesmap 3
	
PRGNUM		= 2
CHRNUM		= 1

	LIST
	.rsset $00
tmp			.rs 9
cnt			.rs 1

hscr		.rs 1
vscr		.rs 1



pad			.rs 1
pad_trg		.rs 1

	.rsset $0770
fHideScreen	.rs 1	

ppu1		.rs 1
ppu2		.rs 1
	
rnd			.rs 7

	
	.bank 0
	.org $8000
	
	.bank 1
	.org $A000
	
	.bank 2
	.org $C000
	
	.bank 3
	.org $E000
	
	.include "_func.h"
	
RESET
	sei
	cld
	lda #$10
	sta $2000
	ldx #$FF
	txs
	
.waitvsync
	lda $2002
	bpl .waitvsync
.waitvsync2
	lda $2002
	bpl .waitvsync2
	
	ldy #$FF
	jsr initializer
	
	sta $4011

	lda #$FF
	sta $4017
	lda #$00
	sta $4010
	
	lda #$A5		; seed of random value
	sta rnd
	lda #$0F		; SQ1, SQ2, TRI, NOI - enabled
	sta $4015

	lda #$01
	sta $E000		; disable MMC3 irq generating 
	
	lda #$06		; Hide BG and Sprites
	sta $2001
	
	jsr initvram
	jsr initspr
	inc fHideScreen
	
	lda ppu1
	ora #$80
	sta $2000

	cli

InfiniteLoop
	jmp InfiniteLoop
	
NMI
	
	rti

IRQ
	rti
	
	.org $FFFA
	.dw NMI
	.dw RESET
	.dw IRQ
	
	
	.bank PRGNUM*2
	.org $0000
	
	
	