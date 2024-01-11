#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"

#include "JACE/_UI/WindowManager.h"
#include "JACE/_UI/themeManager.h"

#include "JACE/plugins/PluginManager.h"

#include <filesystem>

// This is to be coppied to ANYTHING that has window specific functions, I can see this getting REAL MESSY in the future witch is a later problem
#ifdef _WIN32
    #include "JACE/_win/win32.h"
    const bool IsWindows = true;
#elif __linux__
    #include "JACE/_linux/linux.h"
    const bool IsWindows = false;
else
    // Continue adding for other display services
#endif


int main(int argc, char* argv[])
{

    // Get where UserData will be stored and validate
    if(!std::filesystem::exists("app.cfg"))
    {
        if(IsWindows)
        {
            #ifdef _WIN32
                app::win32::system::GetDataPath();
            #endif
        }else
        {
            char* DataPath = getenv("HOME");
            
            if(DataPath !=nullptr)
            {
                app::common::global::APPDATA = std::string(DataPath) + "\\.local\\share\\Hedge239\\JustAnotherCodeEditor";
            }
        }
    }

    app::common::fileHandeler::SetDataPath();

    if(!std::filesystem::is_directory(app::common::global::APPDATA))
        {app::common::log::CreateCrashLog("'path=' in app.cfg does not lead to a valid directory"); exit(-1);}
    if(app::common::global::APPDATA == "")
        {app::common::log::CreateCrashLog("'path=' in app.cfg can not be empty"); exit(-1);}
    
    app::common::log::startSession();
    app::common::log::LogToFile("application", "[MAIN] UserData set to: " + app::common::global::APPDATA);

    // Load plugins - Testing comes at a later time, hope it works
    app::plugins::manager::LoadPluginsFromFile();

    // Init themeManager
    app::UI::themeManager::InitThemeManager();

    // Load UserData into memory (ie settings, keybinds, etc)
    //// TODO, I will only set this up when I actually need settings
    
    // Load UI - pain
    app::UI::appUI::CreateMainWindow();

    // Application cleanup/closeing
    app::common::log::LogToFile("application", "[MAIN] Application shuting down");
}