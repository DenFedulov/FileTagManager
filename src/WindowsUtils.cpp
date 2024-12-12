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

std::vector<std::string> getDrivesList()
{
    DWORD drives = GetLogicalDrives();
    const int count = std::popcount(drives);
    char *arr = new char[count * 4 + 1];
    GetLogicalDriveStrings(count * 4 + 1, arr);
    std::vector<std::string> result;
    for (int i = 0; strlen(&arr[i]) > 0; i += strlen(&arr[i]) + 1)
    {
        result.push_back(std::string(&arr[i]));
    }
    delete[] arr;
    return result;
}

void showFileInExplorer(const std::wstring &path)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        return;
    }
    PIDLIST_ABSOLUTE pidlFolder = nullptr;
    SFGAOF attributes = 0;
    hr = SHParseDisplayName(path.c_str(), nullptr, &pidlFolder, 0, &attributes);

    if (SUCCEEDED(hr) && pidlFolder)
    {
        hr = SHOpenFolderAndSelectItems(pidlFolder, 0, nullptr, 0);
        CoTaskMemFree(pidlFolder);
    }
    CoUninitialize();
}
