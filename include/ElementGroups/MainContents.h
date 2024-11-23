#pragma once
#include "Elements/UICircle.h"
#include "ElementGroups/ElementGroup.h"

class MainContents : public ElementGroup
{
private:
    void createElementGroup();

public:
    const int SIDEBAR_WIDTH = 350;
    MainContents(CommonObjects *comm);
};