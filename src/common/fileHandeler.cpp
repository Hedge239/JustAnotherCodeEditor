#include "JACE/common/fileHandeler.h"
#include "JACE/common/global.h"

#include <filesystem>
#include <fstream>


void app::common::fileHandeler::SetDataPath()
{
    if(!std::filesystem::exists("app.cfg"))
    {
        std::ofstream cfg;

        cfg.open("app.cfg");
        cfg << "path=" + app::common::global::APPDATA << std::endl;

        cfg.close();
    }else
    {
        std::ifstream cfg("app.cfg");
        std::string line;

        if(std::getline(cfg, line))
        {
            if(line.substr(0,5) == "path=")
            {
                app::common::global::APPDATA = std::string(line.substr(5));
            }
        }else
        {
            return;
        }
    }

    if(!std::filesystem::is_directory(app::common::global::APPDATA))
    {
        std::filesystem::create_directory(app::common::global::APPDATA);
    }
}