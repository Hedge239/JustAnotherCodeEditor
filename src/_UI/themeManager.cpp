#include "JACE/_UI/themeManager.h"

#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"


#include <string>
#include <filesystem>
#include <fstream>

std::string g_AppTheme; // themeManager will handle all the gfx and color picking, so we will just keep a global var here


void app::appUI::themeManager::InitThemeManager()
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\themes\\theme.ini"))
    {
        std::ofstream c_themeFile;
        std::filesystem::create_directory(app::common::global::APPDATA + "\\themes");
        c_themeFile.open(app::common::global::APPDATA + "\\themes\\themes.ini");
        c_themeFile.close();

        app::common::log::LogToFile("application", "[THEME_MANAGER] theme.ini not detected, creating");

        return;
    }

    // Read for theme dir
    std::ifstream themeFile;

    themeFile.open(app::common::global::APPDATA + "\\themes\\theme.ini");
    app::common::log::LogToFile("application", "[THEME_MANAGER] Initializing");

    if(themeFile.is_open())
    {
        std::string line;
        std::getline(themeFile, line);
        
        if(line.empty())
        {
            app::common::log::LogToFile("application", "[THEME_MANAGER] No theme detected in theme.ini");
            return;
        }

        if(!std::filesystem::is_directory(app::common::global::APPDATA + "\\themes\\" + line))
        {
            app::common::log::LogToFile("application", "[THEME_MANAGER] Invalid direcotry: " + app::common::global::APPDATA + "\\themes\\" + line);
            return;
        }

            g_AppTheme = app::common::global::APPDATA + "\\themes\\" + line;
            app::common::log::LogToFile("application", "[THEME_MANAGER] Setting theme too: " + g_AppTheme);
    }else
    {
        app::common::log::LogToFile("application", "[THEME_MANAGER] [ERROR] Faild to load theme.ini, aborting");
        return;
    }
}