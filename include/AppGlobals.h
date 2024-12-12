#pragma once
#include <string>

namespace G_App
{
    const int WIDTH = 1280, HEIGHT = 720;
    const int HEADER_HEIGHT = 32;
    const int RESIZE_PADDING = 6;
    const std::string APP_NAME = "FileTagManager";
    const std::string RESOURCES_PATH = "resources/";
    const std::string IMAGES_PATH = RESOURCES_PATH + "images/";
    const std::string AUDIO_PATH = RESOURCES_PATH + "audio/";
    const std::string FONTS_PATH = RESOURCES_PATH + "fonts/";
    const std::string CONFIG_FILENAME = "config.json";
    const std::string DB_FILENAME = APP_NAME + ".db";
    const std::wstring DEFAULT_PATH = L"<select disk>";
}