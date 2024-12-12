#pragma once
#include <string>
#include <codecvt>
#include <locale>
#include "custom_types.h"
#include "unicode/unistr.h"

uint16string wStrToUInt16(const std::wstring &wStr);
std::wstring strToWStr(const char *str);
std::string wstrToStr(const std::wstring &wStr);
RGBA hexStrToRBGA(const char *str);