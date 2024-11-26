#pragma once
#include "SDL_Helpers.h"
#include "SDL_ttf.h"
#include "Elements/BaseElements/UIElement.h"
#include "Elements/BaseElements/Dynamic.h"

class UIText : public UIElement, Dynamic
{
private:
    TTF_Font *_font = nullptr;
    std::string _fontPath;
    int _fontSize = 24;
    std::string _text = "";
    RGBA _color;
    int _cursorIndex = -1;

    void loadFont(std::string path);

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

    UIText(std::string name, CommonObjects *comm, std::string text, int fontSize = 24, RGBA color = {0, 0, 0});
    ~UIText() override;
};