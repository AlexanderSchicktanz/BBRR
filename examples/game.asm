.section .data
my_string: .asciz "RISC-V Game"

.section .text
.global main

main:
    li a0, 500
    li a1, 500
    la a2, my_string
    li a3, 12
    li a7, 110
    ecall
    loop:
    li      a7, 116
    ecall

    li      a7, 100
    ecall
    bnez     a0, exit

    li      a7, 112
    ecall

    li      a7, 115
    ecall

    li      a7, 114
    ecall

    li      a7, 113
    ecall
    j loop
    exit:
    li      a7, 111
    ecall

    li      a0, 0            # Exit code 0
    li      a7, 102          # Custom ecall number 102 (exit)
    ecall                    # Execute ecall
