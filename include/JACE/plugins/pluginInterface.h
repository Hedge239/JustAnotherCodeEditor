#ifndef _PLUGININTERFACE_H_
#define _PLUGININTERFACE_H_

#include <functional>
#include <string>

namespace app
{
    class PluginInterface
    {
        public:
        // Plugin Sets - Functions that allow the plugin to execute functions allready located in the application
            virtual void SetLogger(std::function<void(std::string)> logFunction) = 0;
        // Application State Calls - Notifications of the application state to plugins
            virtual void beforeAppExit() {}
        // Application Plugin Calls - Request the application makes to plugins
            virtual void pluginLoaded() {}
        
            virtual ~PluginInterface() = default;
    };
}

#endif /*_PLUGININTERFACE_H_*/
