#pragma once
#include <vector>
#include <cstdint>
#include <string>

typedef std::pair<int, int> intPair;
typedef std::vector<intPair> CoordsVector;
typedef unsigned short uint16;
typedef std::basic_string<uint16> uint16string;

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