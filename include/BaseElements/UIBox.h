#pragma once
#include "SurfaceEditor.h"
#include "Geometry.h"
#include "SDL_Helpers.h"
#include "BaseElements/UIDynamicElement.h"

class UIBox : public UIDynamicElement
{
protected:
    int _radius = 0;
    int _borderWidth = 0;
    RGBA _borderColor;

    SDL_Surface *makeSurface();
    void updateSurface();
    int getMaxRadius();

public:
    void setW(int w) override;
    void setH(int h) override;

    void setRadius(int radius);
    int getRadius();

    void setBorderWidth(int borderWidth);
    int getBorderWidth();

    void setBorderColor(RGBA borderColor);
    RGBA getBorderColor();

    UIBox(std::string name, CommonObjects *comm);
    UIBox(
        std::string name,
        CommonObjects *comm, int w, int h,
        int radius = 0, RGBA color = RGBA(),
        int borderWidth = 0, RGBA borderColor = {100, 100, 100});
};