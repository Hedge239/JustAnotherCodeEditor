#include "JACE/_win/win32.h"

#include "JACE/common/global.h"


#include "windows.h"
#include "shlobj.h"


void app::win32::system::GetDataPath()
{
    PWSTR DataPath;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &DataPath);
    std::wstring dataPathString(DataPath);
    
    if(result == S_OK)
    {
        std::wstring dataPathString(DataPath);
        app::common::global::APPDATA = std::string(dataPathString.begin(), dataPathString.end()) + "\\Hedge239\\JustAnotherCodeEditor";
    }else
    {
        return;
    }
}