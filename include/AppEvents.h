#pragma once

enum class AppEventType
{
    None,
    Forward,
    Backward,
    FolderUp,
};

struct AppEvent
{
    AppEventType type = AppEventType::None;
};

enum class EventResultType
{
    Nothing,
    Quit,
    StopPropagation,
};

template <typename ResultDataType>
struct EventResult
{
    int type = 0;
    ResultDataType data;
};