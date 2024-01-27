#ifndef _LOCALESHANDELER_H_
#define _LOCALESHANDELER_H_

#include <string>

namespace app
{
    namespace common
    {
        class Localisation
        {
            public:
            static void setAppLanguage();
            static std::string GetText(std::string key);
        };
    }
}

#endif /*_LOCALESHANDELER_H_*/