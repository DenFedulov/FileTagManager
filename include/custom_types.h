#pragma once
#include <vector>
#include <cstdint>

typedef std::pair<int, int> intPair;
typedef std::vector<intPair> CoordsVector;
struct RGBA
{
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;
};

struct RGB
{
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
};

enum Direction
{
    Up,
    Right,
    Down,
    Left
};