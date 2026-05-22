.section .data
my_string: .asciz "RISC-V Game"

.section .text
.global main

main:
    li a0, 5 # Disable Raylib-Log
    li a7, 10
    ecall

    li a0, 500 # InitWindow
    li a1, 500
    la a2, my_string
    li a3, 12
    li a7, 6
    ecall
    li      t0, 50
    li      t1, 50
    li      t2, 1000
    li      s0, 30
    loop:
    li      a7, 9 # Check if Window should close
    ecall

    bnez     a0, exit

    li      a7, 20 # Begin Drawing
    ecall

    mul     a0, t4, s0 # Clear Background
    mul     a1, t5, s0
    li      a2, 0
    li      a7, 22
    ecall

    addi    a0, t0, 0 # DPAD
    addi    a1, t1, 0
    li      a2, 50
    li      a3, 50
    li      a4, 50
    li      a5, 250
    li      a6, 250
    li      a7, 31
    ecall

    li      a7, 2 # Print DPAD-return
    ecall

    li      t2, 0       # t2 = 0 (top)
    beq     a0, t2, handle_top
    li      t2, 1       # t2 = 1 (bottom)
    beq     a0, t2, handle_bottom
    li      t2, 2       # t2 = 2 (left)
    beq     a0, t2, handle_left
    li      t2, 3       # t2 = 3 (right)
    beq     a0, t2, handle_right
    j       end_dpad     # If no match, do nothing

    handle_top:
        addi    t1, t1, -1  # Decrement y-coordinate (t1) for top press
        j       end_dpad
    handle_bottom:
        addi    t1, t1, 1   # Increment y-coordinate (t1) for bottom press
        j       end_dpad
    handle_left:
        addi    t0, t0, -1  # Decrement x-coordinate (t0) for left press
        j       end_dpad
    handle_right:
        addi    t0, t0, 1   # Increment x-coordinate (t0) for right press
        j       end_dpad
    end_dpad:

    li      a0, 250 # Joystick
    li      a1, 50
    li      a7, 30
    ecall

    addi    t4, a0, 0 # Save result of Joystick
    addi    t5, a1, 0

    li      a7, 21 # End drawing
    ecall
    j loop
    exit:

    li      a7, 8 # Close Window
    ecall

    li      a0, 0 # Exit code 0
    li      a7, 44
    ecall
