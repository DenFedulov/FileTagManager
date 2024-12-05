#include "ElementGroups/MainContents.h"

MainContents::MainContents(CommonObjects *comm) : ElementGroup(comm)
{
    this->createElementGroup();
}

void MainContents::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("mainContent", this->comm, w, h - G_App::HEADER_HEIGHT, 0, RGBA(80, 80, 80));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->y = G_App::HEADER_HEIGHT;

    auto sidebar = std::make_shared<UIBox>("sidebar", this->comm, this->SIDEBAR_WIDTH, 1, 0, RGBA(100, 100, 100));
    sidebar->anchors[Direction::Down] = true;
    sidebar->overflow = OverflowMode::Scroll;
    std::vector<std::shared_ptr<UIElement>> tags;
    for (int i = 0; i < 15; i++)
    {
        // if (i == 3)
        // {
        //     TagElement tag(this->comm, "test tag asdasdawdawadsfasdfasdsfhsdfhsdfvnsdrfgnsdfghgggggggggggggggsdfghsfdhgdfghdfghdfghsftg;ohnijusfgpsipiuhmsdgroimpusbdhfomiubhsvsogimuvdyfsogimfhudygsodfihmuydsoghiruvdgsivhoumlfgfdvihksumdfghrfhndfghdfghdrfthbdftghdfghdfghdfghdfghdfghjdfghdfvgsdfvgdxvgbdfghbdfbxcvbxcfgbcfgbcfgbfdfaewrhbgafdhgzsdgaserfhgsdfgdaszdawdawdawd" + std::to_string(i), RGBA(18, 198, 255, 120));
        //     tag.getParentElement()->overflow = OverflowMode::Scroll;
        //     tag.getParentElement()->scrollDirection = Direction::Right;
        //     tags.push_back(tag.getParentElement());
        //     continue;
        // }
        TagElement tag(this->comm, "test tag " + std::to_string(i), RGBA(18, 198, 255, 120));
        tags.push_back(tag.getParentElement());
    }

    auto sidebarContents = std::make_shared<UIElement>("sidebarContents", this->comm);
    sidebarContents->anchors[Direction::Down] = true;
    sidebarContents->anchors[Direction::Right] = true;
    sidebarContents->displayMode = DisplayMode::Distribute;
    sidebarContents->childrenDistPos = RelPos::Start;
    sidebarContents->childrenAlignPos = RelPos::Start;
    sidebarContents->margin[Direction::Left] = 5;
    sidebarContents->margin[Direction::Right] = 5;
    sidebarContents->addChildren(tags);

    sidebar->addChildren({sidebarContents});

    FileExplorer explorer(this->comm);

    this->_parentElement->addChildren({sidebar,
                                       explorer.getParentElement()});
}