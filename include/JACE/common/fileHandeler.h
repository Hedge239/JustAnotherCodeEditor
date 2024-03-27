#ifndef _FILEHANDELER_H_
#define _FILEHANDELER_H_

#include <string>

namespace app
{
    namespace common
    {
        class fileHandeler
        {
            public:
            static std::string ReadLineFromFile(std::string TargetFilePath, int targetLine);
            static std::string GetTextFromFile(std::string TargetFilePath);
            static void UpdateTextToFile(std::string TargetFilePath, std::string newText);
        };
    }
}

#endif /*_FILEHANDELER_H_*/