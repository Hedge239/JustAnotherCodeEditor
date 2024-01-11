#include "JACE/plugins/PluginLoader.h"

#include "JACE/common/logHandeler.h"

#include <iostream>


#ifdef _WIN32

    #include <windows.h>

    void* LoadLib(const char* LibPath)
    {
        return LoadLibraryA(LibPath);
    }

    void UnloadLib(void* LibHandel)
    {
        FreeLibrary(static_cast<HMODULE>(LibHandel));
    }

    FARPROC GetFunctionPointer(void* LibHandel, const char* FuncName)
    {
        return GetProcAddress(static_cast<HMODULE>(LibHandel), FuncName);
    }

#elif defined(__unix__)

    #include <dlfcn.h>

    void* LoadLib(const char* LibPath)
    {
        return dlopen(LibPath, RTLD_LAZY);
    }

    void UnloadLib(void* LibHandel)
    {
        dlclose(LibHandel);
    }

    void* GetFunctionPointer(void* LibHandel, const char* FuncName)
    {
        return dlsym(LibHandel, FuncName);
    }

#endif

std::vector<void*> app::plugins::loader::PluginHandles;
std::vector<PluginInterface*> app::plugins::loader::LoadedPlugins;
std::vector<CreatePlugin> app::plugins::loader::FuncCreatePlugin;
std::vector<DestroyPlugin> app::plugins::loader::FuncDestroyPlugin;

void app::plugins::loader::LoadPlugins(const std::vector<std::string>& plugins)
{
    for(const auto& path : plugins)
    {
        void* handle = LoadLib(path.c_str());
        
        if(handle)
        {
            PluginHandles.push_back(handle);
            FuncCreatePlugin.push_back(reinterpret_cast<CreatePlugin>(GetFunctionPointer(handle, "CreatePluginInstance")));
            FuncDestroyPlugin.push_back(reinterpret_cast<DestroyPlugin>(GetFunctionPointer(handle, "DestroyPluginInstance")));
        }else
        {
            app::common::log::LogToFile("application", "[PLUGIN_LOADER] Faild to load: " + path);
        }
    }

    for (auto func : FuncCreatePlugin) {
        LoadedPlugins.push_back(func());
    }
}

void app::plugins::loader::UnloadPlugins()
{
    for (size_t i = 0; i < LoadedPlugins.size(); ++i) {
        FuncDestroyPlugin[i](LoadedPlugins[i]);
    }

    for (auto handle : PluginHandles) {
        UnloadLib(handle);
    }

    PluginHandles.clear();
    FuncCreatePlugin.clear();
    FuncDestroyPlugin.clear();
    LoadedPlugins.clear();
}

const std::vector<PluginInterface*>& app::plugins::loader::GetLoadedPlugins() {
    return LoadedPlugins;
}