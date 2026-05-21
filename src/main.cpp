#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <random>
#include <time.h>
#include <math.h>
#include <libriscv/machine.hpp>
#include "../lib/raylib/src/raylib.h"
using namespace riscv;
#include "ecall.hpp"


std::vector<uint8_t> load_elf_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);

    if (!file) {
        throw std::runtime_error("Error reading file: " + filename);
    }

    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv << " <riscv_elf_file> [max_instructions]" << std::endl;
        return 1;
    }

    std::string elf_file = argv[1];
    uint64_t max_instructions = 5'000'000'000ull;

    if (argc >= 3) {
        return -1;
    }

    try {
        std::cout << "[INFO] Loading ELF file: " << elf_file << std::endl;
        const std::vector<uint8_t> binary = load_elf_file(elf_file);

        Machine<RISCV64> machine{binary};

        // Setup Linux environment
        machine.setup_linux(
            {elf_file},
            {"LC_ALL=C", "USER=root"}
        );

        // Setup ALL Linux syscalls (this handles read, write, open, exit, etc.)
        machine.setup_linux_syscalls();

        // Install YOUR custom syscalls (use high numbers like 1000+ to avoid conflicts)
        for(int i = 0; i < ECALLCODE::EXIT; i++)
            Machine<RISCV64>::install_syscall_handler(i, custom_ecall_handler);

        std::cout << "[INFO] Starting simulation..." << std::endl;
        std::cout << "[INFO] ============================================\n" << std::endl;

        machine.simulate<false>(max_instructions);

        if (machine.instruction_limit_reached()) {
            std::cout << "\n[INFO] Instruction limit reached" << std::endl;
        } else {
            std::cout << "\n[INFO] Program completed normally" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
