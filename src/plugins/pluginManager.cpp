#include <JACE/plugins/pluginManager.h>
#include <JACE/plugins/pluginInterface.h>
#include <JACE/common/global.h>
#include <JACE/common/logHandeler.h>

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#ifdef _WIN32
    #include "JACE/_win/win32.h"
    const bool IsWindows = true;
#elif __linux__
    #include "JACE/_linux/linux.h"
else
    // Continue adding for other display services
#endif


void app::plugins::manager::LoadPlugins()
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\plugins\\plugin.ini"))
    {
        std::ofstream c_pluginlist;
        std::filesystem::create_directory(app::common::global::APPDATA + "\\plugins");
        c_pluginlist.open(app::common::global::APPDATA + "\\plugins\\plugin.ini");
        c_pluginlist.close();

        app::common::log::LogToFile("application", "[PLUGIN_MANAGER] plugin.ini not detected, creating");

        return;
    }

    // Read the list of plugins from plugin.ini
    std::ifstream pluginListFile;
    std::vector<std::string> pluginList;

    pluginListFile.open(app::common::global::APPDATA + "\\plugins\\plugin.ini");

    if(pluginListFile.is_open())
    {
        std::string line;
        
        while(std::getline(pluginListFile, line))
        {
            if(line.empty())
            {
                app::common::log::LogToFile("application", "[PLUGIN_MANAGER] Loading all detected plugins");
                break;
            }

            // This is a horrible way for this to function, is yet another problem for future me
            if(IsWindows)
            {
                line = line + ".dll";
            }else
            {
                line = line + ".so";
            };

            app::common::log::LogToFile("application", "[PLUGIN_MANAGER] Found: " + line);
            pluginList.push_back(line);
        }
    }else
    {
        app::common::log::LogToFile("application", "[PLUGIN_MANAGER] [ERROR] Faild to load plugin.ini, aborting");
        return;
    }

    // Load plugin/plugin.so/lib
    for(std::string plugin : pluginList)
    {
        
    }
    // Load plugin in and add to vector
    // do the func that run at the beginning
}