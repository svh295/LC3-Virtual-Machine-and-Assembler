; jmp and ret example
; 
; JMP is unconditional jump and does not return to previous code
; This is unlike JSR and JSRR which return to previous code

.ORIG x3000
    AND R1, R1, #0
    AND R2, R2, #0

    ADD R1, R1, #4
    ADD R2, R2, #7

    LEA R7, returnPoint

    LD R4, jumpPoint
    JMP R4

returnPoint
    TRAP x21
    HALT

jumpPoint .FILL x0400

.END
