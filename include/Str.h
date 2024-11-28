#pragma once
#include <string>
#include <cwctype>
#include "Setter.h"

namespace Str
{
    bool endsWith(std::string const &subject, std::string const &ending);
    int distanceToWSpace(const std::wstring &str, int step, int offset = 0);
}