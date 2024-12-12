#include "type_helpers.h"

uint16string wStrToUInt16(const std::wstring &wStr)
{
    uint16string result(wStr.begin(), wStr.end());
    return result;
}

std::wstring strToWStr(const char *str)
{
    icu::UnicodeString ustr = icu::UnicodeString::fromUTF8(str);
    std::wstring result(ustr.getBuffer(), ustr.getBuffer() + ustr.length());
    return result;
}

std::string wstrToStr(const std::wstring &wStr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wStr);
}

RGBA hexStrToRBGA(const char *str)
{
    uint8_t r, g, b;
    sscanf(str, "%02hhx%02hhx%02hhx", &r, &g, &b);
    return RGBA(r, g, b);
}
