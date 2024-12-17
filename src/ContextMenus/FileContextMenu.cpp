#include "ContextMenus/FileContextMenu.h"

FileContextMenu::FileContextMenu(CommonObjects *comm, std::wstring filePath)
    : ContextMenu(comm),
      _filePath(filePath)
{
}
std::vector<std::shared_ptr<UIElement>> FileContextMenu::make()
{
    UIButton AddTagToFileButton(this->comm, L"Add tag to file", 12, RGBA(100, 100, 100), 5);
    AddTagToFileButton.getElement()->anchors[Direction::Right] = true;
    auto addTagToFile = [filePath = this->_filePath](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->state->tagActionMode = (int)TagActionMode::Add;
        el->comm->state->addTagPath = filePath;
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::CloseContextMenu));
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagActionChange));
        std::cout << "option event\n";
        return EventResult<std::shared_ptr<UIElement>>();
    };
    AddTagToFileButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, addTagToFile);
    return std::vector<std::shared_ptr<UIElement>>(
        {
            AddTagToFileButton.getElement(),
        });
}