.ORIG x3000
    AND R0, R0, #0; clears out whatever is in R0
    ADD R0, R0, #5; sets R0 to 5
Done HALT

;end program
.END

; 5 is an immediate value (hard coded constant) these are limited to 5 bits (-16 to +15)
; for larger or smaller values, memory has to be allocated