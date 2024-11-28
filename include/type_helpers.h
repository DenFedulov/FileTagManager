#pragma once
#include <string>
#include "custom_types.h"
#include "unicode/unistr.h"

uint16string wStrToUInt16(const std::wstring &wStr);
std::wstring strToWStr(const char *str);
RGBA hexStrToRBGA(const char *str);