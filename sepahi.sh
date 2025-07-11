#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <input_file.cpy> [-m] [-d]"
    exit 1
fi

INPUT="$1"
BASE="${INPUT%.*}"
OUTPUT_EXE="${BASE}.out"
TMP_CPP="${BASE}.cpp"

# Parse additional flags
RUN_MODE=false
DEBUG_MODE=false

for arg in "$@"; do
    if [ "$arg" == "-m" ]; then
        RUN_MODE=true
    elif [ "$arg" == "-d" ]; then
        DEBUG_MODE=true
    fi
done

# [0] Compile the parser
echo "[0/3] Compiling sepahi parser..."
g++ parser.cpp -o sepahi
if [ $? -ne 0 ]; then
    echo "Failed to compile sepahi (parser.cpp)"
    exit 1
fi

# [1] Parse .cpy file and compile
echo "[1/3] Parsing and compiling..."

if $DEBUG_MODE; then
    echo "[Debug] Saving intermediate C++ to $TMP_CPP"
    ./sepahi "$TMP_CPP" < "$INPUT"
    g++ "$TMP_CPP" -o "$OUTPUT_EXE"
else
    ./sepahi - < "$INPUT" | g++ -x c++ -o "$OUTPUT_EXE" -
fi

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# [2] Run if requested
if $RUN_MODE; then
    echo "[2/3] Executing program:"
    echo "----------------------"
    ./"$OUTPUT_EXE"
else
    echo "[2/3] Done. You can run it with: ./$OUTPUT_EXE"
fi
