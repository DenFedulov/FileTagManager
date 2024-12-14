#pragma once
#include <string>
#include <codecvt>
#include <locale>
#include <format>
#include "custom_types.h"
#include "unicode/unistr.h"

uint16string wStrToUInt16(const std::wstring &wStr);
std::wstring strToWStr(const char *str);
std::string wstrToStr(const std::wstring &wStr);
RGBA hexStrToRGBA(const char *str);
std::string uint8ToHexStr(uint8_t number);
std::string RGBAToHexStr(const RGBA &color);
std::string wstrToHex(const std::wstring &wsrt);
std::wstring hexStrToWStr(std::string hex);
