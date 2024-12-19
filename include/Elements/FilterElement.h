#pragma once
#include "BaseElements/UIText.h"

class FilterElement : public UIText
{
public:
    FilterElement(std::string name, CommonObjects *comm, std::wstring text, int fontSize = 24, RGBA color = {255, 255, 255});
    void setText(std::wstring text) override;
};