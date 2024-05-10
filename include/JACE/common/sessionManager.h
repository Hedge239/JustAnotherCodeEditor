#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_


#include <string>


namespace app
{
    namespace common
    {
        class sessionManager
        {
            public:
            static std::string lastOpenedProject();
            static int WindowHeight();
            static int WindowWidth();

            static void setWindowSize(int Height, int Width);
        };
    }
}

#endif /*_SESSIONMANAGER_H_*/