#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "Usage: ./run_emulator.sh [FILE].s" >&2
    exit 1
fi

file="$1"

if [[ ! -f "$file" ]]; then
    echo "Error: File does not exist." >&2
    exit 1
fi

if [[ "$file" !=  *.s ]]; then
    echo "Error: Please input an assembly source file." >&2
    exit 1
fi

mkdir -p build/tmp
TMP_DIR=$(mktemp -d build/tmp/XXXXXXXX)
trap 'rm -rf "$TMP_DIR"' EXIT

obj="$TMP_DIR"/tmp.o
bin="$TMP_DIR"/program.bin

echo "Assembling $file into $obj."
aarch64-linux-gnu-as "$file" -o "$obj"

echo "Extracting raw instructions into $bin."
aarch64-linux-gnu-objcopy -O binary -j .text "$obj" "$bin"

echo "Running emulator:"
./build/main "$bin"
