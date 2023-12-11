#include <JACE/plugins/pluginManager.h>
#include <JACE/plugins/pluginInterface.h>
#include <JACE/common/global.h>
#include <JACE/common/logHandeler.h>


#ifdef _WIN32
    #include "JACE/_win/win32.h"
#elif __linux__
    #include "JACE/_linux/linux.h"
else
    // Continue adding for other display services
#endif

bool IsWindows()
{
    #ifdef _WIN32
        return true;
    #else
        return false;
    #endif
}


void app::plugins::manager::LoadPlugins()
{
    
}