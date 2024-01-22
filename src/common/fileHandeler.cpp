#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include "JACE/common/logHandeler.h"

#include <filesystem>
#include <fstream>
#include <string>


std::string app::common::fileHandeler::ReadLineFromFile(std::string TargetFile, int TargetLine)
{
    if(!std::filesystem::exists(app::common::global::APPDATA + "\\" + TargetFile))
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