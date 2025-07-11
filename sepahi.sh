#!/bin/bash

# Usage: ./sepahi.sh program.cpy

# Check arguments
if [ $# -ne 1 ]; then
    echo "Usage: $0 <input_file.cpy>"
    exit 1
fi

INPUT="$1"
BASE="${INPUT%.*}"           # Remove extension
TMP_CPP="${BASE}.cpp"
OUTPUT_EXE="${BASE}.out"

# Step 1: Run indentpp on input
echo "[1/3] Running indentpp..."
./sepahi < "$INPUT" > "$TMP_CPP"

# Step 2: Compile the result with g++
echo "[2/3] Compiling with g++..."
g++ "$TMP_CPP" -o "$OUTPUT_EXE"
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 2
fi

# Step 3: Execute the compiled binary
echo "[3/3] Running output:"
echo "----------------------"
./"$OUTPUT_EXE"
echo "----------------------"
