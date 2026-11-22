; if/else conditional

.ORIG x3000
    LEA R0, enterAge
    PUTS
    IN 
    JSR convertInt; jump to subroutine to convert to an int
    BRp Valid
    BRnz Invalid

    convertInt
        LD R1, negZero
        ADD R0, R0, R1
        RET

    Valid
        LEA R0, validAge
        PUTS
        BR Done

    Invalid
        LEA R0, invalidAge
        PUTS

    Done HALT

; data declarations

negZero .FILL #-48
enterAge .STRINGZ "Enter Age\n"
validAge .STRINGZ "Valid Age\n"
invalidAge .STRINGZ "Invalid Age\n"

.END
