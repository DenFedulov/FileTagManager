#include "Elements/Elements.h"

UIDivider::UIDivider(std::string name, CommonObjects *comm, int w, int h, RGBA color) : UIBox(name, comm, w, h, 0, color)
{
}

UICircle::UICircle(std::string name, CommonObjects *comm, int radius, RGBA color) : UIBox(name, comm)
{
    this->w = radius * 2;
    this->h = radius * 2;
    this->_radius = radius;
    this->updateSurface();
}
