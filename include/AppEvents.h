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
};

struct AppEvent
{
    AppEventType type = AppEventType::None;
    std::wstring newPath = L"";
    bool isDirectory = false;
};

enum class EventResultType
{
    Nothing,
    Quit,
    StopPropagation,
    RemoveElement,
    AddElement
};

template <typename ResultDataType>
struct EventResult
{
    int type = 0;
    ResultDataType data;
};