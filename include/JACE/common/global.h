#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "JACE/plugins/pluginInterface.h"

#include <string>
#include <vector>

namespace app
{
    namespace common
    {
        namespace global
        {
            extern std::string APPDATA;
            extern std::vector<app::PluginInterface*> plugins;
        }
    }
}

#endif /*_GLOBAL_H_*/