#ifndef _PLUGININTERFACE_H_
#define _PLUGININTERFACE_H_

#include <functional>
#include <string>

namespace app
{
    class PluginInterface
    {
        public:
        // Required Functions - Functions all plugins are required to linke (May be removed)
            virtual void SetLogger(std::function<void(std::string)> logFunction) = 0;
        // Application State Calls - Notifications of the application state to plugins
            virtual void startOfWindowGeneration() {}
            virtual void beforeAppExit() {}
        // Application Plugin Calls - Request the application makes to plugins
            virtual void createSettings() {}
            virtual void pluginLoaded() {}
        
            virtual ~PluginInterface() = default;
    };
}

#endif /*_PLUGININTERFACE_H_*/
