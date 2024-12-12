#pragma once
#include <string>
#include <cwctype>
#include "Setter.h"

namespace Str
{
    bool endsWith(std::string const &subject, std::string const &ending);
    int distanceToWSpace(const std::wstring &str, int step, int offset = 0);
    std::string getTailByChar(const std::string &str, const char *c, bool excludeChar = true);
    std::wstring getTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar = true);
    std::wstring cutTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar = true);
}