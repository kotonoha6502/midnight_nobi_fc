;MMCのバンク切り替え実行
;Yに切り替えモード、Aにセットするバンクを格納してから呼び出すこと。
MMCBankSet
	sty $8000
	sta $8001
	rts

initializer
	ldx #$07
	lda #$00
	sta <tmp+6
.0
	cpx #$01
	beq .1
	stx <tmp+7
.2
	sta [tmp+6],y
	dey
	cpy #$FF
	bne .2
.1
	dex
	bpl .0
	rts

ClearBGBuffer
	lda #$00
	tay
.rep
	sta $0300,y
	iny
	cpy #$40
	bne .rep
	rts
	
acsvram_main:
	sta $2006
	iny
	lda [tmp],y
	sta $2006
	iny
	lda [tmp],y
	asl a
	pha
	lda ppu1
	ora #$04
	bcs .setppu
	and #$FB
.setppu
	jsr setppuctl
	pla
	lsr a
	tax
.rep
	iny
	lda [tmp],y
	sta $2007
	dex
	bne .rep
	tya
	sec
	adc <tmp
	sta <tmp
	lda #$00
	adc <tmp+1
	sta <tmp+1
	lda #$3F
	sta $2006
	lda #$00
	sta $2006
	sta $2006
	sta $2006 
acsvram
	lda $2002
	ldy #$00
	lda [tmp],y
	bne acsvram_main
	rts

initvram
	lda $2002
	lda ppu1
	ora #$10
	and #$F0
	jsr setppuctl
	lda #$24
	jsr .initnmtbl
	lda #$20
.initnmtbl
	sta $2006
	lda #$00
	sta $2006
	ldx #$04
	ldy #$C0
	lda #$24
.0
	sta $2007
	dey
	bne .0
	dex
	bne .0
	ldy #$3F
	txa
.1
	sta $2007
	sta $0300,y
	dey
	bpl .1
	sta <hscr
	sta <vscr
	sta $2005
	sta $2005
	rts

setppuctl
	sta $2000
	sta ppu1
	rts

initspr
	lda #$F7
	lda #$00
	tax
.rep
	sta $0200,x
	inx
	inx
	inx
	inx
	bne .rep
	rts



indirjmp
	asl a
	tay
	pla
	sta <tmp+4
	pla
	sta <tmp+5 
	iny
	lda [tmp+4],y
	sta <tmp+6
	iny
	lda [tmp+4],y
	sta <tmp+7
	jmp [tmp+6]

getpad
	lda <pad
	sta <pad_trg
	ldx #$01
	stx $4016
	dex
	stx $4016
	ldy #$08
.rep
	pha
	lda $4016
	sta <tmp
	lsr a
	ora <tmp
	lsr a
	pla
	ror a
	dey
	bne .rep
	sta <pad
	eor <pad_trg
	and <pad
	sta <pad_trg
	rts

getrnd
	ldx #$00
	ldy #$07
	lda rnd
	and #$02
	sta <tmp
	lda rnd+1
	and #$02
	eor <tmp
	clc
	beq .loop
	sec
.loop
	ror rnd,x
	inx
	dey
	bne .loop
	rts

	
