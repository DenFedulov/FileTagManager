#pragma once
#include <string>

enum class TagActionMode
{
    Select,
    Add,
};

struct AppState
{
    int tagActionMode = (int)TagActionMode::Select;
    std::wstring addTagPath = L"";
};
