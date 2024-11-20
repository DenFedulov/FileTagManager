#pragma once
#include "AppElementsCommon.h"
#include "app.h"
#include "Elements.h"

class ElementGroup
{
protected:
    FileTagManager *app;
    std::shared_ptr<UIElement> parentElement = NULL;

public:
    ElementGroup(FileTagManager *app);
    virtual ~ElementGroup() {};
    virtual void createElementGroup() = 0;
    std::shared_ptr<UIElement> getParentElement();
};

class HeaderBar : public ElementGroup
{
private:
    void createElementGroup();

public:
    HeaderBar(FileTagManager *app);
};
