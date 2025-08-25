#!/bin/bash
echo "TERMUX: Setting up x11 server"
# Set custom tmp path

# Kill Termux:X11 process if running
pkill termux-x11 2>/dev/null
am broadcast -a com.termux.x11.ACTION_STOP -p com.termux.x11

# Wait briefly to ensure shutdown
sleep 1

# Remove stale socket and lock files
TMPDIR="$PREFIX/tmp"
rm -rf "$TMPDIR/.X11-unix" "$TMPDIR/.X1-lock"

# Export display
export DISPLAY=:1

# Check if Termux:X11 Android activity is running
if ! pidof termux-x11 >/dev/null; then
    echo "Termux:X11 activity not running. Starting it..."
    am start -n com.termux.x11/.MainActivity
    sleep 2  # Give it time to initialize
else
    echo "Termux:X11 activity is already running."
fi

# Start Termux:X11 (non-legacy, no desktop)
termux-x11 :1 -legacy-drawing &

