#pragma once
#include <string>
#include <cwctype>
#include <algorithm>
#include <cctype>
#include <vector>
#include "Setter.h"

namespace Str
{
    bool endsWith(std::string const &subject, std::string const &ending);
    int distanceToWSpace(const std::wstring &str, int step, int offset = 0);
    template <typename T>
    T getTailByChar(const T &str, const T &c, bool excludeChar = true, bool returnSource = true);
    std::wstring cutTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar = true);
    template <typename T>
    T toLowerCase(T str);
    template <typename T>
    std::vector<T> explode(const T &str, const T &separator);
}