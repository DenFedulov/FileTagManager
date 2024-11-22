#pragma once
#include "Elements/BaseElements/UIBox.h"

class UIDivider : public UIBox
{
public:
    UIDivider(std::string name, CommonObjects *comm, int w, int h, RGBA color = RGBA(0, 0, 0));
};

class UICircle : public UIBox
{
public:
    UICircle(std::string name, CommonObjects *comm, int radius, RGBA color = RGBA(255, 255, 255));
};