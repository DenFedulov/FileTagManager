#pragma once
#include <optional>
#include "CommonObjects.h"
#include "BaseElements/Renderable.h"
#include "Layout.h"
#include "custom_types.h"
#include "EventManager.h"
#include "Setter.h"

class UIElement : public Renderable
{
protected:
    SDL_Surface *_surface = NULL;
    SDL_Texture *_texture = NULL;
    CommonObjects *_comm;
    std::shared_ptr<Hitbox> _cropRect;
    SDL_Rect _lastDest;
    SDL_Rect _lastCrop;
    int _w = 0;
    int _h = 0;
    int _z = -1;
    int _scrollH = 0;
    int _scrollV = 0;

    int calcChildRealW();
    int calcChildRealH();
    int getMaxChildW();
    int getMaxChildH();
    intPair calcChildWrapping(int childInd);
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
                  int pivotDefault,
                  Direction scrollDirection);
    bool hasCropRect();

public:
    int defaultRenderOrder = 0;
    int id = -1;
    int childIndex = -1;
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
    std::shared_ptr<UIElement> parentElement = NULL;
    std::vector<std::shared_ptr<UIElement>> childElements;
    EventManager<std::shared_ptr<UIElement> &, const SDL_Event &> events;

    int calcX();
    int calcY();

    int calcW();
    int getW();
    virtual void setW(int w);

    int calcH();
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

    void addChildren(const std::vector<std::shared_ptr<UIElement>> &childElements);
    void removeChild(int id);
    std::shared_ptr<UIElement> getChild(int id);
    CommonObjects *getCommonObjects();
    virtual bool checkCollision(int x, int y);
    void freeSurface();
    void freeTexture();
    virtual void draw();
    virtual void render(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    UIElement(std::string name, CommonObjects *comm);
    ~UIElement() override;
    SDL_Texture *getTexture();
};