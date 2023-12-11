#include "JACE/common/logHandeler.h"
#include "JACE/common/global.h"

#include <filesystem>
#include <fstream>
#include <iostream>

void app::common::log::Clear()
{
    if(std::filesystem::exists(app::common::global::APPDATA + "\\logs"))
    {
        std::filesystem::remove_all(app::common::global::APPDATA + "\\logs");
    }
}

void app::common::log::CreateCrashLog(std::string message)
{
    // Clear previous crash log, hope you did not need that
    if(std::filesystem::exists("crash.log"))
    {
        std::filesystem::remove("crash.log");
    }

    // Write text to crashlog
    std::ofstream crashlog;
    crashlog.open("crash.log");

    crashlog << "!! A Urgent error has occured, as a result the application has forcefully closed its self !!" << std::endl;
    crashlog << "Please check your logs (if any) before sending a error report \n" << std::endl;
    crashlog << "The following message was sent before the crash:" << std::endl;

    crashlog << message << std::endl;

    crashlog.close();
}

void app::common::log::LogToFile(std::string FileName, std::string Message)
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\logs"))
    {
        std::filesystem::create_directory(app::common::global::APPDATA + "\\logs");
    }

    std::ofstream file;
    std::string FilePath = app::common::global::APPDATA + "\\logs\\" + FileName + ".log";

    if(std::filesystem::exists(FilePath))
    {
        file.open(FilePath, std::ios_base::app);
    }else
    {
        file.open(FilePath);
    }

    file << "[APP]: " << Message << std::endl;
    file.close();
}