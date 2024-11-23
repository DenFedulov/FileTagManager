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
    int pressState = -1;
    std::string text = "";
    int keycode = -1;
};

struct MouseEvent
{
    int pressState = -1;
    intPair pos = {-1, -1};
    int button = -1;
};

struct WindowEvent
{
    SDL_Window *window = NULL;
    int data1;
    int data2;
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
        window_resized,
    };
    Type type;
    KeyEvent keyEvent;
    MouseEvent mouseEvent;
    WindowEvent windowEvent;
};

template <typename... Args>
class EventManager
{
    typedef std::unordered_map<int, std::function<void(Args...)>> CallbackList;
    typedef std::unordered_map<int, CallbackList> EventHandlers;

private:
    EventHandlers _eventHandlers;
    CallbackList &getCallbackList(const int &eventTypeEnum)
    {
        if (!this->_eventHandlers.contains(eventTypeEnum))
        {
            this->_eventHandlers.emplace(eventTypeEnum, CallbackList());
        }
        return this->_eventHandlers.at(eventTypeEnum);
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