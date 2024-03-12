#include "JACE/common/localesHandeler.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>


std::unordered_map<std::string, std::string> g_localtextMap;
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

// Check if using Hybrid Mode, if so check if its been loaded
bool loadedInMemory(std::string key, bool isDisabled)
{
    if(!isDisabled)
    {
        if(app::common::global::MEMORYMODE == 1)
        {
            if(g_localtextMap.find(key) != g_localtextMap.end())
                {return true;}
        }
    }

    return false;
}

// Read the text either from .local file or memory || added dontUseMemory since if the main window is allways open, why use extra memory to save strings?
std::string app::common::Localisation::GetText(std::string inputKey, bool dontUseMemory)
{
    if(!loadedInMemory(inputKey, dontUseMemory))
    {
        std::ifstream langFile;
        std::string currentKey;
        std::string line;

        langFile.open(g_appLanguage);
        if(!langFile.is_open())
        {
            app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] Faild to open: " + g_appLanguage);
            app::common::log::CreateCrashLog("FAILD TO OPEN LANGUAGE FILE"); exit(-1);
        }

        while(std::getline(langFile, line))
        {
            size_t colonPos = line.find(":");
            if(colonPos != std::string::npos)
            {
                currentKey = line.substr(0, colonPos);
                if(currentKey == inputKey)
                {
                    if(!dontUseMemory)
                    {
                        if(app::common::global::MEMORYMODE == 1)
                            {g_localtextMap[currentKey] = line.substr(colonPos + 1);}
                    }

                    return line.substr(colonPos + 1);
                    break;
                }
            }
        }

    }else
    {
        // Redundant, and only kept because I KNOW someone will find a way to bypass the loadedinMemory function
        try
        {
            return g_localtextMap.at(inputKey);
        }
        catch(std::out_of_range& e)
        {
            app::common::log::LogToFile("application", "[LOCALISATION_MANAGER] [ERROR] Invalid key for g_localTextMap");
        }
        
    }

    return "INVALID_LANG_KEY";
}