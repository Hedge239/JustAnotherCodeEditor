#include "JACE/plugins/pluginManager.h"
#include "JACE/plugins/pluginLoader.h"

#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

// I would change this to fit the rest that I re-did, but this scares me, and I dont touch what works
#ifdef _WIN32
    const bool IsWindows = true;
#elif defined(__unix__)
    const bool IsWindows = false;
#else
    #error "Not supported operating system"
#endif


void app::plugins::manager::LoadPluginsFromFile()
{
    app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] Initializing");

    // Validate plugin.ini
    app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] Searching for plugin.ini");

    if(!std::filesystem::exists(app::common::global::USRDATA + "\\plugins\\plugin.ini"))
    {
        std::ofstream c_pluginlist;
        std::filesystem::create_directory(app::common::global::USRDATA + "\\plugins");
        c_pluginlist.open(app::common::global::USRDATA + "/plugins/plugin.ini");
        c_pluginlist.close();

        app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] plugin.ini not detected, creating");

        return;
    }

    // Read from plugin.ini
    std::ifstream pluginListFile;
    std::vector<std::string> pluginList;

    pluginListFile.open(app::common::global::USRDATA + "\\plugins\\plugin.ini");
    app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] Located plugin.ini, reading");

    if(pluginListFile.is_open())
    {
        std::string line;
        
        while(std::getline(pluginListFile, line))
        {
            if(line.empty())
            {break;}

            // Add on .dll (windows) or .so (Litterally everything else, except apple.. I think)
            if(IsWindows)
            {
                line = app::common::global::USRDATA + "\\" + line + ".dll";
            }else
            {
                line = app::common::global::USRDATA + "\\" + line + ".so";
            };

            app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] Found: " + line);
            pluginList.push_back(line);
        }
    }else
    {
        app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] Faild to load plugin.ini");
        return;
    }

    if(pluginList.empty())
    {
        app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] No Plugins Detected");
        return;
    }

    // Load Plugins
    app::plugins::loader::LoadPlugins(pluginList);

    // Assign log & alert plugins
    const auto& loadedPlugins = app::plugins::loader::GetLoadedPlugins();
    for (const auto& plugin : loadedPlugins) 
    {
        plugin->SetLogCallBack(app::common::log::LogForPlugins);
        plugin->PluginLoaded();
    }

    app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] Plugins Loaded");
}

void app::plugins::manager::UnloadLoadedPlugins()
{
    app::common::log::LogToFile("application", "[plguins/pluginManager.cpp] Unloading Plugins");
    app::plugins::loader::UnloadPlugins();
}


// Plugin Calls & Callbacks //

void app::plugins::manager::pmPluginPreUnloaded()
{
    const auto& loadedPlugins = app::plugins::loader::GetLoadedPlugins();
    for (const auto& plugin : loadedPlugins) 
    {
        plugin->PluginPreUnloaded();
    }
}