# Print a string
li a7, 0          # ECALL code for PRINTSTRING
la a0, hello_msg  # Load address of the string
li a1, 13         # Length of the string
ecall

# Print a character
li a7, 1          # ECALL code for PRINTCHAR
li a0, 'A'        # Character to print
ecall

# Print an integer
li a7, 2          # ECALL code for PRINTINT
li a0, 42         # Integer to print
ecall

li a7, 44
li a0, 0
ecall

.data
hello_msg: .asciz "Hello, RISC-V!\n"
