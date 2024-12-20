#pragma once
#include <string>
#include <unordered_set>

enum class TagActionMode
{
    Select,
    AddToFile,
    DeleteFromFile,
    Delete,
};

enum class SortMode
{
    None,
    Ascending,
    Descending
};

enum class TagFilterMode
{
    Any,
    All
};

struct AppState
{
    int tagActionMode = (int)TagActionMode::Select;
    std::wstring addTagPath = L"";
    int typeSortMode = (int)SortMode::Ascending;
    int nameSortMode = (int)SortMode::Ascending;
    std::wstring fileFilter = L"";
    std::unordered_set<std::string> selectedTags;
    bool tagFilterMode = (bool)TagFilterMode::Any;
};
