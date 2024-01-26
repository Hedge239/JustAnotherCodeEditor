#include "JACE/setup.h"

#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"
#include "JACE/common/localesHandeler.h"

#include "JACE/_UI/WindowManager.h"
#include "JACE/_UI/themeManager.h"

#include "JACE/plugins/PluginManager.h"

#include <filesystem>
#include <fstream>

// This is to be coppied to ANYTHING that has window specific functions, I can see this getting REAL MESSY in the future witch is a later problem
#ifdef _WIN32
    #include "JACE/_win/win32.h"
    const bool IsWindows = true;
#elif defined(__unix__)
    #include "JACE/_linux/linux.h"
    const bool IsWindows = false;
#else
    #error "Not supported operating system"
#endif


int main(int argc, char* argv[])
{

    // Get userData dir
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

    // Set userData dir
    app::setup::SetDataPath();

    if(!std::filesystem::is_directory(app::common::global::APPDATA))
        {app::common::log::CreateCrashLog("'path=' in app.cfg does not lead to a valid directory"); exit(-1);}
    if(app::common::global::APPDATA == "")
        {app::common::log::CreateCrashLog("'path=' in app.cfg can not be empty"); exit(-1);}
    
    // Create Cache
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\cache"))
    {std::filesystem::create_directory(app::common::global::APPDATA + "\\cache");}

    // Init Logger
    app::common::log::startSession();
    app::common::log::LogToFile("application", "[MAIN] UserData set to: " + app::common::global::APPDATA);

    // Load userData
    app::setup::validateUserFiles();
    app::common::Localisation::setAppLanguage();
    app::plugins::manager::LoadPluginsFromFile();
    app::UI::themeManager::InitThemeManager();

    // Create UI
    app::UI::appUI::InitMainWindow();

    // Cleanup
    app::common::log::LogToFile("application", "[MAIN] Main Window Closed, cleaning up");

    app::plugins::manager::pmPluginPreUnloaded();
    app::plugins::manager::UnloadLoadedPlugins();
}