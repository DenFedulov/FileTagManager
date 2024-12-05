#pragma once
#include <string>
#include <vector>
#include <bit>
#include <windows.h>

std::wstring getClipboardText();
bool isKeyPressed(int keyCode);
std::vector<std::string> getDrivesList();
