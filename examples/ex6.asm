.ORGI x3000
    AND R2, R2, #0; clear R2
    ADD R2, R2, #7; set R2
    ST R2, Param; store R2 into Param

    JSR DoubleValue; result contains 14

    LD R2, Result; place result into register
    ST R2, Param; copy result into param
    JSR DoubleValue; 28

    LD R2, Result
    ST R2, Param
    JSR DoubleValue

Done HALT

;Double the value inside param
; input: param contains value
; output; result contains value
DoubleValue
    LD R4, Param
    ADD R4, R4, R4
    ST R4, Result
    RET

; data declarations
    Param .BLKW 1; store block of words
    Result .BLKW 1

.END