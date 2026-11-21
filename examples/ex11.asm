; trap examples
; lc3 has built in trap routines (2 user input, 3 user output, and halt)
; TRAP x20 and GETC do the same function
; TRAP x25 is HALT

.ORIG x3000
    TRAP x20; gets a single character and stores in R0
    HALT
.END

; the trap vector table in lc3 reserves space for 256 memory locations
; however there are only 6 impelemented