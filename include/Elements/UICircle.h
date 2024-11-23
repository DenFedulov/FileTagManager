#pragma once
#include "BaseElements/UIBox.h"

class UICircle : public UIBox
{
public:
    UICircle(std::string name, CommonObjects *comm, int radius, RGBA color = RGBA(255, 255, 255));
};