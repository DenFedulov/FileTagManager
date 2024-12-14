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

RGBA hexStrToRGBA(const char *str)
{
    uint8_t r, g, b;
    sscanf(str, "%02hhx%02hhx%02hhx", &r, &g, &b);
    return RGBA(r, g, b);
}

std::string uint8ToHexStr(uint8_t number)
{
    return std::format("{:02x}", number);
}

std::string RGBAToHexStr(const RGBA &color)
{
    return uint8ToHexStr(color.r) + uint8ToHexStr(color.g) + uint8ToHexStr(color.b);
}

std::string wstrToHex(const std::wstring &wsrt)
{
	std::string result = "";
	const wchar_t *wideData = wsrt.c_str();
	for (size_t i = 0; i < wsrt.size(); i++)
	{
		result += std::format("{:04x}", static_cast<unsigned long long>(wideData[i]));
	}
	return result;
}

std::wstring hexStrToWStr(std::string hex)
{
	std::wstring result = L"";
	for (size_t i = 0; i < hex.size() / 4; i++)
	{
		result += (static_cast<wchar_t>(std::stoull(hex.substr(i * 4, 4), nullptr, 16)));
	}
	return result;
}