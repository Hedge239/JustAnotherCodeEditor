#include "JACE/common/localesHandeler.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>

#define MAX_NUMBER_OF_LINES 1


std::unordered_map<std::string, std::string> g_localtextMem;
std::string g_appLanguage;

void app::common::Localisation::setAppLanguage()
{
    // Find and get language from settings
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\settings.ini"))
    {
        app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to find settings. Relaunch the app, if that fails you did something wrong");
        app::common::log::CreateCrashLog("NO SETTINGS FILE : RESTART APP"); exit(-1);
    }

    g_appLanguage = app::common::fileHandeler::ReadLineFromFile("settings.ini", 1);

    // Check if empty
    if(g_appLanguage == "")
    {app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to find language, defaulting to english"); g_appLanguage = "english";}

    // Apply file path to locales folder
    app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Language set to: " + g_appLanguage);
    g_appLanguage = "locales\\" + g_appLanguage + ".local";

    // Check if it exist
    if(!std::filesystem::exists(g_appLanguage))
    {
        app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] No .local file detected");
        app::common::log::CreateCrashLog("INVALID .LOCAL FILE"); exit(-1);
    }
}