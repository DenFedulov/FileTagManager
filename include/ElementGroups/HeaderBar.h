#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIPictureElement.h"
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "type_helpers.h"

class HeaderBar : public ElementGroup
{
public:
    HeaderBar(CommonObjects *comm);
    std::shared_ptr<UIElement> getElement();
};