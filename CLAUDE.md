# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Communication Language
このリポジトリでは日本語で応答してください。

## Project Overview
This is a C++23 CMake project that demonstrates integration with the Boost library. The current `boost/asio` branch is specifically designed for testing and verifying Boost.Asio functionality, serving as a testing ground for asynchronous I/O operations and network programming features.

## Build System
- **Build Tool**: CMake (minimum version 3.14)
- **C++ Standard**: C++23
- **Target Name**: CmakeCpp
- **Main Executable**: CmakeCpp.exe

### Build Commands
```bash
# Configure and build (Windows with Visual Studio)
cmake -B out/build/x64-Debug -S .
cmake --build out/build/x64-Debug

# Run the executable
./out/build/x64-Debug/CmakeCpp.exe
```

## Dependencies
- **Boost**: Located at `D:/Library/boost`, using headers-only components
  - Currently using `boost/version.hpp`
  - CMake finds Boost via `CMAKE_PREFIX_PATH`

## Architecture
- **Entry Point**: `main.cpp` - Simple Hello World program that displays Boost version
- **Build Output**: `out/build/x64-Debug/` directory contains all build artifacts
- **Testing**: GoogleTest framework is available in the build system (found in `_deps` directory)

## Branch Information
- **Current Branch**: boost/asio (dedicated to testing and verifying Boost.Asio functionality)
- **Main Branch**: master (contains C++23 HelloWorld project)
- **Purpose**: This branch focuses on implementing and testing asynchronous I/O operations using Boost.Asio

## Development Notes
- Project uses Japanese comments in CMakeLists.txt and README.md
- The build system appears to support multiple libraries and test configurations based on the build directory structure
- GoogleTest integration suggests this project may expand to include unit testing