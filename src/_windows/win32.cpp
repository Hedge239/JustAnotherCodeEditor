#include "JACE/_win/win32.h"

#include "JACE/common/global.h"

#include <string>
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

// .... What did  I type again? - Did I mention that I hate windows?
std::wstring app::win32::system::StringToWideString(std::string InputString)
{
    if(InputString.empty())
    {return L"";}

    int wstring_length = MultiByteToWideChar(CP_UTF8, 0, InputString.c_str(), InputString.length(), NULL, 0);
    std::wstring wideStringTo(wstring_length, L' ');
    MultiByteToWideChar(CP_UTF8, 0, InputString.c_str(), InputString.length(), &wideStringTo[0], wstring_length);

    return wideStringTo;
}