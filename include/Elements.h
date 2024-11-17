#pragma once
#include <unordered_map>
#include <string>
#include "AppElementsCommon.h"
#include "SDL.h"
#include "custom_types.h"
#include "EventManager.h"
#include "app.h"
#include "SurfaceEditor.h"
#include "geometry.h"
#include "Setter.h"
#include "SDL_Helpers.h"

class Drawable
{
public:
    virtual ~Drawable() {};
    virtual void draw(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) = 0;
};

class UIElement : public Drawable
{
protected:
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    FileTagManager *app;
    int w = 0;
    int h = 0;

public:
    std::shared_ptr<UIElement> parentElement = NULL;
    std::vector<std::shared_ptr<UIElement>> childElements;

    int x = 0;
    int y = 0;
    const std::string name;
    intPair pivot = intPair(0, 0);
    PivotPosition pivotPositionV = PivotPosition::None;
    PivotPosition pivotPositionH = PivotPosition::None;
    bool visible = true;

    EventManager<std::shared_ptr<UIElement> &, AppEvent &> events;

    int calcPivotPosition(PivotPosition p, bool vertical);
    int calcX();
    int calcY();
    int getW();
    int getH();
    FileTagManager *getApp();
    virtual bool checkCollision(int x, int y);
    void freeSurface();
    void freeTexture();
    virtual void draw(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    UIElement(std::string name, FileTagManager *app, bool isMainElement = false);
    virtual ~UIElement();
    SDL_Texture *getTexture();
};

class Dynamic
{
protected:
    virtual void updateSurface() = 0;

public:
    virtual ~Dynamic() {};
};

class UIText : public UIElement, Dynamic
{
private:
    TTF_Font *font = nullptr;
    std::string fontPath;
    int fontSize = 24;
    std::string text = "";
    RGBA color;
    void loadFont(std::string path);
    int cursorIndex = -1;

protected:
    void updateSurface();

public:
    bool editable = false;
    int calcTextX();
    int calcTextY();
    int getFontSize();
    std::string getFontPath();
    void setFont(std::optional<int> fontSize = std::nullopt, std::optional<std::string> path = std::nullopt);
    std::string getText();
    void setText(std::string text);
    RGBA getColor();
    void setColor(RGBA color);
    bool checkCollision(int x, int y) override;
    int getCursorIndex();
    void setCursorIndex(int cursorIndex);

    void draw(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) override;
    UIText(std::string name, FileTagManager *app, std::string text, int fontSize = 24, RGBA color = {0, 0, 0});
    UIText(std::string name, FileTagManager *app, std::string text, std::shared_ptr<UIElement> parentElement, int fontSize = 24, RGBA color = {0, 0, 0});
    ~UIText() override;
};

class UIPictureElement : public UIElement
{
private:
    SDL_Texture *loadPictureTexture(std::string filename);

public:
    UIPictureElement();
    UIPictureElement(std::string name, FileTagManager *app);
};

class UIDynamicElement : public UIElement, Dynamic
{
protected:
    SDL_Surface *makeSurface();
    UIDynamicElement(std::string name, FileTagManager *app);
    ~UIDynamicElement() override;

public:
    void setW(int w);
    void setH(int h);
};

class UIBox : public UIDynamicElement
{
private:
    void updateSurface();
    int getMaxRadius();
    int radius = 0;
    RGBA color;
    int borderWidth = 0;
    RGBA borderColor;

public:
    void setRadius(int radius);
    int getRadius();

    void setColor(RGBA color);
    RGBA getColor();

    void setBorderWidth(int borderWidth);
    int getBorderWidth();

    void setBorderColor(RGBA borderColor);
    RGBA getBorderColor();

    UIBox(
        std::string name,
        FileTagManager *app, int w, int h,
        int radius = 0, RGBA color = RGBA(),
        int borderWidth = 0, RGBA borderColor = {100, 100, 100});
};