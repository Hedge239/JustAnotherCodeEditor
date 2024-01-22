#include "JACE/common/localesHandeler.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include <string>
#include <filesystem>
#include <fstream>


std::string g_appLanguage;

void app::common::Localisation::setAppLanguage()
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\settings.ini"))
    {
        app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to find settings. Relaunch the app, if that fails you did something wrong");
        app::common::log::CreateCrashLog("NO SETTINGS FILE : RESTART APP"); exit(-1);
    }

    g_appLanguage = app::common::fileHandeler::ReadLineFromFile("settings.ini", 1);
    if(g_appLanguage == "")
    {app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to find language, defaulting to english"); g_appLanguage = "english";}

    g_appLanguage = g_appLanguage + ".local";
    app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Language set to: " + g_appLanguage);

    // Should only happen if the file is manually edited
    if(!std::filesystem::exists("locales\\" + g_appLanguage))
    {
        app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] No .local file detected");
        app::common::log::CreateCrashLog("INVALID .LOCAL FILE"); exit(-1);
    }
}   