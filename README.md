# Bare Bones RISC-V Runner

A cross-platform RISC-V emulator with [Raylib](https://www.raylib.com)-based visualization and [libriscv](https://github.com/fraser/libriscv)-based emulation.

---

## Features
* automatic Assembly and Emulation
* many custom [ecalls](https://github.com/AlexanderSchicktanz/BBRR/wiki/ECALLs) allowing for
    * easy GUI programming
    * Terminal-I/O
    * debugging
---

## Installation

### 1. Clone the Repository
Clone the repository **with submodules** to ensure `libriscv` and `raylib` are included:
```bash
git clone --recurse-submodules https://github.com/yourusername/riscv_executor.git
cd riscv_executor
```
> **Note:** If you already cloned the repository without submodules, run:
> ```bash
> git submodule update --init --recursive
> ```

---

### 2. Install Dependencies
Ensure the following dependencies are installed on your system:

| Dependency       | Linux (Debian/Ubuntu)       | Linux (Fedora)             | macOS (Homebrew)       | Windows (Chocolatey)   |
|------------------|-----------------------------|----------------------------|------------------------|------------------------|
| **CMake**        | `sudo apt install cmake`    | `sudo dnf install cmake`   | `brew install cmake`   | `choco install cmake`  |
| **C++ Compiler** | `sudo apt install g++`      | `sudo dnf install gcc-c++` | `xcode-select --install` | Install [MSVC](https://visualstudio.microsoft.com/) or MinGW |
| **Python 3**     | `sudo apt install python3`  | `sudo dnf install python3` | `brew install python`  | `choco install python` |
| **Build System** | `sudo apt install ninja-build` | `sudo dnf install ninja-build` | `brew install ninja` | `choco install ninja` |

> **Note for Windows Users**:
> - Use **Git Bash** or **WSL** for the best experience.
> - Ensure `python3` and `cmake` are added to your `PATH`.

---

### 3. Build Raylib (Submodule)
Raylib is included as a submodule. Build it using CMake:
```bash
cd lib/raylib
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
cmake --build . -j$(nproc)
cd ../..
```
> **Windows (Git Bash/WSL)**:
> ```bash
> cd lib/raylib
> mkdir -p build && cd build
> cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
> cmake --build . --config Release
> cd ../..
> ```

---

### 4. Build libriscv (Submodule)
Build the `libriscv` submodule:
```bash
cd libriscv/emulator
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
cd ../../..
```
> **Windows (Git Bash/WSL)**:
> ```bash
> cd libriscv/emulator
> mkdir -p build && cd build
> cmake .. -DCMAKE_BUILD_TYPE=Release
> cmake --build . --config Release
> cd ../../..
> ```

---

### 5. Build the Project
Create a build directory and configure the project with CMake:
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build . -j$(nproc)
```
> **Alternative Generators**:
> - Linux/macOS: Use `-G "Unix Makefiles"` or `-G Xcode`.
> - Windows: Use `-G "Visual Studio 17 2022"` or `-G "MinGW Makefiles"`.

> **Windows (Git Bash/WSL)**:
> ```bash
> cmake .. -DCMAKE_BUILD_TYPE=Release -G "Ninja"
> cmake --build . --config Release
> ```

---
### 6. Run the Executable
After building, run the emulator:
```bash
./riscv_executor
```
> **Windows**:
> ```bash
> ./riscv_executor.exe
> ```

---
### 7. (Optional) Run the Python or Bash Helper Script
If your project includes a Python helper script, run it with:
```bash
./run.py <filename>
```
> **Windows**:
> Run the script explicitly using:
> ```bash
> python3 run.py <filename>
> ```
> **Note**: Double-clicking the script on Windows will not work unless you associate `.py` files with Python or use a wrapper.

---

## 🐛 Troubleshooting
| Issue                          | Solution                                                                 |
|--------------------------------|--------------------------------------------------------------------------|
| **Missing submodules**         | Run `git submodule update --init --recursive`.                           |
| **CMake not found**            | Ensure CMake is installed and added to your `PATH`.                     |
| **Compiler not found**         | Install a C++17-compatible compiler (e.g., `g++`, `clang`, or MSVC).    |
| **Build fails**                | Check the CMake output for specific errors. Common fixes include:       |
|                                | - Reinstalling dependencies.                                             |
|                                | - Using a different build generator (e.g., `-G "Unix Makefiles"`).      |
| **Python script won't run**    | Run it explicitly with `python3 helper.py`.                              |

---
## 🌍 Platform Support
| Platform       | Status       | Notes                                  |
|----------------|--------------|----------------------------------------|
| **Linux**      | ✅ Tested    | Works with GCC                         |

> **Windows Note**: Full support requires a Unix-like environment (e.g., WSL, Git Bash) or Visual Studio with CMake integration.

---
## 🤝 Contributing
Contributions are welcome! Open an issue or submit a pull request.

---
## 📜 License
[GNU GPL v3](LICENSE)
