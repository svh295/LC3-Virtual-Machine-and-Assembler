; ex7 with safeguarding of registers

.ORGI x3000
    AND R2, R2, #0
    ADD R2, R2, #7
    ST R2, Param

    AND R3, R3, #0
    ADD R3, R3, #4

    JSR DoubleValue

Done HALT

DoubleValue
    ST R3, SaveR3
    ST R4, SaveR4

    LD R4, Param

    Loop
        ADD R4, R4, R4
        ADD R3, R3, #-1
        BRp Loop

    ST R4, Result
    LD R3, SaveR3
    LD R4, SaveR4

    RET

; memory locations

    SaveR3 .BLKW 1
    SaveR4 .BLKW 1

;end of program

; date declarations
    Param .BLKW 1
    Result .BLKW 1

.END
