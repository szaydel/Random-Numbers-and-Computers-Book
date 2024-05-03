;
;  file:  xorshift32.s
;
;  Marsaglia's xorshift32 in 6502 assembly
;
    org $300

;  32-bit seed value, <b0><b1><b2><b3>
seed
    .byte 01
    .byte 00
    .byte 00
    .byte 00
tmp
    .byte 00
    .byte 00
    .byte 00
    .byte 00
x32
    jsr step1
    jsr step2
    jsr step3
    rts

step1
    jsr copy
    ldx #13
loop1
    jsr up
    dex
    bne loop1
    jsr merge
    rts

step2
    jsr copy
    ldx #17
loop2
    jsr down
    dex
    bne loop2
    jsr merge
    rts

step3
    jsr copy
    ldx #5
loop3
    jsr up
    dex
    bne loop3
    jsr merge
    rts

copy
    lda seed+0
    sta tmp+0
    lda seed+1
    sta tmp+1
    lda seed+2
    sta tmp+2
    lda seed+3
    sta tmp+3
    rts

merge
    lda tmp+0
    eor seed+0
    sta seed+0
    lda tmp+1
    eor seed+1
    sta seed+1
    lda tmp+2
    eor seed+2
    sta seed+2
    lda tmp+3
    eor seed+3
    sta seed+3
    rts

up
    asl tmp+0
    rol tmp+1
    rol tmp+2
    rol tmp+3
    rts

down
    lsr tmp+3
    ror tmp+2
    ror tmp+1
    ror tmp+0
    rts

;unsigned long s32 = 1;
;
;unsigned long xorshift32(void) {
;    s32 ^= (s32<<13);
;    s32 ^= (s32>>17);
;    s32 ^= (s32<<5);
;    return s32;
;}

