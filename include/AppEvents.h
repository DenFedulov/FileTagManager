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