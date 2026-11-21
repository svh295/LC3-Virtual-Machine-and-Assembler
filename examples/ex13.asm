; trap examples
; stringz

; TRAP x22 prints out strings (same as PUTS)

.ORIG x3000
    LEA R0, Message; loads address of message into Register 0
    TRAP x22; or PUTS, displays whats inside of R0
    HALT

;data declarations
Message .STRINGZ "Hello, World!"

.END
