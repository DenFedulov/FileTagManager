#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "type_helpers.h"

class InputBox : public ElementGroup
{
private:
    std::shared_ptr<UIText> _inputElement;

public:
    int w;
    int h = 24;
    int borderRadius = 5;
    int borderWidth = 2;
    int fontSize = 14;
    std::wstring placeholder = L"";
    InputBox(CommonObjects *comm,
             int w,
             std::shared_ptr<UIText> inputElement = nullptr);
    std::shared_ptr<UIElement> getElement();
    std::shared_ptr<UIText> getInputElement();
};