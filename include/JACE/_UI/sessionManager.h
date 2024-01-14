#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_

namespace app
{
    namespace UI
    {
        class sessionManager
        {
            public:
            bool hasPreviousSession();
            std::string lastOpenedProject();
            struct window {int height; int width;};

            static void WriteWindowSize();
            static void WriteProjectDirectory();
        };
    }
}

#endif /*_SESSIONMANAGER_H_*/