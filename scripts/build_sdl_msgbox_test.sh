#!/bin/bash

SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
INPUT_DIR="$SOURCE_DIR/code"
OUTPUT_DIR="$SOURCE_DIR/out"

COMPILER="c++"
CFLAGS="-g $(sdl2-config --cflags --libs)"

# Check for correct number of arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_source_file> <output_binary>"
    exit 1
fi

INPUT_FILE="$1"
OUTPUT_FILE="$2"

SOURCE_PATH="$INPUT_DIR/$INPUT_FILE"
OUTPUT_PATH="$OUTPUT_DIR/$OUTPUT_FILE"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Compile the source file
$COMPILER $CFLAGS "$SOURCE_PATH" -o "$OUTPUT_PATH"
