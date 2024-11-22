#include "SurfaceEditor.h"

SurfaceEditor::SurfaceEditor(SDL_Surface *surface)
{
    if (surface->format->format != SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32)
    {
        throw std::invalid_argument("Surface format must be SDL_PIXELFORMAT_RGBA32");
    }
    this->_surface = surface;
}

void SurfaceEditor::setDrawColor(RGBA color)
{
    this->_color = color;
}

void SurfaceEditor::setSurfacePixel(int x, int y)
{
    Setter::setInRange(x, 0, this->_surface->w - 1);
    Setter::setInRange(y, 0, this->_surface->h - 1);
    uint8_t *pixels = (uint8_t *)this->_surface->pixels;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 0] = this->_color.r;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 1] = this->_color.g;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 2] = this->_color.b;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 3] = this->_color.a;
}

void SurfaceEditor::setSurfacePixelRow(int x1, int x2, int y)
{
    Setter::swap(x1, x2, x1 > x2);
    Setter::setInRange(x1, 0, this->_surface->w - 1);
    Setter::setInRange(x2, 0, this->_surface->w - 1);
    Setter::setInRange(y, 0, this->_surface->h - 1);
    uint8_t *pixels = (uint8_t *)this->_surface->pixels;
    for (int x = x1; x <= x2; x++)
    {
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 0] = this->_color.r;
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 1] = this->_color.g;
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 2] = this->_color.b;
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 3] = this->_color.a;
    }
}

void SurfaceEditor::setSurfacePixelColumn(int x, int y1, int y2)
{
    Setter::swap(y1, y2, y1 > y2);
    Setter::setInRange(x, 0, this->_surface->w - 1);
    Setter::setInRange(y1, 0, this->_surface->h - 1);
    Setter::setInRange(y2, 0, this->_surface->h - 1);
    uint8_t *pixels = (uint8_t *)this->_surface->pixels;
    for (int y = y1; y <= y2; y++)
    {
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 0] = this->_color.r;
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 1] = this->_color.g;
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 2] = this->_color.b;
        pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 3] = this->_color.a;
    }
}

void SurfaceEditor::fillRectangleH(int x1, int y1, int x2, int y2)
{
    Setter::swap(x1, x2, x1 > x2);
    for (int y = y1; y < y2; y++)
    {
        this->setSurfacePixelRow(x1, x2, y);
    }
}

void SurfaceEditor::fillRectangleV(int x1, int y1, int x2, int y2)
{
    Setter::swap(y1, y2, y1 > y2);
    for (int x = x1; x < x2; x++)
    {
        this->setSurfacePixelColumn(x, y1, y2);
    }
}

void SurfaceEditor::fillRectangle(int x1, int y1, int x2, int y2)
{
    SDL_LockSurface(this->_surface);
    Setter::swap(x1, x2, x1 > x2);
    Setter::swap(y1, y2, y1 > y2);
    x1 - x2 < y1 - y2 ? this->fillRectangleH(x1, y1, x2, y2) : this->fillRectangleV(x1, y1, x2, y2);
    SDL_UnlockSurface(this->_surface);
}

void SurfaceEditor::drawWithCoordsVector(const CoordsVector &coords, int x1, int y1, int x2, int y2)
{
    SDL_LockSurface(this->_surface);
    Setter::setIf(x2, this->_surface->w, x2 < 0);
    Setter::setIf(y2, this->_surface->h, y2 < 0);
    for (const intPair &coord : coords)
    {
        int x = coord.first + x1;
        int y = coord.second + y1;
        Setter::setIf(x, x2, x > x2);
        Setter::setIf(y, y2, y > y2);
        this->setSurfacePixel(x, y);
    }

    SDL_UnlockSurface(this->_surface);
}

void SurfaceEditor::fillWithCoordsVector(const CoordsVector &coords, int baseLine, bool vertical, int x1, int y1, int x2, int y2)
{
    SDL_LockSurface(this->_surface);
    Setter::setIf(x2, this->_surface->w, x2 < 0);
    Setter::setIf(y2, this->_surface->h, y2 < 0);
    for (const intPair &coord : coords)
    {
        int x = coord.first + x1;
        int y = coord.second + y1;
        int fillTarget = baseLine;
        Setter::setIf(x, x2, x > x2);
        Setter::setIf(y, y2, y > y2);
        if (vertical)
        {
            Setter::setInRange(fillTarget, y1, y2 - 1);
            this->setSurfacePixelColumn(x, y, fillTarget);
        }
        else
        {
            Setter::setInRange(fillTarget, x1, x2 - 1);
            this->setSurfacePixelRow(x, fillTarget, y);
        }
    }
    SDL_UnlockSurface(this->_surface);
}
