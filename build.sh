#!/bin/bash

# Get the directory of the script itself
SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define input and output paths
INPUT_FILE="$SOURCE_DIR/code/sdl_handmade.cpp"
OUTPUT_DIR="$SOURCE_DIR/out"
OUTPUT_FILE="$OUTPUT_DIR/handmade"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Compile the source file
c++ "$INPUT_FILE" -o "$OUTPUT_FILE" -g `sdl2-config --cflags --libs`
