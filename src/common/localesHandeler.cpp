#include "JACE/common/localesHandeler.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>


std::unordered_map<std::string, std::string> g_localtextInMem;
std::string g_appLanguage;

void app::common::Localisation::setAppLanguage()
{
    // Find and get language from settings
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\settings.ini"))
    {
        app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to find settings. Relaunch the app, if that fails you did something wrong");
        app::common::log::CreateCrashLog("NO SETTINGS FILE"); exit(-1);
    }

    g_appLanguage = app::common::fileHandeler::ReadLineFromFile("settings.ini", 1);

    // Check if empty
    if(g_appLanguage == "")
    {app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to find language, defaulting to english"); g_appLanguage = "english";}

    // Apply file path to locales folder
    app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Language set to: " + g_appLanguage);
    g_appLanguage = "locales\\" + g_appLanguage + ".local";
}

// Read/write to language file/memory
bool loadedInMemory(std::string key)
{
    if(app::common::global::MEMORYMODE == 1)
    {
        return true;
    }

    return false;
}

void writeToMemory(std::string key, std::string text)
{

}

std::string app::common::Localisation::GetText(std::string inputKey)
{
    if(!loadedInMemory(inputKey))
    {
        std::ifstream localFile(g_appLanguage);
        std::string line;

        if(localFile.is_open())
        {
            while(getline(localFile, line))
            {
            }
        }
    }else
    {

    }
}