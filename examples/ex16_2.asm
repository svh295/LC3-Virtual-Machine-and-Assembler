; second part of ex16
; lc3 only handles one .ORGI per file

; subroutine section

.ORIG x0400
    ADD R0, R2, R1; combine R1 and R2
    ADD R0, R0, #15
    ADD R0, R0, #15
    ADD R0, R0, #9
    RET

.END
