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

    li      a0, 0
    li      a1, 0
    li      a2, 0
    li      a7, 22
    ecall

    li      a0, 50
    addi    t0, t0, 1
    addi    t1, t1, 1
    remu    t0, t0, t2
    remu    t1, t1, t2
    addi    a1, t0, 0
    addi    a2, t1, 0
    li      a3, 50
    li      a4, 250
    li      a5, 50
    li      a6, 50
    li      a7, 23
    ecall

    li      a7, 21
    ecall
    j loop
    exit:
    li      a7, 39
    ecall

    li      a7, 8
    ecall

    li      a0, 0            # Exit code 0
    li      a7, 44           # Custom ecall number 102 (exit)
    ecall                    # Execute ecall
