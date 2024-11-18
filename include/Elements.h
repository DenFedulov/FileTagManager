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
    std::shared_ptr<UIElement> parentElement = NULL;
    std::vector<std::shared_ptr<UIElement>> childDrawElements;
    void fillChildDrawElements();
    std::unordered_map<std::string, std::shared_ptr<UIElement>> childElements;

public:
    int x = 0;
    int y = 0;
    int z = -1;
    const std::string name;
    intPair pivot = intPair(0, 0);
    RelativePosition pivotPositionV = RelativePosition::None;
    RelativePosition pivotPositionH = RelativePosition::None;
    RelativePosition alignPositionV = RelativePosition::None;
    RelativePosition alignPositionH = RelativePosition::None;
    bool anchors[4] = {0, 0, 0, 0};
    bool visible = true;

    EventManager<std::shared_ptr<UIElement> &, AppEvent &> events;

    int calcRelativePosition(RelativePosition p, bool vertical);
    int calcX();
    int calcY();
    int calcW();
    int calcH();
    int getW();
    int getH();
    virtual void setW(int w);
    virtual void setH(int h);
    void addChild(std::shared_ptr<UIElement> childElement, bool firstCall = true);
    void removeChild(std::string name, bool firstCall = true);
    void setParent(std::shared_ptr<UIElement> parent, bool firstCall = true);
    std::shared_ptr<UIElement> getParent();
    std::shared_ptr<UIElement> getChild(std::string name);
    FileTagManager *getApp();
    virtual bool checkCollision(int x, int y);
    void freeSurface();
    void freeTexture();
    virtual void draw(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    UIElement(std::string name, FileTagManager *app, bool isMainElement = false);
    ~UIElement() override;
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
    int getFontSize();
    std::string getFontPath();
    void setFont(std::optional<int> fontSize = std::nullopt, std::optional<std::string> path = std::nullopt);
    std::string getText();
    void setText(std::string text);
    RGBA getColor();
    void setColor(RGBA color);
    int getCursorIndex();
    void setCursorIndex(int cursorIndex);

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
    void setW(int w) override;
    void setH(int h) override;
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