#pragma once
#include <string>

enum class TagActionMode
{
    Select,
    Add,
};

enum class SortMode
{
    None,
    Ascending,
    Descending
};

struct AppState
{
    int tagActionMode = (int)TagActionMode::Select;
    std::wstring addTagPath = L"";
    int typeSortMode = (int)SortMode::Ascending;
    int nameSortMode = (int)SortMode::Ascending;
    std::wstring fileFilter = L"";
};
