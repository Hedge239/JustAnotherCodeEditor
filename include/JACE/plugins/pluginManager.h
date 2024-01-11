#ifndef _PLUGINMANAGER_H_
#define _PLUGINMANAGER_H_

#include "PluginInterface.h"
#include <vector>

namespace app
{
    namespace plugins
    {
        class manager
        {
            public:
            static void LoadPluginsFromFile();
            static void UnloadLoadedPlugins();

            // Plugin Functions
            static void pmSetLogCallBack();
            static void pmPluginLoaded();
            static void pmPluginUnloaded();
        };
    }
}

#endif /*_PLUGINMANAGER_H_*/