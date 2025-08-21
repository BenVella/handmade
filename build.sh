#!/bin/bash

SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INPUT_FILE="$SOURCE_DIR/code/sdl_handmade.cpp"
OUTPUT_FILE="$SOURCE_DIR/out/handmade"
COMPILE_COMMANDS="$SOURCE_DIR/compile_commands.json"

mkdir -p "$(dirname "$OUTPUT_FILE")"

COMPILER="c++"
CFLAGS="-g $(sdl2-config --cflags --libs)"

# Compile
$COMPILER $CFLAGS "$INPUT_FILE" -o "$OUTPUT_FILE"

# Convert CFLAGS string to array
read -ra FLAGS_ARRAY <<< "$CFLAGS"

# Build arguments array
ARGS=("$COMPILER")
ARGS+=("${FLAGS_ARRAY[@]}")
ARGS+=("$INPUT_FILE" "-o" "$OUTPUT_FILE")

# Generate compile_commands.json next to build.sh
jq -n --arg dir "$SOURCE_DIR" \
      --arg file "$INPUT_FILE" \
      --arg output "$OUTPUT_FILE" \
      --argjson args "$(printf '%s\n' "${ARGS[@]}" | jq -R . | jq -s .)" \
      '[{directory: $dir, file: $file, output: $output, arguments: $args}]' \
      > "$COMPILE_COMMANDS"
