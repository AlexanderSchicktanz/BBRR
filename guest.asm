# test_program.asm - RISC-V assembly with custom ecalls
.section .text
.global main

main:
    # Define a string in the .data section
    .data
    my_string: .asciz "Hello, RISC-V!"  # Null-terminated string

    .text
    # Load the address of the string into a0
    la      a0, my_string    # Address of the string
    li      a1, 14           # Length of the string (including null terminator)

    # Custom ecall: print string (assuming ecall 101 is for printing a string)
    li      a7, 101          # Custom ecall number 101 (print string)
    ecall                    # Execute ecall

    li      a0, 500
    li      a1, 500
    la      a2, my_string
    li      a3, 14
    li      a7, 110
    ecall
    li      a7, 111
    ecall
    li      a7, 112
    ecall

    # Exit
    li      a0, 0            # Exit code 0
    li      a7, 102          # Custom ecall number 102 (exit)
    ecall                    # Execute ecall

    # Normal return (shouldn't reach here due to custom exit ecall)
    li      a0, 0
    li      a7, 93           # Linux syscall 93 = exit_group
    ecall
