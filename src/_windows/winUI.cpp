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
        case WM_CREATE:
        {
            // Create MenuBar - TOPBAR //
            HMENU hMenu = CreateMenu();

            // File
            HMENU hFileMenu = CreateMenu();
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file")).c_str());
            
            // Edit
            HMENU hEditMenu = CreateMenu();
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_edit")).c_str());
            
            // View
            HMENU hViewMenu = CreateMenu();
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_view")).c_str());
            
            // Project
            HMENU hProjectMenu = CreateMenu();
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hProjectMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_project")).c_str());
            
            // Extensions
            HMENU hPluginMenu = CreateMenu();
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hPluginMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_plugin")).c_str());
            
            // Settings
            HMENU hSettingsMenu = CreateMenu();
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSettingsMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_settings")).c_str());
            
            // Help
            HMENU hHelpMenu = CreateMenu();
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_help")).c_str());

            SetMenu(hwnd, hMenu);
            break;
        }

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
    // Define application name - Because windows sucks
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