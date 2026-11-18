# LC3 Virtual Machine and Assembler

This project features an LC3 virtual machine based off of https://www.jmeiners.com/lc3-vm/#:lc3.c_2
and an assembler to create codes to run inside the virtual machine.

The purpose of this project was to learn how assembly languages, instruction encoding, memory, and CPU 
execution work on a low level, as an introduction to computer architecture.

# Assembler
The assembler converts LC3 assembly source files (.asm) into binary object files (.obj).
It uses a two pass assembly process to convert LC3 instructions into 16-bit machine code.

The assembler use is: ./assembler <.asm file> <.obj file>

# Virtual Machine
The simulator loads LC3 object files into simulated memory and executes the encoded instructions.

The virtual machine's use is: ./LC3 <.obj file>

# Instruction Set
currently implemented: ADD, AND, NOT, LEA, BR, JMP, LDI, STI, LDR, STR, JSR, JSRR, and TRAP

# Specifications
word size = 2 bytes (16 bits), 
addressable memory = 65,536 words, 
total addressable memory = 128 KB, 
general purpose registers = 8, 
program counter = 16 bits