#include "BaseElements/UIElement.h"

UIElement::UIElement(std::string name, CommonObjects *comm) : name(name), _comm(comm)
{
}

int UIElement::calcCoordRelToParent(
    int baseCoord,
    std::function<int()> pCalc,
    DistDirection mainDistDirection,
    std::function<int(RelPos)> calcDistPos,
    RelPos distPos,
    std::function<int(RelPos)> calcAlignPos,
    RelPos alignPos,
    RelPos pivotPos,
    int pivotDim,
    int pivotDefault)
{
    if (this->parentElement->displayMode == DisplayMode::Distribute)
    {
        if (this->parentElement->distDirection == mainDistDirection)
        {
            if (distPos != RelPos::None ||
                (distPos == RelPos::None && this->parentElement->childrenDistPos != RelPos::None))
            {
                return calcDistPos(
                    distPos != RelPos::None ? distPos : this->parentElement->childrenDistPos);
            }
        }
        else
        {
            if (alignPos != RelPos::None ||
                (alignPos == RelPos::None && this->parentElement->childrenAlignPos != RelPos::None))
            {
                return calcAlignPos(
                    alignPos != RelPos::None ? alignPos : this->parentElement->childrenAlignPos);
            }
        }
    }
    return baseCoord + pCalc() - calcPivotOffset(pivotPos, pivotDim, pivotDefault);
}

int UIElement::calcCoord(
    int baseCoord,
    Direction anchorDirection,
    std::function<int()> pCalc,
    DistDirection mainDistDirection,
    std::function<int(RelPos)> calcDistPos,
    RelPos distPos,
    std::function<int(RelPos)> calcAlignPos,
    RelPos alignPos,
    RelPos pivotPos,
    int pivotDim, // potential overhang
    int pivotDefault,
    Direction marginDirection)
{
    int result;
    if (this->anchors[anchorDirection] && this->parentElement != NULL)
    {
        result = pCalc();
    }
    else if (this->parentElement != NULL)
    {
        result = this->calcCoordRelToParent(
            baseCoord,
            pCalc,
            mainDistDirection,
            calcDistPos,
            distPos,
            calcAlignPos,
            alignPos,
            pivotPos,
            pivotDim,
            pivotDefault);
    }
    else
    {
        result = baseCoord - calcPivotOffset(pivotPos, pivotDim, pivotDefault);
    }
    return result + this->margin[marginDirection];
}

int UIElement::calcX()
{
    return this->calcCoord(
        this->x,
        Direction::Left,
        [this]()
        { return this->parentElement != NULL ? this->parentElement->calcX() : 0; },
        DistDirection::column,
        [this](RelPos p)
        { return this->calcDistPosH(p); },
        this->distPosH,
        [this](RelPos p)
        { return this->calcAlignPosV(p); },
        this->alignPosV,
        this->pivotPosH,
        this->calcW(),
        this->pivot.first,
        Direction::Left);
}

int UIElement::calcY()
{
    return this->calcCoord(
        this->y,
        Direction::Up,
        [this]()
        { return this->parentElement != NULL ? this->parentElement->calcY() : 0; },
        DistDirection::row,
        [this](RelPos p)
        { return this->calcDistPosV(p); },
        this->distPosV,
        [this](RelPos p)
        { return this->calcAlignPosH(p); },
        this->alignPosH,
        this->pivotPosV,
        this->calcH(),
        this->pivot.second,
        Direction::Left);
}

void UIElement::setMargin(int m)
{
    for (int i = 0; i < (sizeof(this->margin) / sizeof(this->margin[0])); i++)
    {
        this->margin[i] = m;
    }
}

int UIElement::calcW()
{
    if (this->anchors[Direction::Right] && this->parentElement != NULL)
    {
        return this->parentElement->calcW();
    }
    return this->w;
}

int UIElement::calcH()
{
    if (this->anchors[Direction::Down] && this->parentElement != NULL)
    {
        return this->parentElement->calcH();
    }
    return this->h;
}

int UIElement::getW()
{
    return this->w + this->margin[Direction::Left] + this->margin[Direction::Right];
}

int UIElement::getH()
{
    return this->h + this->margin[Direction::Up] + this->margin[Direction::Down];
}

void UIElement::setW(int w)
{
    this->w = w;
}

void UIElement::setH(int h)
{
    this->h = h;
}

int UIElement::calcPivotOffsetH(RelPos p)
{
    return calcPivotOffset(p, this->calcW(), this->pivot.first);
}

int UIElement::calcPivotOffsetV(RelPos p)
{
    return calcPivotOffset(p, this->calcH(), this->pivot.second);
}

int UIElement::getChildWSum(std::optional<int> upTo)
{
    int limit = upTo.value_or(this->childElements.size() - 1);
    int sum = 0;
    for (int i = 0; i <= limit; i++)
    {
        sum += this->childElements[i]->getW();
    }
    return sum;
}

