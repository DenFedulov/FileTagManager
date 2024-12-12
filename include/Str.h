#pragma once
#include <string>
#include <cwctype>
#include <algorithm>
#include <cctype>
#include "Setter.h"

namespace Str
{
    bool endsWith(std::string const &subject, std::string const &ending);
    int distanceToWSpace(const std::wstring &str, int step, int offset = 0);
    std::string getTailByChar(const std::string &str, const char *c, bool excludeChar = true);
    std::wstring getTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar = true);
    std::wstring cutTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar = true);
    template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, std::wstring>::value, T>::type
    toLowerCase(T str)
    {
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        return str;
    }
}