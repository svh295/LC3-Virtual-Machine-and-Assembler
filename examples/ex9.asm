; branching example

; Branching uses labels and condition code reg to skip to other sections of code.
; The instruction has either a p, n, of z to detonate the condition at which to
; skip to other code. These can be conbined to skip for multiple conditions, however
; they must remain in the nzp order to prevent errors.

.ORGI x3000
    LD R0, myData
    BRz ifZero
    BRp ifPositive
    BRn ifNegative

    ifZero
        NOP; insert code (example)
        BRnzp Done; skip rest and halt

    ifPositive
        NOP
        BRnzp Done

    ifNegative
        NOP
        ; BRnzp not needed as already end of program

    Done HALT

; end of program

; data declarations
    myData .FILL xF00D

.END