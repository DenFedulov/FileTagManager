#pragma once
#include <string>

enum class AppEventType
{
    None,
    NewChild,
    Forward,
    Backward,
    OpenDir,
    FolderUp,
    EditText,
    CloseContextMenu,
    TagActionChange,
    SortChange,
    FilterChange,
    TagsChanged,
    TagFilterChanged,
};

struct AppEvent
{
    AppEventType type = AppEventType::None;
    std::wstring eventText = L"";
};

enum class EventResultType
{
    Nothing,
    Quit,
    StopPropagation,
    RemoveElement,
    AddElement,
    AddElementToMain,
};

template <typename ResultDataType>
struct EventResult
{
    int type = 0;
    ResultDataType data;
};