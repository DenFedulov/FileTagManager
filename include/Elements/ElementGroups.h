#pragma once
#include "Elements/BaseElements/UIElement.h"
#include "Elements/BaseElements/UIPictureElement.h"
#include "Elements/BaseElements/UIText.h"
#include "Elements/BaseElements/UIBox.h"
#include "AppGlobals.h"

class ElementGroup
{
protected:
    CommonObjects *_comm;
    std::shared_ptr<UIElement> _parentElement = NULL;

public:
    ElementGroup(CommonObjects *comm);
    virtual ~ElementGroup() {};
    virtual void createElementGroup() = 0;
    std::shared_ptr<UIElement> getParentElement();
};

class HeaderBar : public ElementGroup
{
private:
    void createElementGroup();

public:
    HeaderBar(CommonObjects *comm);
};

class MainContents : public ElementGroup
{
private:
    void createElementGroup();

public:
    const int SIDEBAR_WIDTH = 350;
    MainContents(CommonObjects *comm);
};