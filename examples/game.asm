.section .data
my_string: .asciz "RISC-V Game"

.section .text
.global main

main:
    li a0, 5 # Disable Raylib-Log
    li a7, 10
    ecall

    li a0, 500
    li a1, 500
    la a2, my_string
    li a3, 12
    li a7, 6
    ecall
    li      t0, 50
    li      t1, 50
    li      t2, 1000
    loop:
    li      a7, 9
    ecall

    bnez     a0, exit

    li      a7, 20
    ecall

    li      a0, 250
    li      a1, 100
    li      a2, 50
    li      a7, 22
    ecall

    li      a0, 0
    li      a1, 0
    li      a2, 0
    li      a7, 22
    ecall

    li      a0, 50
    li      a1, 50
    li      a2, 50
    li      a3, 50
    li      a4, 50
    li      a5, 250
    li      a6, 250
    li      a7, 31
    ecall

    li      a7, 2
    ecall

    addi    a0, a1, 0
    ecall

    li      a7, 21
    ecall
    j loop
    exit:

    li      a7, 8
    ecall

    li      a0, 0            # Exit code 0
    li      a7, 44           # Custom ecall number (exit)
    ecall                    # Execute ecall
