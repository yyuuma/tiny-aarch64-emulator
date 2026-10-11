cmake_minimum_required(VERSION 3.20)
project(aarch64_emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main
    src/main.cpp
    src/bits.cpp
    src/decode.cpp
    src/memory.cpp
    src/cpu.cpp)

target_include_directories(main PRIVATE include)

target_compile_options(main PRIVATE
    -Wall
    -Wextra
    -Wpedantic)
