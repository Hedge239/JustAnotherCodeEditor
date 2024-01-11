#include "JACE/_UI/WindowManager.h"

#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"

#include <filesystem>
#include <fstream>

#ifdef _WIN32
    #include "JACE/_win/winUI.h"
#elif defined(__unix__)
    #ifdef UNIX_DYNAMIC_DS
        #include "JACE/_linux/linux-x11.h"
    #endif
        #include "JACE/_linux/linux-wl.h"
#else
    #error "Not supported operating system"
#endif


void app::UI::appUI::CreateEditorWindow()
{
    app::common::log::LogToFile("application", "[WINDOW_MANAGER] Generating Editor Window");

    #ifdef _WIN32
        app::win32::UI::w32CreateWindow();
    #elif defined(__unix__)
        // Linux Stuff
    #else
        app::common::log::LogToFile("application", "[WindowManager] Faild to detect valid operating system");
        app::common::log::CreateCrashLog("Unknown Operating System : Faild to generate main window"); exit(-1);
    #endif
}