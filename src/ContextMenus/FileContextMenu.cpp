#include "ContextMenus/FileContextMenu.h"

FileContextMenu::FileContextMenu(CommonObjects *comm, std::wstring filePath)
    : ContextMenu(comm),
      _filePath(filePath)
{
}
std::vector<std::shared_ptr<UIElement>> FileContextMenu::make()
{
    UIButton ShowInExplorerButton(this->comm, L"Show in explorer", 12, RGBA(100, 100, 100), 5);
    ShowInExplorerButton.getElement()->anchors[Direction::Right] = true;
    auto ShowInExplorer = [filePath = this->_filePath](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        showFileInExplorer(filePath);
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::CloseContextMenu));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    ShowInExplorerButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, ShowInExplorer);
    UIButton AddTagToFileButton(this->comm, L"Add tag to file", 12, RGBA(100, 100, 100), 5);
    AddTagToFileButton.getElement()->anchors[Direction::Right] = true;
    auto AddTagToFile = [filePath = this->_filePath](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->state->tagActionMode = (int)TagActionMode::AddToFile;
        el->comm->state->addTagPath = filePath;
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::CloseContextMenu));
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagActionChange));
        std::cout << "option event\n";
        return EventResult<std::shared_ptr<UIElement>>();
    };
    AddTagToFileButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, AddTagToFile);
    return std::vector<std::shared_ptr<UIElement>>(
        {
            ShowInExplorerButton.getElement(),
            AddTagToFileButton.getElement(),
        });
}