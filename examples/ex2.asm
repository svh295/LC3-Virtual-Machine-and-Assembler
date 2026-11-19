;memory allocation
.ORIG x3000
    LD R0, Data1; LD loads data into a register
    LD R1, Data2
    LD R2, Data3
Done HALT

;end of program

;data declarations
    Data1 .FILL #10
    Data2 .FILL x00FF
    Data3 .FILL b10110100000101111
    Result .BLKW 1
.END

; no need to clear registers first as the declaration overwrite the previous data completely

; memory locations hold 16  bits of data (value from -32,768 to 32,767)