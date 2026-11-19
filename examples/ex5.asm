.ORGI x3000
    AND R2, R2, #0; clear R2
    ADD R2, R2, #7; add 7 into R2

    JSR DoubleValue; contains 14
    JSR DoubleValue; contains 28
    JSR DoubleValue; 56
Done HALT

; Doubles R2
; inputs: R2
; outputs: R2
DoubleValue
    ADD R2, R2, R2; doubles the value inside of R2
    RET

.END

;example of a subroutine
; registers are shared between the main code and the subroutine