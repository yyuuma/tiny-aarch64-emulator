cmake_minimum_required(VERSION 3.20)
project(aarch64_emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/988ea2c1798de7779f656df2281dd36d6039a17a.zip
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
FetchContent_MakeAvailable(googletest)

add_executable(main
    src/main.cpp
    src/bits.cpp
    src/decode.cpp
    src/memory.cpp
    src/cpu.cpp
)

target_include_directories(main PRIVATE
    include)

target_compile_options(main PRIVATE
    -Wall
    -Wextra
    -Wpedantic)

enable_testing()

add_executable(emulator_tests
    tests/bits_test.cpp
    src/bits.cpp)

target_include_directories(emulator_tests PRIVATE
    include)

target_link_libraries(emulator_tests PRIVATE
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(emulator_tests)
