#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIPictureElement.h"
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"

class HeaderBar : public ElementGroup
{
private:
    void createElementGroup();

public:
    HeaderBar(CommonObjects *comm);
};