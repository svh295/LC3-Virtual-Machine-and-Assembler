;trap examples

; trap x21 or OUT prints a character from R0 to the screen
; R0 must contain the ascii character to print

.ORIG x3000
    LD R0, myChar
    TRAP x21; OUT command also works
    HALT

myChar .FILL x41; ascii for A

.END