#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <libriscv/machine.hpp>
#include "lib/raylib/src/raylib.h"

using namespace riscv;


// Custom ecall handler for YOUR custom syscalls
void custom_ecall_handler(Machine<RISCV64>& machine) {
    const uint64_t ecall_num = machine.cpu.reg(17);

    std::cout << "[ECALL] Custom ecall: " << ecall_num << std::endl;

    switch (ecall_num) {
        case 100: {
            // Your custom ecall 1000: print a0 as number
            uint64_t value = machine.cpu.reg(10);
            std::cout << "[CUSTOM] Number: " << value << std::endl;
            break;
        }
        case 101: {
            // Your custom ecall 1001: print string
            uint64_t addr = machine.cpu.reg(10);
            uint64_t len = machine.cpu.reg(11);
            std::cout << "[CUSTOM] String: " << machine.memory.memstring(addr,len) << std::endl;
            break;
        }
        case 102: {
            // Your custom fence
            std::cout << "[CUSTOM] Fence executed!" << std::endl;
            exit(0);
            break;
        }
        case 110: {
            uint64_t width = machine.cpu.reg(10);
            uint64_t height = machine.cpu.reg(11);
            uint64_t titleAddr = machine.cpu.reg(12);
            uint64_t titleLen = machine.cpu.reg(13);

            std::string title = machine.memory.memstring(titleAddr, titleLen);
            InitWindow(width, height, title.c_str());
            break;
        }
        case 111: {
            CloseWindow();
            break;
        }
        case 112: {
            BeginDrawing();
            break;
        }
        case 113: {
            EndDrawing();
            break;
        }
        case 114: {
            DrawRectangle(50, 50, 100, 100, BLUE);
            break;
        }
        case 115: {
            ClearBackground(BLACK);
        }
        case 116: {
            machine.cpu.reg(10) = WindowShouldClose();
            break;
        }
        case 199: {
            // Your custom exit
            uint64_t code = machine.cpu.reg(10);
            std::cout << "[CUSTOM] Exit with code: " << code << std::endl;
            machine.stop();
            break;
        }
        default:
            std::cout << "[ECALL] Unknown custom ecall: " << ecall_num << std::endl;
            break;
    }
}


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

void CustomLog(int msgType, const char *text, va_list args) { return; }

int main(int argc, char* argv[]) {
    SetTraceLogCallback(CustomLog);
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
        Machine<RISCV64>::install_syscall_handler(100, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(101, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(102, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(110, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(111, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(112, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(113, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(114, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(115, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(116, custom_ecall_handler);
        Machine<RISCV64>::install_syscall_handler(199, custom_ecall_handler);

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
