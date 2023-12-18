#ifndef _PLUGININTERFACE_H_
#define _PLUGININTERFACE_H_

#include <functional>
#include <string>

namespace app
{
    class PluginInterface
    {
        public:
        // Required Functions
            virtual void SetLogger(std::function<void(std::string)> logFunction) = 0;
        // Optional Functions
            virtual void createSettings() {}
        
            virtual ~PluginInterface() = default;
    };
}

#endif /*_PLUGININTERFACE_H_*/
