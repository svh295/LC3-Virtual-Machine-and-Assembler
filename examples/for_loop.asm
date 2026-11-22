; simple for loop example
; for (int i=0; i<10; i++)

.ORIG x3000
    AND R0, R0, #0; load 0 into R0 (i=0)
    AND R1, R1, #0; load 0 into R0
    ADD R1, R1, #15; R1 = 15

    ForLoop
        NOT R2, R1; 
        ADD R2, R2, #1; R2 = -15
        ADD R2, R0, R2; R2 = i - 15

        BRzp Done; if i>= 15 exit
        
        ADD R0, R0, #1; i++
        BRnzp ForLoop; repeat

    Done HALT
.END
