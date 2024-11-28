#include "BaseElements/UIBox.h"

SDL_Surface *UIBox::makeSurface()
{
    this->freeSurface();
    return SDL_CreateRGBSurfaceWithFormat(0, this->w, this->h, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32);
}

void UIBox::setW(int w)
{
    this->w = w;
    this->updateSurface();
}

void UIBox::setH(int h)
{
    this->h = h;
    this->updateSurface();
}

void UIBox::setRadius(int radius)
{
    this->_radius = radius;
    Setter::setInRange(this->_radius, 0, this->getMaxRadius() - 1);
    this->updateSurface();
}

int UIBox::getRadius()
{
    return this->_radius;
}

void UIBox::setBorderWidth(int borderWidth)
{
    this->_borderWidth = borderWidth;
    this->updateSurface();
}

int UIBox::getBorderWidth()
{
    return this->_borderWidth;
}

void UIBox::setBorderColor(RGBA borderColor)
{
    this->_borderColor = borderColor;
    this->updateSurface();
}

RGBA UIBox::getBorderColor()
{
    return this->_borderColor;
}

void UIBox::updateSurface()
{
    this->_surface = this->makeSurface();
    SurfaceEditor editor(this->_surface);
    editor.setDrawColor(this->_color);
    editor.fillRectangle(this->_radius, 0, this->w - this->_radius, this->h);
    editor.fillRectangle(0, this->_radius, this->w, this->h - this->_radius);
    CoordsVector topLeft = Geometry::mirrorAndAddCoords(
        Geometry::mirrorAndAddCoords(
            Geometry::circleQuater(this->_radius), this->_radius, this->_radius, Geometry::Horizontal),
        this->_radius, this->_radius, Geometry::Vertical);
    CoordsVector topRight = Geometry::mirrorAndAddCoords(Geometry::circleQuater(this->_radius), this->_radius, this->_radius, Geometry::Vertical);
    CoordsVector bottomLeft = Geometry::mirrorAndAddCoords(Geometry::circleQuater(this->_radius), this->_radius, this->_radius, Geometry::Horizontal);
    CoordsVector bottomRight = Geometry::circleQuater(this->_radius);

    editor.fillWithCoordsVector(topLeft, this->_radius, true, 0, 0);
    editor.fillWithCoordsVector(topRight, this->_radius, true, this->w - 1 - this->_radius, 0);
    editor.fillWithCoordsVector(bottomLeft, this->h - 1 - this->_radius, true, 0, this->h - 1 - this->_radius);
    editor.fillWithCoordsVector(bottomRight, this->h - 1 - this->_radius, true, this->w - 1 - this->_radius, this->h - 1 - this->_radius);

    topRight = Geometry::addToCoords(topRight, this->w - 1 - this->_radius, 0);
    bottomLeft = Geometry::addToCoords(bottomLeft, 0, this->h - 1 - this->_radius);
    bottomRight = Geometry::addToCoords(bottomRight, this->w - 1 - this->_radius, this->h - 1 - this->_radius);
    CoordsVector top = Geometry::line({this->_radius, 0}, {this->w - 1 - this->_radius, 0});
    CoordsVector left = Geometry::line({0, this->_radius}, {0, this->h - 1 - this->_radius});
    CoordsVector right = Geometry::line({this->w - 1, this->_radius}, {this->w - 1, this->h - 1 - this->_radius});
    CoordsVector down = Geometry::line({this->_radius, this->h - 1}, {this->w - 1 - this->_radius, this->h - 1});

    CoordsVector countor = Vect::concat<intPair>(topLeft, topRight, bottomLeft, bottomRight, top, left, right, down);
    Geometry::BorderMaker borderMaker(countor, this->_borderWidth);
    CoordsVector border = borderMaker.make();
    editor.setDrawColor(this->_borderColor);
    editor.drawWithCoordsVector(border);

    this->freeTexture();
    this->_texture = surfaceToTexture(this->_comm, this->_surface);
}

int UIBox::getMaxRadius()
{
    return std::min(this->w / 2, this->h / 2);
}

UIBox::UIBox(std::string name, CommonObjects *comm, int w, int h,
             int radius, RGBA color,
             int borderWidth, RGBA borderColor) : UIDynamicElement(name, comm)
{
    this->w = w;
    this->h = h;
    this->_radius = radius;
    Setter::setInRange(this->_radius, 0, this->getMaxRadius() - 1);
    this->_color = color;
    this->_borderWidth = borderWidth;
    this->_borderColor = borderColor;
    this->updateSurface();
}

UIBox::UIBox(std::string name, CommonObjects *comm) : UIDynamicElement(name, comm)
{
}
