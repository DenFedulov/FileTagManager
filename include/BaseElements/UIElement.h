#pragma once
#include <optional>
#include <algorithm>
#include "CommonObjects.h"
#include "BaseElements/Renderable.h"
#include "Layout.h"
#include "custom_types.h"
#include "EventManager.h"
#include "Setter.h"
#include "AppEvents.h"
#include "vect.h"
#include "DynamicMap.h"

class UIElement : public Renderable
{
protected:
    SDL_Surface *_surface = NULL;
    SDL_Texture *_texture = NULL;
    std::shared_ptr<Hitbox> _cropRect;
    SDL_Rect _lastDest;
    SDL_Rect _lastCrop;
    int _w = 0;
    int _h = 0;
    int _z = -1;
    int _scrollH = 0;
    int _scrollV = 0;

    int getMaxChildW();
    int getMaxChildH();
    ChildWrappingData calcChildWrapping(size_t toChild);
    int calcCoordRelToParent(
        int baseCoord,
        int pCalc,
        DistDirection mainDistDirection,
        std::function<int(RelPos)> calcDistPos,
        RelPos distPos,
        std::function<int(RelPos)> calcAlignPos,
        RelPos alignPos,
        RelPos pivotPos,
        int pivotDim,
        int pivotDefault);
    int calcCoord(int baseCoord,
                  Direction anchorDirection,
                  int pCalc,
                  DistDirection mainDistDirection,
                  std::function<int(RelPos)> calcDistPos,
                  RelPos distPos,
                  std::function<int(RelPos)> calcAlignPos,
                  RelPos alignPos,
                  RelPos pivotPos,
                  int pivotDim,
                  int pivotDefault);
    bool hasCropRect();

public:
    int defaultRenderOrder = 0;
    size_t id = 0;
    size_t childId = 0;
    size_t childIndex = 0;
    int x = 0;
    int y = 0;
    int margin[4] = {0, 0, 0, 0};
    std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>();
    const std::string name;
    intPair pivot = intPair(0, 0);
    RelPos pivotPosH = RelPos::None;
    RelPos pivotPosV = RelPos::None;
    RelPos childrenPivotPos = RelPos::None;
    RelPos distPosH = RelPos::None;
    RelPos distPosV = RelPos::None;
    RelPos childrenDistPos = RelPos::None;
    RelPos alignPosH = RelPos::None;
    RelPos alignPosV = RelPos::None;
    RelPos childrenAlignPos = RelPos::None;
    bool anchors[4] = {0, 0, 0, 0};
    bool visible = true;
    bool showHitbox = false;
    OverflowMode overflow = OverflowMode::Visible;
    Direction scrollDirection = Direction::Down;
    DisplayMode displayMode = DisplayMode::Normal;
    DistDirection distDirection = DistDirection::column;
    PositionMode posMode = PositionMode::Relative;
    bool distibutionWrapping = true;
    std::shared_ptr<UIElement> parentElement = NULL;
    std::shared_ptr<UIElement> groupParentElement = NULL;
    DynamicMap<std::shared_ptr<UIElement>> childElements;
    EventManager<EventResult<std::shared_ptr<UIElement>>, std::shared_ptr<UIElement> &, const SDL_Event &> events;
    EventManager<EventResult<std::shared_ptr<UIElement>>, std::shared_ptr<UIElement> &, const std::shared_ptr<AppEvent> &> appEvents;
    CommonObjects *comm;

    int calcX();
    int calcY();

    int calcW(bool inner = false);
    int getW();
    virtual void setW(int w);

    int calcH(bool inner = false);
    int getH();
    virtual void setH(int h);

    int getZ();
    void setZ(int z);

    void setMargin(int m);

    int getScrollH();
    void setScrollH(int scrollH);
    void moveScrollH(int step);

    int getScrollV();
    void setScrollV(int scrollV);
    void moveScrollV(int step);

    int calcPivotOffsetH(RelPos p);
    int calcPivotOffsetV(RelPos p);

    int calcDistPosH(RelPos p);
    int calcDistPosV(RelPos p);

    int calcAlignPosH(RelPos p);
    int calcAlignPosV(RelPos p);

    int getChildWSum(std::optional<int> upTo = std::nullopt);
    int getChildHSum(std::optional<int> upTo = std::nullopt);
    int calcChildRealW();
    int calcChildRealH();

    int countRelativeChildren();

    static void addChildren(const std::shared_ptr<UIElement> &parentElement, const std::vector<std::shared_ptr<UIElement>> &childElements);
    void updateChildVec();
    void removeChild(size_t id);
    void sortChildVec(bool (*sortFunc)(std::shared_ptr<UIElement>, std::shared_ptr<UIElement>));
    void setDefaultRenderOrder(int order = 0, bool fromMainParent = false);

    std::vector<size_t> getFamilyIndicies();

    CommonObjects *getCommonObjects();
    virtual bool checkCollision(int x, int y);
    void freeSurface();
    void freeTexture();
    void setVisible(bool visible);
    virtual void draw();
    virtual void render(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    UIElement(std::string name, CommonObjects *comm);
    ~UIElement() override;
    SDL_Texture *getTexture();
};