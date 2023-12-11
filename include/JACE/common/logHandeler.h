#ifndef _LOGHANDELER_H_
#define _LOGHANDELER_H_

#include <string>

namespace app
{
    namespace common
    {
        class log
        {
            public:
            static void LogToFile(std::string FileName, std::string message);
            static void CreateCrashLog(std::string message);
            
            static void Clear();
        };
    }
}

#endif /*_LOGHANDELER_H_*/