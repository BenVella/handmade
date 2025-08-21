#!/bin/bash

# Get the directory of the script itself
SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define input and output paths
INPUT_FILE="$SOURCE_DIR/code/sdl_handmade.cpp"
OUTPUT_DIR="$SOURCE_DIR/out"
OUTPUT_FILE="$OUTPUT_DIR/handmade"
BUILD_DIR="$SOURCE_DIR/build"

# Create necessary directories
mkdir -p "$OUTPUT_DIR"
mkdir -p "$BUILD_DIR"

# Compiler and flags
COMPILER="c++"
CFLAGS="-g $(sdl2-config --cflags --libs)"

# Compile the source file
$COMPILER $CFLAGS "$INPUT_FILE" -o "$OUTPUT_FILE"

# Generate compile_commands.json
cat > "$BUILD_DIR/compile_commands.json" <<EOF
[
  {
    "directory": "$SOURCE_DIR",
    "file": "$INPUT_FILE",
    "output": "$OUTPUT_FILE",
    "arguments": [
      "$COMPILER",
      "-g",
$(sdl2-config --cflags --libs | xargs -n1 | sed 's/^/      "/;s/$/",')
      "$INPUT_FILE",
      "-o",
      "$OUTPUT_FILE"
    ]
  }
]
EOF
