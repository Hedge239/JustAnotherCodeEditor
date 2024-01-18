#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include "JACE/common/logHandeler.h"


#include <filesystem>
#include <fstream>
#include <string>

// Most functions in this file are made to use files specified in "app.cfg"
std::string app::common::fileHandeler::ReadLineFromFile(std::string TargetFile, int TargetLine)
{
    if(!std::filesystem::is_directory(app::common::global::APPDATA + "\\" + TargetFile))
        {app::common::log::LogToFile("application", "[fileHandeler] Faild to find file: " + app::common::global::APPDATA + "\\" + TargetFile);}

    std::ifstream file;
    std::string line;

    file.open(app::common::global::APPDATA + "\\" + TargetFile);
    if(file.is_open())
    {
        int CurrentLine = 0;

        // Go through every line on the target file until we reach the desired line
        while(getline(file, line))
        {
            CurrentLine++;

            if(CurrentLine == TargetLine)
            {
                file.close();
                return line;
            }
        }
    }else
    {
        app::common::log::LogToFile("application", "[fileHandeler] Faild to open file: " + app::common::global::APPDATA + "\\" + TargetFile);
    }

    // Return with nothing, when there is nothing
    return "";
}

bool app::common::fileHandeler::DoesFileExist(std::string InputFile)
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\" + InputFile))
        {return false;}

    return true;
}

// Functions
void app::common::fileHandeler::SetDataPath()
{
    if(!std::filesystem::exists("app.cfg"))
    {
        std::ofstream cfg;

        cfg.open("app.cfg");
        cfg << "path=" + app::common::global::APPDATA << std::endl;

        cfg.close();
    }else
    {
        std::ifstream cfg("app.cfg");
        std::string line;

        if(std::getline(cfg, line))
        {
            if(line.substr(0,5) == "path=")
            {
                app::common::global::APPDATA = std::string(line.substr(5));
            }
        }else
        {
            return;
        }
    }

    if(!std::filesystem::is_directory(app::common::global::APPDATA))
        {std::filesystem::create_directory(app::common::global::APPDATA);}
}