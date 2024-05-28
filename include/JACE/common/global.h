#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string>

namespace app
{
    namespace common
    {
        namespace global
        {
            extern std::string USRDATA; // Where UserData is stored
            extern std::string WORKINGDIR; // Where the working Project is sotred

            extern int MEMORYMODE; // 1 = Load to memory 0 = Allways load from file
        }
    }
}

#endif /*_GLOBAL_H_*/