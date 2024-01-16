#include "JACE/_UI/sessionManager.h"

#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"
#include "JACE/common/fileHandeler.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <cmath>


// READ FROM SESSION // Now before I actually test this, I just notice how much unessary code this is, so ima fix that later then test
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

std::string app::UI::sessionManager::lastOpenedProject()
{
    if(!app::UI::sessionManager::hasPreviousSession())
    {
        return "";
    }else
    {
        if(!app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 3).empty())
        {
            return app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 3);
        }else
        {
            return "";
        }
    }
}

app::UI::sessionManager::window GetWindowSize()
{
    if(!app::UI::sessionManager::hasPreviousSession())
    {
        return {500, 500};
    }else
    {
        if(!app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 1).empty() && !app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 2).empty())
        {
            int winHeight;
            int winWidth;

            // Attempt to convert the strings to intigers (I hate error prevention, just dont mess with the file)
            try
            {
                winHeight = std::stoi(app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 1));
                winWidth = std::stoi(app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 2));
            }catch(std::invalid_argument& ia)
            {
                return {500, 500};
            }catch(std::out_of_range& oor)
            {
                return {500, 500};
            }

            return {winHeight, winWidth};
        }else
        {
            return {500, 500};
        }
    }
}

// WRITE TO SESSION //