#pragma once
#include <optional>
#include "CommonObjects.h"
#include "BaseElements/Drawable.h"
#include "Layout.h"
#include "custom_types.h"
#include "EventManager.h"

class UIElement : public Drawable
{
protected:
    SDL_Surface *_surface = NULL;
    SDL_Texture *_texture = NULL;
    CommonObjects *_comm;
    int w = 0;
    int h = 0;

    int getChildMaxW();
    int getChildMaxH();
    intPair calcChildWrapping(int childInd);
    int calcCoordRelToParent(
        int baseCoord,
        std::function<int()> pCalc,
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
                  std::function<int()> pCalc,
                  DistDirection mainDistDirection,
                  std::function<int(RelPos)> calcDistPos,
                  RelPos distPos,
                  std::function<int(RelPos)> calcAlignPos,
                  RelPos alignPos,
                  RelPos pivotPos,
                  int pivotDim,
                  int pivotDefault,
                  Direction marginDirection);

public:
    int id = -1;
    int childIndex = -1;
    int x = 0;
    int y = 0;
    int z = -1;
    int margin[4] = {0, 0, 0, 0};
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
    DisplayMode displayMode = DisplayMode::Normal;
    DistDirection distDirection = DistDirection::column;
    std::shared_ptr<UIElement> parentElement = NULL;
    std::vector<std::shared_ptr<UIElement>> childElements;
    EventManager<std::shared_ptr<UIElement> &, AppEvent &> events;

    int calcX();
    int calcY();
    void setMargin(int m);
    int calcW();
    int calcH();
    int getW();
    virtual void setW(int w);
    int getH();
    virtual void setH(int h);
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
    virtual void draw(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    UIElement(std::string name, CommonObjects *comm);
    ~UIElement() override;
    SDL_Texture *getTexture();
};