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
        };
    }
}

#endif /*_FILEHANDELER_H_*/