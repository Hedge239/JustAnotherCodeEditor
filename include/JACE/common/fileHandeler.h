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
            static std::string ReadLineFromFile(std::string TargetFile, int targetLine);
            static bool DoesFileExist(std::string File);

            static void SetDataPath();
        };
    }
}

#endif /*_FILEHANDELER_H_*/