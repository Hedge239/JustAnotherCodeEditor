#include "JACE/platforms/Windows/windows.h"

#include "JACE/common/global.h"

#include <string>
#include <shlobj.h>


void app::platforms::windows::system::GetUserDataPath()
{
    PWSTR DataPath;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &DataPath);
    std::wstring dataPathString(DataPath);
    
    if(result == S_OK)
    {
        std::wstring dataPathString(DataPath);
        app::common::global::USRDATA = std::string(dataPathString.begin(), dataPathString.end()) + "\\Hedge239\\JustAnotherCodeEditor";
    }else
    {
        return;
    }
}

std::wstring app::platforms::windows::system::StringToWideString(std::string InputString)
{
    if(InputString.empty())
    {return L"";}

    int wstring_length = MultiByteToWideChar(CP_UTF8, 0, InputString.c_str(), InputString.length(), NULL, 0);
    std::wstring wideStringTo(wstring_length, L' ');
    MultiByteToWideChar(CP_UTF8, 0, InputString.c_str(), InputString.length(), &wideStringTo[0], wstring_length);

    return wideStringTo;
}