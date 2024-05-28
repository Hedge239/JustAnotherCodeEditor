#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"
#include "JACE/common/logHandeler.h"

#include <filesystem>
#include <algorithm>
#include <fstream>
#include <string>


std::string app::common::fileHandeler::ReadLineFromFile(std::string TargetFilePath, int TargetLine)
{
    if(!std::filesystem::exists(app::common::global::USRDATA + "\\" + TargetFilePath))
        {app::common::log::LogToFile("application", "[fileHandeler.cpp] Faild to find file: " + app::common::global::USRDATA + "\\" + TargetFilePath); return "";}

    std::ifstream file;
    std::string line;

    file.open(app::common::global::USRDATA + "\\" + TargetFilePath);
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
        app::common::log::LogToFile("application", "[fileHandeler.cpp] Faild to open file: " + app::common::global::USRDATA + "\\" + TargetFilePath);
    }

    // Return with nothing, when there is nothing
    return "";
}

// ! with proper spacing !
std::string app::common::fileHandeler::GetTextFromFile(std::string TargetFilePath)
{
    if(!std::filesystem::exists((TargetFilePath)))
        {app::common::log::LogToFile("application", "[fileHandeler.cpp] Faild to find file: " + TargetFilePath); return "";}

    std::ifstream file;

    file.open(TargetFilePath);
    if(file.is_open())
    {
        std::string fileContent;
        std::string line;

        while(std::getline(file, line))
        {
            #ifdef _WIN32
                fileContent += line + "\r\n";
            #elif 
                fileContent += line + "\n";
            #endif
        }

        return fileContent;
    }else 
    {
        app::common::log::LogToFile("application", "[fileHandeler.cpp] fild to open file: " + TargetFilePath);
    }

    return "";
}

void app::common::fileHandeler::UpdateFileText(std::string TargetFilePath, std::string newText)
{
    if(!std::filesystem::exists((TargetFilePath)))
        {app::common::log::LogToFile("application", "[fileHandeler.cpp] Faild to find file: " + TargetFilePath); return;}

    std::ofstream file;
    std::filesystem::remove(TargetFilePath);

    file.open(TargetFilePath);
    if(file.is_open())
    {
        std::string FileText = newText;
        #ifdef _WIN32
            // Windows loves causign extra work...
            FileText.erase(std::remove(FileText.begin(), FileText.end(), '\r'), FileText.end());
        #endif

        file << FileText;
        file.close();
    }

    return;
}

void app::common::fileHandeler::TransferFile(std::string OldFilePath, std::string NewFilePath, std::string fileText)
{
    if(!std::filesystem::exists((OldFilePath)))
        {app::common::log::LogToFile("application", "[fileHandeler.cpp] Faild to find file: " + OldFilePath); return;}

    std::ofstream file;

    file.open(NewFilePath);
    if(file.is_open())
    {
        file.close();
        app::common::fileHandeler::UpdateFileText(NewFilePath, fileText);
        std::filesystem::remove(OldFilePath);
    }

    return;
}