#include "JACE/common/localesHandeler.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include <string>
#include <filesystem>
#include <fstream>

#define MAX_NUMBER_OF_LINES 1

std::string g_appLanguage;
std::string* g_languageInMem = nullptr;
bool UseMemoryStorage;

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

    // Load language into memory : Controled by user : May be removed in future release if not needed
    if(app::common::fileHandeler::ReadLineFromFile("settings.ini", 2) == "true")
    {
        app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Loading into memory: " + g_appLanguage);
        
        g_languageInMem = new std::string[MAX_NUMBER_OF_LINES];   // Up as more language storage is needed
        UseMemoryStorage = true;

        std::ifstream localFile("locales\\" + g_appLanguage);

        if(localFile.is_open())
        {
            for(int i = 0; i < MAX_NUMBER_OF_LINES && std::getline(localFile, g_languageInMem[i]); ++i){
                app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Loaded" + g_languageInMem[i]);
            }
            localFile.close();
        }else
        {
            app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to open: locales\\" + g_appLanguage);
            app::common::log::CreateCrashLog("INVALID .LOCAL FILE"); exit(-1);
        }

        app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Finished loading file into memory");
    }
}   