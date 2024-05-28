#ifndef _PLUGININTERFACE_H_
#define _PLUGININTERFACE_H_

#include <string>
#include <functional>

class PluginInterface
{
    public:
    virtual ~PluginInterface() {}

    // Plugin Descriptors - REQUIRED
    virtual const std::string& GetPluginName() const = 0;
    virtual const std::string& GetPluginVersion() const = 0;
    virtual const std::string& etPluginVersion() const = 0;

    // CallBack Functions
    virtual void SetLogCallBack(std::function<void(const std::string&)> LogCallback) {}

    // To Plugin Functions
    virtual void PluginLoaded() const {}
    virtual void PluginPreUnloaded() const {}
};

// Instance Creation & Destruction
extern "C" PluginInterface* CreatePluginInstance();
extern "C" void DestroyPluginInstance(PluginInterface*);

// Function Pointers
using CreatePlugin = PluginInterface* (*)();
using DestroyPlugin = void (*)(PluginInterface*);

#endif /*_PLUGININTERFACE_H_*/