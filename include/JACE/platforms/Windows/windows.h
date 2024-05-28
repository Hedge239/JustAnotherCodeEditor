#ifndef _JACE_WINDOWS_H_
#define _JACE_WINDOWS_H_

#include <string>

namespace app
{
    namespace platforms
    {
        namespace windows
        {
            class system
            {
                public:
                static void GetUserDataPath();
                static std::wstring StringToWideString(std::string InputString);
            };
        }
    }
}

#endif /*_JACE_WINDOWS_H_*/