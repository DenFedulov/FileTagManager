#pragma once
#include "AppElementsCommon.h"
#include "app.h"
#include "BaseElements.h"

class UIDivider : public UIBox
{
public:
    UIDivider(std::string name, FileTagManager *app, int w, int h, RGBA color = RGBA(0, 0, 0));
};