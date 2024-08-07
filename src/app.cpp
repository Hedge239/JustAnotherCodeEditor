#include "JACE/setup.h"

#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/localesHandeler.h"

#include "JACE/plugins/pluginManager.h"

#include "JACE/GUI/themeManager.h"
#include "JACE/GUI/windowManager.h"

#include <filesystem>

#ifdef _WIN32
    #include "JACE/platforms/Windows/windows.h"
    #define IsSystemWindows()
#elif __linux__
    #include "JACE/platforms/Linux/linux.h"
#else
    #error "No Supported Operating System"
#endif


int main(int argc, char* argv[])
{
    // Get userData Directory
    if(!std::filesystem::exists("app.cfg"))
    {
        #ifdef IsSystemWindows
            app::platforms::windows::system::GetUserDataPath();
        #elif 
            char* DataPath = getenv("HOME");
            
            if(DataPath !=nullptr)
            {
                app::common::global::USRDATA = std::string(DataPath) + "\\.local\\share\\Hedge239\\JustAnotherCodeEditor";
            }
        #endif
    }

    // set USRDATA for entire application
    app::setup::SetDataPath();

    if(!std::filesystem::is_directory(app::common::global::USRDATA))
        {app::common::log::CreateCrashLog("Variable 'path=' in 'app.cfg' does not lead to a valid directory"); exit(-1);}
    if(app::common::global::USRDATA == "")
        {app::common::log::CreateCrashLog("Variable 'path=' in 'app.cfg' can not be empty"); exit(-1);}

    // Create Temp & Cache Folders
    if(!std::filesystem::exists(app::common::global::USRDATA + "\\cache"))
        {std::filesystem::create_directory(app::common::global::USRDATA + "\\cache");}
    if(!std::filesystem::exists(app::common::global::USRDATA + "\\temp"))
        {std::filesystem::create_directory(app::common::global::USRDATA + "\\temp");}

    // Init Logger
    app::common::log::StartSession();
    app::common::log::LogToFile("application", "[app.cpp] Variable 'USRDATA' changed to: " + app::common::global::USRDATA);

    // Load Settings
    app::setup::ValidateUserFiles();
    app::common::Localisation::SetAppLanguage();
    app::plugins::manager::LoadPluginsFromFile();
    app::GUI::themeManager::InitThemeManager();

    // Create Editor Window
    app::GUI::windowManager::CreateEditorWindow();

    // Cleanup
    app::common::log::LogToFile("application", "[app.cpp] Performing Cleanup Tasks");

    app::plugins::manager::pmPluginPreUnloaded();
    app::plugins::manager::UnloadLoadedPlugins();
}