int UIElement::getChildHSum(std::optional<int> upTo)
{
    int limit = upTo.value_or(this->childElements.size() - 1);
    int sum = 0;
    for (int i = 0; i <= limit; i++)
    {
        sum += this->childElements[i]->getH();
    }
    return sum;
}

int UIElement::getChildMaxW()
{
    int max = 0;
    for (const auto &child : this->childElements)
    {
        int width = child->calcW();
        if (max < width)
        {
            max = width;
        }
    }
    return max;
}

int UIElement::getChildMaxH()
{
    int max = 0;
    for (const auto &child : this->childElements)
    {
        int height = child->calcH();
        if (max < height)
        {
            max = height;
        }
    }
    return max;
}

int UIElement::calcDistPosH(RelPos p)
{
    int childCount = this->parentElement->childElements.size();
    int wSum = this->parentElement->getChildWSum();
    int wOffset = this->parentElement->getChildWSum(this->childIndex - 1);
    return calcRelPos(
        p,
        this->parentElement->calcX(),
        this->parentElement->calcW(),
        this->childIndex,
        childCount,
        wSum,
        wOffset,
        this->pivot.first);
}

int UIElement::calcDistPosV(RelPos p)
{
    int childCount = this->parentElement->childElements.size();
    int hSum = this->parentElement->getChildHSum();
    int hOffset = this->parentElement->getChildHSum(this->childIndex - 1);
    return calcRelPos(
        p,
        this->parentElement->calcY(),
        this->parentElement->calcH(),
        this->childIndex,
        childCount,
        hSum,
        hOffset,
        this->pivot.second);
}

int UIElement::calcAlignPosH(RelPos p)
{
    RelPos pivotPos = this->pivotPosV != RelPos::None ? this->pivotPosV : this->parentElement->childrenPivotPos;
    int maxH = this->parentElement->getChildMaxH();
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->calcH(), this->pivot.second, maxH) + this->parentElement->calcY();

    int result = calcRelPos(
        p,
        parentOffset,
        this->parentElement->getChildMaxH(),
        0,
        1,
        this->calcH(),
        0,
        this->pivot.second);
    return result;
}

int UIElement::calcAlignPosV(RelPos p)
{
    RelPos pivotPos = this->pivotPosH != RelPos::None ? this->pivotPosH : this->parentElement->childrenPivotPos;
    int maxW = this->parentElement->getChildMaxW();
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->calcW(), this->pivot.first, maxW) + this->parentElement->calcX();

    int result = calcRelPos(
        p,
        parentOffset,
        maxW,
        0,
        1,
        this->calcW(),
        0,
        this->pivot.first);
    return result;
}

bool UIElement::checkCollision(int x, int y)
{
    return x >= this->calcX() && x < this->calcX() + this->calcW() && y >= this->calcY() && y < this->calcY() + this->calcH();
}

void UIElement::freeSurface()
{
    if (this->_surface != nullptr)
    {
        SDL_FreeSurface(this->_surface);
        this->_surface = nullptr;
    }
}

SDL_Texture *UIElement::getTexture()
{
    return this->_texture;
}

void UIElement::freeTexture()
{
    if (this->_texture != nullptr)
    {
        SDL_DestroyTexture(this->_texture);
        this->_texture = nullptr;
    }
}

CommonObjects *UIElement::getCommonObjects()
{
    return this->_comm;
}

UIElement::~UIElement()
{
    this->freeTexture();
}

void UIElement::addChildren(const std::vector<std::shared_ptr<UIElement>> &childElements)
{
    for (auto &childElement : childElements)
    {
        childElement->childIndex = this->childElements.size();
        childElement->parentElement = std::shared_ptr<UIElement>(this);
        this->childElements.push_back(childElement);
    }
}

void UIElement::removeChild(int id)
{
    if (id >= this->childElements.size())
    {
        return;
    }
    auto childElement = this->childElements.at(id);
    this->childElements.erase(this->childElements.begin() + id);
    this->parentElement = NULL;
}

std::shared_ptr<UIElement> UIElement::getChild(int id)
{
    if (id < this->childElements.size())
    {
        return this->childElements.at(id);
    }
    return nullptr;
}

void UIElement::draw(SDL_Point *rotationPoint, double angle, SDL_RendererFlip flip)
{
    if (!this->visible)
    {
        return;
    }
    if (this->_texture != NULL)
    {
        SDL_Rect dest;
        dest.x = this->calcX();
        dest.y = this->calcY();
        dest.w = this->calcW();
        dest.h = this->calcH();
        SDL_RenderCopyEx(this->_comm->renderer, this->_texture, NULL, &dest, angle, rotationPoint, flip);
    }
    if (this->z > -1)
    {
        return;
    }
    for (auto &childElement : this->childElements)
    {
        childElement->draw(rotationPoint, angle, flip);
    }
}