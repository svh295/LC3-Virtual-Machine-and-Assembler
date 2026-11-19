.ORGI x3000
    LD R0, Data1
    LD R1, Data2

    ADD R3, R0, R1

    ST R3, Result; copies values from registers into memory locations
Done HALT

; end of program

    Data1 .FILL #10
    Data2 .FILL x00FF
    Result .BLKW 1
.END

;