#include "Elements/UICircle.h"

UICircle::UICircle(std::string name, CommonObjects *comm, int radius, RGBA color) : UIBox(name, comm)
{
    this->_w = radius * 2;
    this->_h = radius * 2;
    this->_radius = radius;
    this->updateSurface();
}
