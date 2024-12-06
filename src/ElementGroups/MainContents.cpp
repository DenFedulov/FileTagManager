#include "ElementGroups/MainContents.h"

MainContents::MainContents(CommonObjects *comm) : ElementGroup(comm)
{
    this->createElementGroup();
}

void MainContents::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->_comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("mainContent", this->_comm, w, h - G_App::HEADER_HEIGHT, 0, RGBA(80, 80, 80));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->y = G_App::HEADER_HEIGHT;

    auto sidebar = std::make_shared<UIBox>("sidebar", this->_comm, this->SIDEBAR_WIDTH, h - G_App::HEADER_HEIGHT, 0, RGBA(100, 100, 100));
    sidebar->anchors[Direction::Down] = true;
    sidebar->displayMode = DisplayMode::Distribute;
    sidebar->childrenDistPos = RelPos::Start;
    sidebar->childrenAlignPos = RelPos::Start;
    sidebar->overflow = OverflowMode::Scroll;

    TagElement tag1(this->_comm, "test tag1", RGBA(18, 198, 255, 120));
    TagElement tag2(this->_comm, "test tag2", RGBA(18, 198, 255, 120));
    TagElement tag3(this->_comm, "tag3asrgsadfgsdcgsdfrgsadrhgdfg", RGBA(18, 198, 255, 120));
    tag3.getParentElement()->overflow = OverflowMode::Scroll;
    tag3.getParentElement()->scrollDirection = Direction::Left;
    TagElement tag4(this->_comm, "tag4", RGBA(18, 198, 255, 120));
    TagElement tag5(this->_comm, "tag5", RGBA(18, 198, 255, 120));
    TagElement tag6(this->_comm, "tag5", RGBA(18, 198, 255, 120));
    TagElement tag7(this->_comm, "tag5", RGBA(18, 198, 255, 120));
    TagElement tag8(this->_comm, "tag5", RGBA(18, 198, 255, 120));
    // TagElement tag6(this->_comm, "tag6", RGBA(18, 198, 255, 120));
    sidebar->addChildren({
        tag1.getParentElement(),
        tag2.getParentElement(),
        tag3.getParentElement(),
        tag4.getParentElement(),
        tag5.getParentElement(),
        tag6.getParentElement(),
        tag7.getParentElement(),
        tag8.getParentElement(),
        // tag6.getParentElement(),
    });

    this->_parentElement->addChildren({sidebar});
}