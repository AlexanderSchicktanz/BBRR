# Barebone RISC-V Runner
## Installation & Build
* install riscv64-linux-gnu
* clone this repository

    cd build
    make

## Usage

    ./run <filename>

## Additional ecalls
* 110 OpenWindow(a0,a1,stringFrom(a2,a3))
* 111 CloseWindow()
* 112 BeginDrawing()
* 113 EndDrawing()
* 114 DrawRectangle()
* 115 ClearBackground()
* 116 a0=WindowShouldClose()
The current ecalls will be made more flexible, by only using values in registers and the applications memory.
More ecalls will be added later.
