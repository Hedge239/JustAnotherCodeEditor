#include "JACE/setup.h"

#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"

#include <filesystem>
#include <fstream>
#include <string>


void app::setup::SetDataPath()
{
    if(!std::filesystem::exists("app.cfg"))
    {
        std::ofstream cfg;

        cfg.open("app.cfg");
        cfg << "path=" + app::common::global::APPDATA << std::endl;

        cfg.close();
    }else
    {
        std::ifstream cfg("app.cfg");
        std::string line;

        if(std::getline(cfg, line))
        {
            if(line.substr(0,5) == "path=")
            {
                app::common::global::APPDATA = std::string(line.substr(5));
            }
        }else
        {
            return;
        }
    }

    if(!std::filesystem::is_directory(app::common::global::APPDATA))
        {std::filesystem::create_directories(app::common::global::APPDATA);}
}

void LoadUserCfgToMem()
{
    app::common::log::LogToFile("application", "[SETUP] Loading required settings to memory");

    // MEMORY MODE //
    try
    {
        app::common::global::MEMORYMODE = std::stoi(app::common::fileHandeler::ReadLineFromFile("settings.ini", 2));
    }catch(std::invalid_argument& ia)
    {
        app::common::log::LogToFile("application", "[SETUP] [ERROR] Invalid argument in 'settings.ini' line 2");
    }catch(std::out_of_range& oor)
    {
        app::common::log::LogToFile("application", "[SETUP] [ERROR] Arugment out of range in 'settings.ini' line 2");
    }

    if(!(app::common::global::MEMORYMODE == 0 || app::common::global::MEMORYMODE == 1))
    {
        app::common::log::LogToFile("application", "[SETUP] [ERROR] Invalid argument in 'settings.ini' line 2, defaulting to 0");
        app::common::global::MEMORYMODE = 0;
    }

    app::common::log::LogToFile("application", "[SETUP] MEMORYMODE set to: " + std::to_string(app::common::global::MEMORYMODE));

}

void app::setup::validateUserFiles()
{
    app::common::log::LogToFile("application", "[SETUP] Vaidating required UserFiles");

    // APPLICATION //
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\keybinds.ini"))
    {
        // KEYBINDS //
        app::common::log::LogToFile("application", "[SETUP] Creating `keybinds.ini`");
        std::ofstream keybinds;

        keybinds.open(app::common::global::APPDATA + "\\keybinds.ini");

        keybinds.close();
    }

    if(!std::filesystem::exists(app::common::global::APPDATA + "\\settings.ini"))
    {
        // SETTINGS //
        app::common::log::LogToFile("application", "[SETUP] Creating `settings.ini`");
        std::ofstream settings;

        settings.open(app::common::global::APPDATA + "\\settings.ini");
        settings << "english" << std::endl;  // System Language
        settings << "0";  // Application Memory Mode: 0 = Direct: Load directly from file each time, 1 = Hybrid: Load text directly from file then save to memory

        settings.close();
    }
    
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\_repos.cfg"))
    {
        // PLUGIN REPOS //
        app::common::log::LogToFile("application", "[SETUP] Creating `_repos.cfg`");
        std::ofstream repos;

        repos.open(app::common::global::APPDATA + "\\_repos.cfg");

        repos.close();
    }

    app::common::log::LogToFile("application", "[SETUP] All files validated");
    LoadUserCfgToMem();
}