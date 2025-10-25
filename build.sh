#!/bin/bash

SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INPUT_FILES=$(find "$SOURCE_DIR/code" -name '*.cpp')
OUTPUT_FILE="$SOURCE_DIR/out/handmade"
COMPILE_COMMANDS="$SOURCE_DIR/compile_commands.json"

mkdir -p "$(dirname "$OUTPUT_FILE")"

COMPILER="clang++ -std=c++20"
CFLAGS="-g $(sdl2-config --cflags --libs)"

# Compile
echo "Compiling: $INPUT_FILES"
$COMPILER $INPUT_FILES -o "$OUTPUT_FILE" $CFLAGS 

# Convert CFLAGS string to array
read -ra FLAGS_ARRAY <<< "$CFLAGS"

# Build arguments array
ARGS=("$COMPILER")
ARGS+=("${FLAGS_ARRAY[@]}")
ARGS+=($INPUT_FILES "-o" "$OUTPUT_FILE")

echo "[" > "$COMPILE_COMMANDS"
first=1
for file in $INPUT_FILES; do
  if [ $first -eq 0 ]; then echo "," >> "$COMPILE_COMMANDS"; fi
  first=0

  # Build per-file arguments
  read -ra FLAGS_ARRAY <<< "$CFLAGS"
  ARGS=("$COMPILER")
  ARGS+=("${FLAGS_ARRAY[@]}")
  ARGS+=("$file" "-o" "$OUTPUT_FILE")

  jq -n \
    --arg dir "$SOURCE_DIR" \
    --arg file "$file" \
    --arg output "$OUTPUT_FILE" \
    --argjson args "$(printf '%s\n' "${ARGS[@]}" | jq -R . | jq -s .)" \
    '{directory: $dir, file: $file, output: $output, arguments: $args}' >> "$COMPILE_COMMANDS"
done
echo "]" >> "$COMPILE_COMMANDS"

