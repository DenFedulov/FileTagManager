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

RGBA SurfaceEditor::getPixel(int x, int y)
{
    uint8_t *pixels = (uint8_t *)this->_surface->pixels;
    uint8_t r = pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 0];
    uint8_t g = pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 1];
    uint8_t b = pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 2];
    uint8_t a = pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 3];
    return RGBA(r, g, b, a);
}

void SurfaceEditor::setPixel(int x, int y, const RGBA &color)
{
    uint8_t *pixels = (uint8_t *)this->_surface->pixels;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 0] = color.r;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 1] = color.g;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 2] = color.b;
    pixels[(x * this->_surface->format->BytesPerPixel) + y * this->_surface->pitch + 3] = color.a;
}

void SurfaceEditor::setSurfacePixel(int x, int y)
{
    Setter::setInRange(x, 0, this->_surface->w - 1);
    Setter::setInRange(y, 0, this->_surface->h - 1);
    this->setPixel(x, y, this->_color);
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
        this->setPixel(x, y, this->_color);
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
        this->setPixel(x, y, this->_color);
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

void SurfaceEditor::drawHitbox()
{
    SDL_LockSurface(this->_surface);
    this->setSurfacePixelColumn(0, 0, this->_surface->h);
    this->setSurfacePixelColumn(this->_surface->w, 0, this->_surface->h);
    this->setSurfacePixelRow(0, this->_surface->w, 0);
    this->setSurfacePixelRow(0, this->_surface->w, this->_surface->h);
    SDL_UnlockSurface(this->_surface);
}

void SurfaceEditor::multiply(const RGBA &color)
{
    SDL_LockSurface(this->_surface);
    for (int y = 0; y < this->_surface->h; y++)
    {
        for (int x = 0; x < this->_surface->w; x++)
        {
            RGBA currentPixel = this->getPixel(x, y);
            currentPixel.r *= ((double)color.r / 255);
            currentPixel.g *= ((double)color.g / 255);
            currentPixel.b *= ((double)color.b / 255);
            currentPixel.a *= ((double)color.a / 255);
            this->setPixel(x, y, currentPixel);
        }
    }
    SDL_UnlockSurface(this->_surface);
}
