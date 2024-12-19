#include "Elements/UIFileElement.h"

UIFileElement::UIFileElement(std::string name, CommonObjects *comm, std::wstring filePath)
    : UIElement(name, comm),
      filePath(filePath)
{
}