#pragma once
#include "BaseElements/UIBox.h"

class UIDivider : public UIBox
{
public:
    UIDivider(std::string name, CommonObjects *comm, int w, int h, RGBA color = RGBA(0, 0, 0));
};