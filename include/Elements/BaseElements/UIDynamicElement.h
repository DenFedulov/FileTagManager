#pragma once
#include "Elements/BaseElements/UIElement.h"
#include "Elements/BaseElements/Dynamic.h"

class UIDynamicElement : public UIElement, Dynamic
{
protected:
    SDL_Surface *makeSurface();
    UIDynamicElement(std::string name, CommonObjects *comm);
    ~UIDynamicElement() override;

public:
    void setW(int w) override;
    void setH(int h) override;
};