// test_program.c
int main() {
    // Custom ecall: print number 42
    asm volatile(
        "li a0, 42\n"
        "li a7, 100\n"
        "ecall\n"
    );

    // Custom fence
    asm volatile(
        "li a7, 101\n"
        "ecall\n"
    );

    // Exit
    asm volatile(
        "li a0, 0\n"
        "li a7, 102\n"
        "ecall\n"
    );

    return 0;
}
