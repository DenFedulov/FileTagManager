#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

namespace Events
{
    enum MouseButtons
    {
        None,
        Left,
        Middle,
        Right,
        Back,
        Forward
    };
}

struct KeyEvent
{
    int pressState;
    std::string text;
    int32_t keycode;
    KeyEvent() : pressState(-1),
                 text(""),
                 keycode(-1) {};
};

struct MouseEvent
{
    int pressState;
    intPair pos;
    int32_t button;
    MouseEvent() : pressState(-1),
                   pos({-1, -1}),
                   button(-1) {};
};

struct AppEvent
{
    enum Type
    {
        mouse_button_down,
        mouse_click,
        mouse_button_up,
        mouse_move,
        keyboard_button_down,
        keyboard_button_up,
        text_input,
    };
    Type type;
    KeyEvent keyEvent;
    MouseEvent mouseEvent;
};

template <typename... Args>
class EventManager
{
    typedef std::unordered_map<int, std::function<void(Args...)>> CallbackList;
    typedef std::unordered_map<int, CallbackList> EventHandlers;

private:
    EventHandlers eventHandlers;
    CallbackList &getCallbackList(const int &eventTypeEnum)
    {
        if (!this->eventHandlers.contains(eventTypeEnum))
        {
            this->eventHandlers.emplace(eventTypeEnum, CallbackList());
        }
        return this->eventHandlers.at(eventTypeEnum);
    }

public:
    int addHandler(const int &eventTypeEnum, std::function<void(Args...)> func)
    {
        CallbackList &callbacks = this->getCallbackList(eventTypeEnum);
        int index = callbacks.size();
        callbacks.emplace(index, func);
        return index;
    }
    bool deleteHandler(const int &eventTypeEnum, int i)
    {
        CallbackList *callbacks = this->getCallbackList(eventTypeEnum);
        if (callbacks->contains(i))
        {
            callbacks->erase(i);
            return true;
        }
        return false;
    }
    void triggerEvent(const int &eventTypeEnum, Args... args)
    {
        CallbackList &callbacks = this->getCallbackList(eventTypeEnum);
        for (auto &[i, func] : callbacks)
        {
            func(std::forward<Args>(args)...);
        }
    }
};