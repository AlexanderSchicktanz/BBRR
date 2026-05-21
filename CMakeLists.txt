cmake_minimum_required(VERSION 3.14)
project(riscv_executor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Raylib as a subdirectory
add_subdirectory(lib/raylib)

# Find libriscv (make sure you've cloned and built it first)
add_subdirectory(libriscv/emulator)

# Your executable
add_executable(riscv_executor src/main.cpp)

# Link Raylib to your executable
target_link_libraries(riscv_executor PRIVATE raylib)

# Link libriscv to your executable
target_link_libraries(riscv_executor PRIVATE riscv)

# Include directories for libriscv
target_include_directories(riscv_executor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libriscv/include)
