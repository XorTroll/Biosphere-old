.macro BIO_ASM_Code_Begin name
    .section .text.\name, "ax", %progbits
    .global \name
    .type \name, %function
    .align 2
    .cfi_startproc
\name:
.endm

.macro BIO_ASM_Code_End
    .cfi_endproc
.endm

.weak bioUnusedExceptionHandler

.weak bioInitExceptionEntry
BIO_ASM_Code_Begin bioInitExceptionEntry
    cmp x1, #0
    beq bioExceptionEntryAbort
    adrp x5, :got:bioUnusedExceptionHandler
    ldr x5, [x5, #:got_lo12:bioUnusedExceptionHandler]
    cmp x5, #0
    beq bioExceptionEntryAbort
    stp x9, x10, [sp, #-16]!
    stp x11, x12, [sp, #-16]!
    stp x13, x14, [sp, #-16]!
    stp x15, x16, [sp, #-16]!
    stp x17, x18, [sp, #-16]!
    stp x19, x20, [sp, #-16]!
    str x21, [sp, #-16]!

    stp x0, x1, [sp, #-16]!
    sub sp, sp, #16

    mov x0, sp
    mov x1, #8
    mov w2, wzr
    mov x3, #0
    bl SVC_GetInfo
    mov w6, w0
    ldr x7, [sp], #16

    ldp x0, x1, [sp], #16

    ldr x21, [sp], #16
    ldp x19, x20, [sp], #16
    ldp x17, x18, [sp], #16
    ldp x15, x16, [sp], #16
    ldp x13, x14, [sp], #16
    ldp x11, x12, [sp], #16
    ldp x9, x10, [sp], #16

    cbnz w6, bioExceptionEntryAbort

    adrp x6, excignoredebug
    ldr  w5, [x6, #:lo12:excignoredebug]

    cbnz w5, bioExceptionEntryStart
    cbnz x7, bioExceptionEntryAbort

bioExceptionEntryStart:
    adrp x2, excdump
    add  x2, x2, #:lo12:excdump
    mov x5, x2

    str w0, [x2], #4
    str wzr, [x2], #4
    str wzr, [x2], #4
    str wzr, [x2], #4

    ldp x3, x4, [x1]
    str x5, [x1], #16
    stp x3, x4, [x2], #16
    ldp x3, x4, [x1], #16
    stp x3, x4, [x2], #16
    ldp x3, x4, [x1], #16
    stp x3, x4, [x2], #16
    ldp x3, x4, [x1], #16
    stp x3, x4, [x2], #16
    ldr x3, [x1], #8
    str x3, [x2], #8

    str x9, [x2], #8
    stp x10, x11, [x2], #16
    stp x12, x13, [x2], #16
    stp x14, x15, [x2], #16
    stp x16, x17, [x2], #16
    stp x18, x19, [x2], #16
    stp x20, x21, [x2], #16
    stp x22, x23, [x2], #16
    stp x24, x25, [x2], #16
    stp x26, x27, [x2], #16
    str x28, [x2], #8

    str x29, [x2], #8

    ldr x3, [x1], #8
    str x3, [x2], #8

    adrp x4, excstack
    add  x4, x4, #:lo12:excstack

    adrp x5, excstsize
    ldr  x5, [x5, #:lo12:excstsize]
    add x4, x4, x5

    ldr x3, [x1]
    str x4, [x1], #8
    str x3, [x2], #8

    adrp x4, bioExceptionReturnEntry
    add  x4, x4, #:lo12:bioExceptionReturnEntry

    ldr x3, [x1]
    str x4, [x1], #8
    str x3, [x2], #8

    str xzr, [x2], #8

    stp q0, q1, [x2], #32
    stp q2, q3, [x2], #32
    stp q4, q5, [x2], #32
    stp q6, q7, [x2], #32
    stp q8, q9, [x2], #32
    stp q10, q11, [x2], #32
    stp q12, q13, [x2], #32
    stp q14, q15, [x2], #32
    stp q16, q17, [x2], #32
    stp q18, q19, [x2], #32
    stp q20, q21, [x2], #32
    stp q22, q23, [x2], #32
    stp q24, q25, [x2], #32
    stp q26, q27, [x2], #32
    stp q28, q29, [x2], #32
    stp q30, q31, [x2], #32

    ldr w3, [x1], #4
    str w3, [x2], #4
    ldr w3, [x1], #4
    str w3, [x2], #4
    ldr w3, [x1], #4
    str w3, [x2], #4
    ldr w3, [x1], #4
    str w3, [x2], #4

    ldr x3, [x1], #8
    str x3, [x2], #8

    mov w0, wzr
    b bioExceptionEntryFinalize

bioExceptionEntryAbort:
    mov w0, #0xf801
bioExceptionEntryFinalize:
    bl SVC_ReturnFromException
    b .
BIO_ASM_Code_End

BIO_ASM_Code_Begin bioExceptionReturnEntry
    bl bioUnusedExceptionHandler

    mov w0, wzr
    mov x1, #0
    mov x2, #0
    bl SVC_Break
    b .
BIO_ASM_Code_End