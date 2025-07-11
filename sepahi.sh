#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <input_file.cpy>"
    exit 1
fi

INPUT="$1"
BASE="${INPUT%.*}"
TMP_CPP="${BASE}.cpp"
OUTPUT_EXE="${BASE}.out"

# Compile parser
echo "[0/3] Compiling sepahi parser..."
g++ parser.cpp -o sepahi
if [ $? -ne 0 ]; then
    echo "Failed to compile sepahi (parser.cpp)"
    exit 1
fi

# Run parser
echo "[1/3] Running sepahi..."
./sepahi "$TMP_CPP" < "$INPUT"

# Compile parsed output
echo "[2/3] Compiling with g++..."
g++ "$TMP_CPP" -o "$OUTPUT_EXE"
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the result
echo "[3/3] Running output:"
echo "----------------------"
./"$OUTPUT_EXE"