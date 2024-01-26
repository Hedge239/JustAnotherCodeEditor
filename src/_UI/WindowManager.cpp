#include "JACE/_UI/WindowManager.h"
#include "JACE/common/logHandeler.h"

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


void app::UI::appUI::InitMainWindow()
{
    app::common::log::LogToFile("application", "[WINDOW_MANAGER] Initilizing Main Window");

    #ifdef _WIN32
        app::common::log::LogToFile("application", "[WINDOW_MANAGER] Display Server: Win32");
        app::win32::UI::w32_createEditorWindow();
    #elif define(__unix__)
        // TODO : LINUX
    #else
        #error "Not supported operating system"
    #endif
}