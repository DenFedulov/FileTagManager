#pragma once
#include "WindowsUtils.h"
#include "BaseElements/UIText.h"
#include "Elements/UICircle.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/TagElement.h"

class MainContents : public ElementGroup
{
private:
    void createElementGroup();

public:
    const int SIDEBAR_WIDTH = 350;
    MainContents(CommonObjects *comm);
};