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

            // Plugin Functions - pm = plugin message //
            static void pmPluginPreUnloaded();
        };
    }
}

#endif /*_PLUGINMANAGER_H_*/