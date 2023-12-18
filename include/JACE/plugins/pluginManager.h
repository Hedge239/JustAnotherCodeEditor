#ifndef _PLUGINMANAGER_H_
#define _PLUGINMANAGER_H_

namespace app
{
    namespace plugins
    {
        class manager
        {
            public:
            static void LoadPlugins();
            static void unloadPlugins();
        };
    }
}

#endif /*_PLUGINMANGER_H_*/