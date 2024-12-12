#pragma once
#include <string>
#include <vector>
#include <bit>
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>

std::wstring getClipboardText();
bool isKeyPressed(int keyCode);
std::vector<std::string> getDrivesList();
void showFileInExplorer(const std::wstring &path);