#include "JACE/_UI/sessionManager.h"

#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"
#include "JACE/common/fileHandeler.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <cmath>


// READ FROM SESSION //
std::string app::UI::sessionManager::lastOpenedProject()
{
    if(app::common::fileHandeler::DoesFileExist("cache\\session.jses"))
    {
        if(!app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 3).empty())
            {return app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 3);}
    }

    return "";
}

app::UI::sessionManager::window GetWindowSize()
{
    if(app::common::fileHandeler::DoesFileExist("cache\\session.jses"))
    {
        if(!app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 1).empty() && !app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 2).empty())
        {
            int winHeight;
            int winWidth;

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
        }
    }

    return {500, 500};
}

// WRITE TO SESSION //