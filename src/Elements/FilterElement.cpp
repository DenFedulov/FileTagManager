#include "Elements/FilterElement.h"

FilterElement::FilterElement(std::string name, CommonObjects *comm, std::wstring text, int fontSize, RGBA color)
    : UIText(name, comm, text, fontSize, color)
{
}

void FilterElement::setText(std::wstring text)
{
    UIText::setText(text);
    this->comm->state->fileFilter = this->getText();
    this->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::FilterChange));
}