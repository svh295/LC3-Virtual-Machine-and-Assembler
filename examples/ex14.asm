; TRAP x23 or PUTSP works similar but displays 8 upper and 8 lower bits seperately

.ORIG x3000
    LEA R0, Hello
    TRAP x24
    HALT

Hello:  .FILL x6548; eH 
        .FILL x6c6c; ll
        .FILL x206f; \0o
        .FILL x0000; empty

; also works without colon ( Hello )
.END
