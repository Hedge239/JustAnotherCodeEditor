#ifndef _WIN32_H_
#define _WIN32_H_

#include <string>

namespace app
{
    namespace win32
    {
        class system
        {
            public:
            static void GetDataPath();
            static std::wstring StringToWideString(std::string InputString);
        };
    }
}

#endif /*_WIN32_H_*/