#include "JACE/GUI/windowManager.h"

#include "JACE/common/logHandeler.h"

#ifdef SUPPORT_WINAPI
    #include "JACE/platforms/Windows/GUI/Win32/EditorWindow.h"
#endif

#ifdef SUPPORT_X11
    #include "JACE/platforms/Linux/GUI/x11/EditorWindow.h"
#endif

#ifdef SUPPORT_WAYLAND
    #include "JACE/platforms/Linux/GUI/Wayland/EditorWindow.h"
#endif


std::string xdgType()
{
    return std::getenv("XDG_SESSION_TYPE");
}

void app::GUI::windowManager::CreateEditorWindow()
{
    app::common::log::LogToFile("application", "[GUI/windowManager.cpp] Initilizing Main Window");

    #ifdef _WIN32
        app::common::log::LogToFile("application", "[GUI/windowManager.cpp] Display Service: Win32");
        app::platforms::windows::Win32::CreateEditorWindow();
    #elif __linux__
        // TODO
    #else
        #error "Not supported operating system"
    #endif
}