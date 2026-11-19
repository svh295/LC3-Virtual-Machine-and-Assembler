;simplification of ex6

.ORGI x3000
    AND R2, R2, #0; clear R2
    ADD R2, R2, #7; set R2
    ST R2, Param; store R2 as param

    AND R3, R3, #0; clear r3
    ADD R3, R3, #3; set r3

    JSR DoubleValue; result contains 56

    AND R3, R3, #0;
    ADD R3, R3, #5;

    JSR DoubleValue; now contains 224

Done HALT

;doubles value multiple times
; inputs: param containing value and R3 holding number of doubles
; outputs: result containing the doubled value
DoubleValue
    LD R4, Param; load param into R4

    Loop
        ADD R4, R4, R4; double once
        ADD R3, R3, #-1; decrement count
        BRp Loop;  continue loop if R2 is greater than 0
    
    ST R4, Result; store R4 in result
    RET; return

;end of program

;data declarations
    Param .BLKW 1
    Result .BLKW 1

.END