#pragma once
#include "SDL_Helpers.h"
#include "type_helpers.h"
#include "SDL_ttf.h"
#include "SurfaceEditor.h"
#include "BaseElements/UIDynamicElement.h"
#include "WindowsUtils.h"
#include "Str.h"

class UIText : public UIDynamicElement
{
private:
    TTF_Font *_font = nullptr;
    std::string _fontPath;
    int _fontSize = 24;
    int _wrapSize = 0;
    std::wstring _text = L"";
    std::wstring _placeholder = L"";
    int _cursorIndex = -1;
    bool _editing = false;
    std::vector<std::wstring> _textHistory;
    int _historyCursor = 1;

    void loadFont(std::string path);
    void updateSurface();

public:
    bool editable = false;
    bool wrap = true;
    int getFontSize();
    std::string getFontPath();
    void setFont(std::optional<int> fontSize = std::nullopt, std::optional<std::string> path = std::nullopt);
    std::wstring getText();
    void setText(std::wstring text);
    std::wstring getPlaceholder();
    void setPlaceholder(std::wstring placeholder);
    int getCursorIndex();
    void setCursorIndex(int cursorIndex);

    int getWrapSize();
    void setWrapSize(int wrapSize);

    void undo();
    void redo();

    UIText(std::string name, CommonObjects *comm, std::wstring text, int fontSize = 24, RGBA color = {255, 255, 255});
    ~UIText() override;
};