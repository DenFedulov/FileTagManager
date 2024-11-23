#pragma once
#include "SDL.h"
#include "Setter.h"
#include "custom_types.h"
#include <stdexcept>
#include <exception>

class SurfaceEditor
{
private:
    SDL_Surface *_surface;
    RGBA _color;
    void setSurfacePixel(int x, int y);
    void setSurfacePixelRow(int x1, int x2, int y);
    void setSurfacePixelColumn(int x, int y1, int y2);
    void fillRectangleH(int x1, int y1, int x2, int y2);
    void fillRectangleV(int x1, int y1, int x2, int y2);

public:
    SurfaceEditor(SDL_Surface *surface);
    void setDrawColor(RGBA color);
    void fillRectangle(int x1, int y1, int x2, int y2);
    void drawWithCoordsVector(const CoordsVector &coords, int x1 = 0, int y1 = 0, int x2 = -1, int y2 = -1);
    void fillWithCoordsVector(const CoordsVector &coords, int baseLine, bool vertical, int x1, int y1, int x2 = -1, int y2 = -1);
    void drawHitbox();
};