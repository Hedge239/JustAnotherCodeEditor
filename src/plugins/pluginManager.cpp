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
        c_pluginlist.open(app::common::global::APPDATA + "\\plugins\\plugin.ini");
        c_pluginlist.close();

        app::common::log::LogToFile("application", "[PLUGIN_MANAGER] plugin.ini not detected, creating");

        return;
    }

    std::ifstream pluginlist;
    std::vector<std::string> plugins;

    pluginlist.open(app::common::global::APPDATA + "\\plugins\\plugin.ini");
    app::common::log::LogToFile("application", "[PLUGIN_MANAGER] Reading from plugin.ini");

    if(pluginlist.is_open())
    {
        std::string line;

        if(std::getline(pluginlist, line))
        {
            app::common::log::LogToFile("application", "[PLUGIN_MANAGER] Text detected in plugin.ini, reading");

            while(std::getline(pluginlist, line) && !line.empty())
            {
                plugins.push_back(line);
            }
        }else
        {
            app::common::log::LogToFile("application", "[PLUGIN_MANAGER] No text detected, aborting");
            return;
        }
    }else
    {
        app::common::log::LogToFile("application", "[PLUGIN_MANAGER] [ERROR] Faild to load plugin.ini, aborting");
        return;
    }

    // Open plugins/pluginName/pluginName.so/lib
    // Load plugin in and add to vector
    // do the func that run at the beginning
}