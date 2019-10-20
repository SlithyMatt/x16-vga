.include "x16.inc"

.org $080D
.segment "STARTUP"
.segment "INIT"
.segment "ONCE"
.segment "CODE"
   jmp start

.include "loadbank.asm"

LAST_BM_BANK   = 9
END_BM_ADDR    = $AC00
PAL_BANK       = 9
PAL_ADDR       = $B000
PAL_END_ADDR   = $B200

VRAM_BITMAP    = $04000

getramaddr: ; A = Offset into RAM bank >> 5
            ; Output: X/Y = Absolute address
   pha
   and #$07
   asl
   asl
   asl
   asl
   asl
   ora #<RAM_WIN
   tax
   pla
   and #$F8
   lsr
   lsr
   lsr
   ora #>RAM_WIN
   tay
   rts

loadvram:   ; A = RAM bank,
            ; X = beginning of data offset >> 5,
            ; Y = end of data offset >> 5 (0 = whole bank)
   sta RAM_BANK
   tya
   pha               ; push end offset
   txa
   jsr getramaddr    ; get start address
   stx ZP_PTR_1
   sty ZP_PTR_1+1
   pla               ; pull end offset
   beq @wholebank
   jsr getramaddr    ; get end address from offset
   stx ZP_PTR_2
   sty ZP_PTR_2+1
   jmp @initloop
@wholebank:
   lda #<(RAM_WIN+RAM_WIN_SIZE)
   sta ZP_PTR_2
   lda #>(RAM_WIN+RAM_WIN_SIZE)
   sta ZP_PTR_2+1
@initloop:
   ldx #0
@loop:
   lda (ZP_PTR_1,x)  ; load from banked RAM
   sta VERA_data     ; store to next VRAM address
   lda #1
   adc ZP_PTR_1
   sta ZP_PTR_1
   lda #0
   adc ZP_PTR_1+1
   sta ZP_PTR_1+1
   lda ZP_PTR_1
   cmp ZP_PTR_2
   bne @loop
   lda ZP_PTR_1+1
   cmp ZP_PTR_2+1
   bne @loop   
   rts

start:
   jsr loadbank                  ; load bitmap and palette to banked RAM
   
   lda #0
   sta VERA_ctrl
   VERA_SET_ADDR VRAM_layer0, 0  ; disable VRAM layer 0
   lda #$FE
   and VERA_data
   sta VERA_data
   
   VERA_SET_ADDR VRAM_layer1, 1  ; configure VRAM layer 1
   lda #$E1                      ; 8bpp bitmap
   sta VERA_data
   lda #$00                      ; 320x240
   sta VERA_data
   sta VERA_data                 ; tile map N/A
   sta VERA_data
   sta VERA_data                 ; bitmap base $04000
   lda #$10
   sta VERA_data
   
   VERA_SET_ADDR VRAM_BITMAP, 1  ; load bitmap into VRAM from banked RAM
   jmp @loop
@bank:
   .byte 0
@loop:
   lda @bank
   cmp #LAST_BM_BANK
   beq lastbank
   ldx #0
   ldy #0
   jsr loadvram
   inc @bank
   jmp @loop
lastbank:
   ldx #0
   ldy #((END_BM_ADDR-RAM_BANK)>>5)
   jsr loadvram
   
   VERA_SET_ADDR VRAM_palette, 1 ; load palette into VRAM from banked RAM
   lda #PAL_BANK
   ldx #((PAL_ADDR-RAM_BANK)>>5)
   ldy #((PAL_END_ADDR-RAM_BANK)>>5)
   jsr loadvram
   
   VERA_SET_ADDR VRAM_hscale, 1  ; set display to 2x scale
   lda #64
   sta VERA_data
   sta VERA_data

end:
   nop
   jmp end  ; loop forever
   
