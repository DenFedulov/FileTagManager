#include "ElementGroups/FileElement.h"

FileElement::FileElement(CommonObjects *comm, std::string filename) : ElementGroup(comm), filename(filename)
{
    if (filename.ends_with(":\\"))
    {
        this->iconType = IconType::Drive;
    }
    this->createElementGroup();
}

void FileElement::createElementGroup()
{
    this->_parentElement = std::make_shared<UIElement>("file", this->comm);
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->distDirection = DistDirection::row;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->childrenAlignPos = RelPos::Center;
    this->_parentElement->setMargin(5);

    std::string iconPath = G_App::IMAGES_PATH + "file.png";
    switch (this->iconType)
    {
    case IconType::Drive:
        iconPath = G_App::IMAGES_PATH + "hard-drive.png";
        break;
    }
    auto icon = std::make_shared<UIPictureElement>(iconPath, this->comm);
    icon->setH(this->iconSize);
    icon->setW(this->iconSize);

    auto name = std::make_shared<UIText>("filename", this->comm, strToWStr(this->filename.c_str()), this->fontSize);

    this->_parentElement->addChildren({icon, name});
    this->_parentElement->setW(this->iconSize);
    this->_parentElement->setH(this->_parentElement->getChildHSum());
}