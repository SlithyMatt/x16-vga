.include "x16.inc"

.include "bankparams.asm"

loadbank:
   lda #0
   sta ROM_BANK
   lda #1
   ldx #1
   ldy #0
   jsr SETLFS        ; SetFileParams(LogNum=1,DevNum=1,SA=0)
   lda #<bankparams
   sta ZP_PTR_1
   lda #>bankparams
   sta ZP_PTR_1+1
   ldx #0
@loop:
   cpx #BANKS_TO_LOAD
   beq end_loadbank
   stx RAM_BANK
   txa
   pha
   jsr @load         ; load bank
   pla
   tax
   inx
   jmp @loop
   
@load:               ; load banked RAM using params starting at ZP_PTR_1
   ldx 0
   lda (ZP_PTR_1,x)
   pha               ; push filename length to stack
   jsr @inczp1
   lda (ZP_PTR_1,x)
   pha               ; push filename address low byte to stack
   jsr @inczp1
   lda (ZP_PTR_1,x)
   tay               ; Y = filename address high byte
   jsr @inczp1
   pla               ; pull filename address low byte from stack
   tax               ; X = filename address low byte
   pla               ; pull filename length from stack to A
   jsr SETNAM        ; SetFileName(filename)
   lda #0
   ldx #<RAM_WIN
   ldy #>RAM_WIN
   jsr LOAD          ; LoadFile(Verify=0,Address=RAM_WIN)
   rts
   
@inczp1:             ; increment ZP_PTR_1
   pha               ; push A to stack
   lda #1
   clc
   adc ZP_PTR_1
   sta ZP_PTR_1
   lda #0
   adc ZP_PTR_1+1
   sta ZP_PTR_1+1
   pla               ; pull A back from stack
   rts

end_loadbank:   
   rts

