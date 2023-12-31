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
    #include "windows.h"
    const bool IsWindows = true;
#elif __linux__
    #include "JACE/_linux/linux.h"
    #include "dlfcn.h"
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
    app::common::log::LogToFile("application", "[PLUGIN_MANAGER] Initializing");

    if(pluginListFile.is_open())
    {
        std::string line;
        
        while(std::getline(pluginListFile, line))
        {
            if(line.empty())
            {
                break;
            }

            // This is yet another horrible function, makeing this another problem for future me
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
    for(std::string pluginPath : pluginList)
    {
        // If I need more then UNIX and Win32 dynamic libs, then that is a later problem
        #ifdef _WIN32
            HMODULE pluginHandle = LoadLibrary(std::string(app::common::global::APPDATA + "\\plugins\\" + pluginPath).c_str());
        #elif
            void pluginHandle = dlopen(std::string(app::common::global::APPDATA + "\\plugins\\" + pluginPath).c_str(), RTLD_LAZY);
        #endif

        if(!pluginHandle)
        {
            app::common::log::LogToFile("application", "[PLUGIN_MANAGER] [ERROR] Invalid plugin: " + pluginPath);
        }else
        {
            using CreatePluginFn = app::PluginInterface* (*)();

            #ifdef _WIN32
                CreatePluginFn createPlugin = reinterpret_cast<CreatePluginFn>(GetProcAddress(pluginHandle, "createPlugin"));
            #elif
                CreatePluginFn createPlugin = reinterpret_cast<CreatePluginFn>(dlsym(pluginHandle, "createPlugin"));
            #endif

            // If the plugin falls, it all falls, for safety
            if(!createPlugin)
            {
                app::common::log::LogToFile("application", "[PLUGIN_MANAGER] [ERROR] Faild to assign createPlugin to: " + pluginPath);
                
                // pain
                #ifdef _WIN32
                    FreeLibrary(pluginHandle);
                #elif
                    dlclose(pluginHandle);
                #endif
            }else
            {
                app::PluginInterface* plugin = createPlugin();
                app::common::log::LogToFile("application", "[PLUGIN_MANAGER] Assigned calls to: " + pluginPath);
            
                // Run all required functions for plugins (eg. logger, etc)
                plugin->SetLogger(app::common::log::LogForPlugins);

                // Keep track of all active plugins in a global vector
                app::common::global::plugins.push_back(plugin);

                // Tell the plugin that it has been loaded
                plugin->pluginLoaded();
            }
        }
    }
}

void app::plugins::manager::unloadPlugins()
{
    // Eventually add FreeLibrary and dlclose, but for now let the OS handel that since this only happens right before the program fully closes - I know this is bad practice, shut-up mom
    for (app::PluginInterface* plugin : app::common::global::plugins)
    {
        delete plugin;
    }
}