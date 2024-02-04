#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"

#include "JACE/common/sessionManager.h"
#include "JACE/common/localesHandeler.h"

#include "JACE/_UI/themeManager.h"

#include <windows.h>
#include <string>


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void app::win32::UI::w32_createEditorWindow()
{
    // Define application name - TODO find a better way to do this
    std::wstring ApplicationName =  app::win32::system::StringToWideString(app::common::Localisation::GetText("app_name"));

    // Define the Window Class
    WNDCLASSW WindowClass = {0};
    WindowClass.lpfnWndProc = WindowProcedure;
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.lpszClassName = ApplicationName.c_str();
    RegisterClassW(&WindowClass);

    // Define the window Style
    DWORD WindowStyle;
    WindowStyle = WS_OVERLAPPEDWINDOW;

    // Create the window
    HWND hwnd = CreateWindowW(
        ApplicationName.c_str(),
        ApplicationName.c_str(),
        WindowStyle,
        0,
        0,
        app::common::sessionManager::WindowWidth(),
        app::common::sessionManager::WindowHeight(),
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );
    
    // Display the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Assign classback & start loop
    MSG wMsg;
    while(GetMessage(&wMsg, NULL, 0, 0))
    {
        TranslateMessage(&wMsg);
        DispatchMessage(&wMsg);
    }
}