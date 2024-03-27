#include "JACE/common/sessionManager.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"
#include "JACE/common/fileHandeler.h"

#include <filesystem>
#include <string>


// READ FROM SESSION //
std::string app::common::sessionManager::lastOpenedProject()
{
    if(std::filesystem::exists(app::common::global::APPDATA + "\\cache\\session.jses"))
    {
        if(!app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 3).empty())
            {return app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 3);}
    }

    return "";
}

int app::common::sessionManager::WindowHeight()
{
    if(std::filesystem::exists(app::common::global::APPDATA + "\\cache\\session.jses"))
    {
        if(!app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 1).empty())
        {
            try
            {
                return std::stoi(app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 1));
            }catch(std::invalid_argument& ia)
            {
                app::common::log::LogToFile("application", "[SESSION_MANAGER] Invalid Argument for 'WindowHeight'");
            }catch(std::out_of_range& oor)
            {
                app::common::log::LogToFile("application", "[SESSION_MANAGER] Argument out of range for 'WindowHeight'");
            }
        }
    }

    return 500;
}

int app::common::sessionManager::WindowWidth()
{
    if(std::filesystem::exists(app::common::global::APPDATA + "\\cache\\session.jses"))
    {
        if(!app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 2).empty())
        {
            try
            {
                return std::stoi(app::common::fileHandeler::ReadLineFromFile("cache\\session.jses", 2));
            }catch(std::invalid_argument& ia)
            {
                app::common::log::LogToFile("application", "[SESSION_MANAGER] Invalid Argument for 'WindowHeight'");
            }catch(std::out_of_range& oor)
            {
                app::common::log::LogToFile("application", "[SESSION_MANAGER] Argument out of range for 'WindowHeight'");
            }
        }
    }

    return 500;
}

// WRITE TO SESSION //