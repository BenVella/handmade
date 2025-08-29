#!/data/data/com.termux/files/usr/bin/bash
#!/bin/bash

# Get the directory where this script is located
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIPT_DIR="$PROJECT_ROOT/scripts"

echo "[DAP] Sourcing setup script from: $SCRIPT_DIR/termux11.sh"
source "$SCRIPT_DIR/termux11.sh"

# log environment variables
echo "[DAP] DISPLAY is: $DISPLAY"

echo "[DAP] Launching binary: $WORKING_DIR/out/handmade"

