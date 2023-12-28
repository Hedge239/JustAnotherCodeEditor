#include "JACE/_UI/WindowManager.h"
#include "JACE/plugins/pluginInterface.h"

#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"

#ifdef _WIN32
    #include "JACE/_win/winUI.h"
#elif __linux__
    #ifdef LINUX_DYNAMIC_DS
        #include "JACE/_linux/linux-wl.h"
    #endif
        #include "JACE/_linux/linux-x11.h"
#else
    // Continue adding for more display servers
#endif


void app::UI::appUI::CreateMainWindow()
{
    app::common::log::LogToFile("application", "[WindowCreation] Generating Editor Window");
    app::common::log::LogToFile("application", "[PLUGIN_MANAGER] Executing startOfWindowGeneration on all plugins");
    for(app::PluginInterface* plugin : app::common::global::plugins){
        plugin->startOfWindowGeneration();
    }

    #ifdef _WIN32
        app::win32::UI::w32CreateWindow();
    #elif __linux__
        // Linux Stuff
    #else
        app::common::log::CreateCrashLog("Unknown Operating System : Faild to generate main window, aborting"); exit(-1);
    #endif
}