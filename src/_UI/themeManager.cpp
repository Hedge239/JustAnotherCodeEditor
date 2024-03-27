#include "JACE/_UI/themeManager.h"

#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"

#include <string>
#include <filesystem>
#include <fstream>


std::string g_AppTheme; // themeManager will handle all the gfx and color picking, so we will just keep a global var here


void app::UI::themeManager::InitThemeManager()
{
    app::common::log::LogToFile("application", "[THEME_MANAGER] Initializing");

    if(!std::filesystem::exists(app::common::global::APPDATA + "\\themes\\theme.ini"))
    {
        std::ofstream themeFile;
        std::filesystem::create_directory(app::common::global::APPDATA + "\\themes");

        themeFile.open(app::common::global::APPDATA + "\\themes\\theme.ini");
        themeFile.close();
        
        app::common::log::LogToFile("application", "[THEME_MANAGER] Faild to locate theme.ini : Creating");
        app::common::log::LogToFile("application", "[THEME_MANAGER] Theme set to 'DEFAULT'");

        g_AppTheme = "DEFAULT";
        return;
    }

    // Get theme from file
    std::ifstream themeFile;
    themeFile.open(app::common::global::APPDATA + "\\themes\\theme.ini");

    if(themeFile.is_open())
    {
        // I know that file manager has this function built in, but like it was allready here and I am to lazy to remove it
        std::string line;
        std::getline(themeFile, line);

        if(line.empty())
        {
            app::common::log::LogToFile("application", "[THEME_MANAGER] No theme detected in 'theme.ini' defaulting");
            g_AppTheme = "DEFAULT";
        }else if(!std::filesystem::exists(app::common::global::APPDATA + "\\themes\\" + line))
        {
            app::common::log::LogToFile("application", "[THEME_MANAGER] Invalid '.theme': " + app::common::global::APPDATA + "\\themes\\" + line + " : Defaulting");
            g_AppTheme = "DEFAULT";
        }else
        {
            g_AppTheme = app::common::global::APPDATA + "\\themes\\" + line;
        }

    }else
    {
        app::common::log::LogToFile("application", "[THEME_MANAGER] [ERROR] Faild to load 'theme.ini' defaulting");
        g_AppTheme = "DEFAULT";
    }

    app::common::log::LogToFile("application", "[THEME_MANAGER] Theme set to: " + g_AppTheme);
}