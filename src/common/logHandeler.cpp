#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"

#include <filesystem>
#include <fstream>
#include <iostream>


void app::common::log::LogToFile(std::string FileName, std::string Message)
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\logs\\sessionLatest"))
    {
        std::filesystem::create_directories(app::common::global::APPDATA + "\\logs\\sessionLatest");
    }

    std::ofstream file;
    std::string FilePath = app::common::global::APPDATA + "\\logs\\sessionLatest\\" + FileName + ".log";

    if(std::filesystem::exists(FilePath))
    {
        file.open(FilePath, std::ios_base::app);
    }else
    {
        file.open(FilePath);
    }

    file << Message << std::endl;
    file.close();
}

void app::common::log::LogForPlugins(std::string Message)
{
    std::ofstream file;
    std::string FilePath = app::common::global::APPDATA + "\\logs\\sessionLatest\\plugins.log";

    if(std::filesystem::exists(FilePath))
    {
        file.open(FilePath, std::ios_base::app);
    }else
    {
        file.open(FilePath);
    }

    file << Message << std::endl;
    file.close();
}

void app::common::log::CreateCrashLog(std::string message)
{
    if(std::filesystem::exists("crash.log"))
    {
        std::filesystem::remove("crash.log");
    }

    std::ofstream crashlog;
    crashlog.open("crash.log");

    crashlog << "!! A Urgent error has occured, as a result the application has forcefully closed its self !!" << std::endl;
    crashlog << "Please check your logs (if any) before sending a error report \n" << std::endl;
    crashlog << "The following message was sent before the crash:" << std::endl;

    crashlog << message << std::endl;

    crashlog.close();
}

void app::common::log::startSession()
{   
    // Log are now stored on a session based system, there for we now have the latest logs and logs from the last time the app was run
    if(std::filesystem::exists(app::common::global::APPDATA + "\\logs\\sessionLatest"))
    {
        if(std::filesystem::exists(app::common::global::APPDATA + "\\logs\\sessionOld"))
        {
            std::filesystem::remove_all(app::common::global::APPDATA + "\\logs\\sessionOld");
            std::filesystem::rename(app::common::global::APPDATA + "\\logs\\sessionLatest", app::common::global::APPDATA + "\\logs\\sessionOld");
        }else
        {
            std::filesystem::rename(app::common::global::APPDATA + "\\logs\\sessionLatest", app::common::global::APPDATA + "\\logs\\sessionOld");
        }
    }
    
    app::common::log::LogToFile("application", "[LOGHANDELER] Session Started at: " + app::common::global::APPDATA + "\\logs\\sessionLatest");
}