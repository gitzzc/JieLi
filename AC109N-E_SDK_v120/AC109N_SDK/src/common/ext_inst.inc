#ifndef __ext_ins__
#define __ext_ins__

#define wr0     0x0
#define wr2     0x1
#define wr4     0x2
#define wr6     0x3

#define iwr0    0x10
#define iwr2    0x11
#define iwr4    0x12
#define iwr6    0x13


add16 macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x00, (0x20 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x00, (0x10 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x00, (0x00 | x<<2 | y)
    #endif
    endm

addc16 macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x00, (0x50 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x00, (0x40 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x00, (0x30 | x<<2 | y)
    #endif
    endm

add16s macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x00, (0x80 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x00, (0x70 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x00, (0x60 | x<<2 | y)
    #endif
    endm

addc16s macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x00, (0xb0 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x00, (0xa0 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x00, (0x90 | x<<2 | y)
    #endif
    endm

sub16 macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x00, (0xe0 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x00, (0xd0 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x00, (0xc0 | x<<2 | y)
    #endif
    endm

subc16 macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x01, (0x10 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x01, (0x00 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x00, (0xf0 | x<<2 | y)
    #endif
    endm

sub16s macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x01, (0x40 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x01, (0x30 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x01, (0x20 | x<<2 | y)
    #endif
    endm

subc16s macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x01, (0x70 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x01, (0x60 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x01, (0x50 | x<<2 | y)
    #endif
    endm

anl16 macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x01, (0xa0 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x01, (0x90 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x01, (0x80 | x<<2 | y)
    #endif
    endm

orl16 macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x01, (0xd0 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x01, (0xc0 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x01, (0xb0 | x<<2 | y)
    #endif
    endm

xrl16 macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x02, (0x00 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x01, (0xf0 | (y & 0xf) | x<<2)
    #else
        db 0xa5, 0x01, (0xe0 | x<<2 | y)
    #endif
    endm

mac16 macro x, y
    #if (x > 0x3)
        errors ...
    #elif (y > 0x3)
        db 0xa5, 0x02, (0x20 | x<<2 | (y & 0xf))
    #else
        db 0xa5, 0x02, (0x10 | x<<2 | y)
    #endif
    endm

mac16s macro x, y
    #if (x > 0x3)
        errors ...
    #elif (y > 0x3)
        db 0xa5, 0x02, (0x40 | x<<2 | (y & 0xf))
    #else
        db 0xa5, 0x02, (0x30 | x<<2 | y)
    #endif
    endm

mov16b macro x, y
    #if (((x > 0x3) + (y > 0x3)) > 1)
        errors ...
    #elif (x > 0x3)
        db 0xa5, 0x02, (0x60 | (x & 0xf) | y<<2)
    #elif (y > 0x3)
        db 0xa5, 0x02, (0x50 | (y & 0xf) | x<<2)
    #else
        errors ...
    #endif
    endm

lsl16k macro x, y
    #if (x > 0xf)
        errors ...
    #else
        db 0xa5, 0x03, (0x00 | (y & 0xc) << 2 | (y & 0x3) | x<<2)
    #endif
    endm

lsr16k macro x, y
    #if (x > 0xf)
        errors ...
    #else
        db 0xa5, 0x03, (0x40 | (y & 0xc) << 2 | (y & 0x3) | x<<2)
    #endif
    endm

rotl16k macro x, y
    #if (x > 0xf)
        errors ...
    #else
        db 0xa5, 0x03, (0x80 | (y & 0xc) << 2 | (y & 0x3) | x<<2)
    #endif
    endm

asr16k macro x, y
    #if (x > 0xf)
        errors ...
    #else
        db 0xa5, 0x03, (0xc0 | (y & 0xc) << 2 | (y & 0x3) | x<<2)
    #endif
    endm

rotl8 macro x, y
    db 0xa5, 0x04
    endm

rotr8 macro x, y
    db 0xa5, 0x05
    endm

rotl8k macro x,y
    #if (y > 0x7)
        errors ...
    #else
        db 0xa5, (0x08 | y)
    #endif
    endm

inc16 macro x
    db 0xa5, (0x10 | x)
    endm

inc216 macro x
    db 0xa5, (0x14 | x)
    endm

dec16 macro x
    db 0xa5, (0x18 | x)
    endm

dec216 macro x
    db 0xa5, (0x1c | x)
    endm

rotl16 macro x, y
    #if (x > 0x3)
        db 0xa5, (0x28 | (x & 0xf))
    #else
        db 0xa5, (0x20 | x)
    #endif
    endm

rotr16 macro x, y
    #if (x > 0x3)
        db 0xa5, (0x2c | (x & 0xf))
    #else
        db 0xa5, (0x24 | x)
    #endif
    endm

lsr16 macro x,y
    #if (x > 0x3)
        db 0xa5, (0x38 | (x & 0xf))
    #else
        db 0xa5, (0x30 | x)
    #endif
    endm

lsl16 macro x, y
    #if (x > 0x3)
        db 0xa5, (0x3c | (x & 0xf))
    #else
        db 0xa5, (0x34 | x)
    #endif
    endm

asr16 macro x, y
    #if (x > 0x3)
        db 0xa5, (0x44 | (x & 0xf))
    #else
        db 0xa5, (0x40 | x)
    #endif
    endm

movsa macro x, y
    db 0xa5, (0x48 | x)
    endm

movsb macro x, y
    db 0xa5, (0x4c | x)
    endm

//mov8bta macro x, y
//    db 0xa5, (0x50 | (y & 0xf))
//    endm
//
//movat8b macro x, y
//    db 0xa5, (0x54 | (x & 0xf))
//    endm
//
//mov8bb macro x, y
//    db 0xa5, (0x70 | (x & 0xf)<<2 | (y & 0xf))
//    endm

cmp16 macro x, y
    #if (x > 0x3)
        errors ...
    #elif (y > 0x3)
        errors ...
    #else
        db 0xa5, (0x60 | x<<2 | y)
    #endif
    endm

mov16 macro x, y
    #if ((x > 0x3) & (y > 0x3))
        db 0xa5, (0xb0 | (x & 0xf)<<2 | (y & 0xf))
    #elif (x > 0x3)
        db 0xa5, (0x90 | ((x & 0xf)) | y<<2)
    #elif (y > 0x3)
        db 0xa5, (0xa0 | (x<<2) | (y & 0xf))
    #else
        db 0xa5, (0x80 | x<<2 | y)
    #endif
    endm

mul16 macro x, y
    #if (x > 0x3)
        errors ...
    #elif (y > 0x3)
        db 0xa5, (0xd0 | x<<2 | (y & 0xf))
    #else
        db 0xa5, (0xc0 | x<<2 | y)
    #endif
    endm

mul16s macro x, y
    #if (x > 0x3)
        errors ...
    #elif (y > 0x3)
        db 0xa5, (0xf0 | x<<2 | (y & 0xf))
    #else
        db 0xa5, (0xe0 | x<<2 | y)
    #endif
    endm
    
mul16wr macro x, y
    #if (x != wr4)
        errors ...
    #elseif (y != wr2)
        errors ...
    #else
        db 0xa5, 0x06
    #endif
    endm

mul16swr macro x, y
    #if (x != wr4)
        errors ...
    #elseif (y != wr2)
        errors ...
    #else
        db 0xa5, 0x07
    #endif
    endm    
      
/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
    @date: 2012-05-29,19:30
*/
/*----------------------------------------------------------------------------*/
uart_put macro reg
    LOCAL lab
    push ACC
lab:
    mov a, UART_STA
    jnb ACC.7, lab
    pop ACC
    mov UART_BUF,reg
    
    endm

#endif