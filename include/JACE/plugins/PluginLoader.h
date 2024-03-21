#ifndef _PLUGINLOADER_H_
#define _PLUGINLOADER_H_

#include <vector>
#include <string>
#include "pluginInterface.h"

namespace app
{
    namespace plugins
    {
        class loader
        {
            public:
            static void LoadPlugins(const std::vector<std::string>& plugins);
            static void UnloadPlugins();

            static const std::vector<PluginInterface*>& GetLoadedPlugins();

            private:
            static std::vector<PluginInterface*> LoadedPlugins;
            static std::vector<void*> PluginHandles;
            static std::vector<CreatePlugin> FuncCreatePlugin;
            static std::vector<DestroyPlugin> FuncDestroyPlugin;
        };
    }
}

#endif /*_PLUGINLOADER_H_*/