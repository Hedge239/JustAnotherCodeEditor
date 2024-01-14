#include "JACE/_UI/sessionManager.h"

#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"
#include "JACE/common/fileHandeler.h"

#include <filesystem>
#include <fstream>
#include <string>


// READ FROM SESSION //
bool app::UI::sessionManager::hasPreviousSession()
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\cache\\session.jses"))
    {
        return false;
    }else
    {
        return true;
    }
}

app::UI::sessionManager::window GetWindowSize()
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\cache\\session.jses"))
    {
        return {500, 500};
    }else
    {
        // TODO : Read the window.session file and get the sizes from previous  session
    }
}

std::string app::UI::sessionManager::lastOpenedProject()
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\cache\\session.jses"))
    {
        return;
    }else
    {
        // TODO : Read the window.session file and get files path of last opened project
    }
}


// WRITE TO SESSION //