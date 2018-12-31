.section ".crt0", "ax"
.global _start

_start:
    b bioInitPreStartup
    .word bioInitMOD0 - _start
    .ascii "Homebrew"

.org _start + 0x80

bioInitPreStartup:
    mov x7, x30

    bl +4
    sub x6, x30, #0x88

    mov x5, x0
    mov x4, x1

    cmp x5, #0
    ccmn x4, #1, #4, ne
    beq bioInitBssClear
    b bioInitExceptionEntry

bioInitBssClear:
    mov x27, x7
    mov x25, x5
    mov x26, x4

    adrp x0, __bss_start__
    adrp x1, __bss_end__
    add x0, x0, #:lo12:__bss_start__
    add x1, x1, #:lo12:__bss_end__
    sub x1, x1, x0
    add x1, x1, #7
    bic x1, x1, #7

bioInitBssLoop: 
    str  xzr, [x0], #8
    subs x1, x1, #8
    bne  bioInitBssLoop

    mov  x1, sp
    adrp x0, stackt
    str  x1, [x0, #:lo12:stackt]

    mov  x0, x6
    adrp x1, _DYNAMIC
    add  x1, x1, #:lo12:_DYNAMIC
    bl   bioInitDynamic

    mov  x0, x25
    mov  x1, x26
    mov  x2, x27
    bl   bioInitStartup

    adrp x0, __argc
    ldr w0, [x0, #:lo12:__argc]
    adrp x1, __argv
    ldr x1, [x1, #:lo12:__argv]
    adrp x30, exit
    add x30, x30, #:lo12:exit
    b main

.global bioQuickPostExit
.type   bioQuickPostExit, %function
bioQuickPostExit:
    adrp x8, stackt
    ldr x8, [x8, #:lo12:stackt]
    mov sp, x8

    br x1

.global bioInitMOD0
bioInitMOD0:
    .ascii "MOD0"
    .word _DYNAMIC - bioInitMOD0
    .word __bss_start__ - bioInitMOD0
    .word __bss_end__ - bioInitMOD0
    .word __eh_frame_hdr_start - bioInitMOD0
    .word __eh_frame_hdr_end - bioInitMOD0
    .word 0