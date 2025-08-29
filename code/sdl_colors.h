#pragma once

#include <SDL.h>

typedef struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

// Predefined color list
const Color RED     = {255, 0, 0};
const Color GREEN   = {0, 255, 0};
const Color BLUE    = {0, 0, 255};
const Color WHITE   = {255, 255, 255};
const Color BLACK   = {0, 0, 0};
const Color YELLOW  = {255, 255, 0};
const Color CYAN    = {0, 255, 255};
const Color MAGENTA = {255, 0, 255};
const Color GRAY    = {128, 128, 128};
const Color ORANGE  = {255, 165, 0};

