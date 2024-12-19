#pragma once
#include "BaseElements/UIElement.h"

class UIFileElement : public UIElement
{
public:
    std::wstring filePath;
    UIFileElement(std::string name, CommonObjects *comm, std::wstring filePath);
};