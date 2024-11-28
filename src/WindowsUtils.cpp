#include "WindowsUtils.h"

std::wstring getClipboardText()
{
    if (!OpenClipboard(nullptr))
    {
        return L"";
    }
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == nullptr)
    {
        CloseClipboard();
        return L"";
    }
    wchar_t *pszText = static_cast<wchar_t *>(GlobalLock(hData));
    if (pszText == nullptr)
    {
        CloseClipboard();
        return L"";
    }
    std::wstring text = pszText;
    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}

bool isKeyPressed(int keyCode)
{
    SHORT keyState = GetAsyncKeyState(keyCode);
    if (keyState & (1 << 15))
    {
        return true;
    }
    return false;
}
