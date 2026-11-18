.ORGI x3000 ; sets starting memory address

AND R0, R0, #0 ; clear register R0
ADD R0, R0, #5 ; add 5 to R0
ADD R1, R0, #3 ; add 3 to R0 and store result in R1
HALT    ; stop the program

.END