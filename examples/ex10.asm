;STR example (store relative)

.ORIG x3000
    LEA R1, myArray; load array adresses (not values)
    AND R3, R3, #0; clear R3
    ADD R3, R3, #-2; set R3 to -2
    STR R3, R1, #2; store R3 into array[2]

Done HALT

myArray .FILL #12; array element 0
        .FILL #22; element 1
        .FILL #-7; element 2, contains -2 after running

.END