#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"

#include "JACE/common/sessionManager.h"
#include "JACE/common/localesHandeler.h"

#include "JACE/_UI/themeManager.h"

#include <windows.h>
#include <string>


// Window Componets, did I mention I still hate winAPI
HWND g_hEditorTextBox;
HMENU g_hEditorMenu;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                // File
                case 9:
                {
                    PostQuitMessage(0);
                    break;
                }
                case 8:
                {
                    // Do reload things, if I keep it
                    break;
                }
            }
        }

        case WM_CREATE:
        {
            // Create MenuBar - TOPBAR //
            g_hEditorMenu = CreateMenu();

            // File
            HMENU hFileMenu = CreateMenu();
            AppendMenuW(g_hEditorMenu, MF_POPUP, (UINT_PTR)hFileMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file")).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 1, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_openfile")).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 2, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_openfolder")).c_str());
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hFileMenu, MF_STRING, 3, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_closefile")).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 4, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_closefolder")).c_str());
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hFileMenu, MF_STRING, 5, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_savecurrent")).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 6, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_saveall")).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 7, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_saveas")).c_str());
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            //AppendMenuW(hFileMenu, MF_STRING, 8, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_reload")).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 9, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_quit")).c_str());
            
            // Edit
            HMENU hEditMenu = CreateMenu();
            AppendMenuW(g_hEditorMenu, MF_POPUP, (UINT_PTR)hEditMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_edit")).c_str());
            
            // View
            HMENU hViewMenu = CreateMenu();
            AppendMenuW(g_hEditorMenu, MF_POPUP, (UINT_PTR)hViewMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_view")).c_str());
            
            // Project - SOURCE CONTROL!!!!
            HMENU hProjectMenu = CreateMenu();
            AppendMenuW(g_hEditorMenu, MF_POPUP, (UINT_PTR)hProjectMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_project")).c_str());
            
            // Extensions - I got to somehow let plugins add to this -_-
            HMENU hPluginMenu = CreateMenu();
            AppendMenuW(g_hEditorMenu, MF_POPUP, (UINT_PTR)hPluginMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_plugin")).c_str());
            
            // Settings
            HMENU hSettingsMenu = CreateMenu();
            AppendMenuW(g_hEditorMenu, MF_POPUP, (UINT_PTR)hSettingsMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_settings")).c_str());
            
            // Help
            HMENU hHelpMenu = CreateMenu();
            AppendMenuW(g_hEditorMenu, MF_POPUP, (UINT_PTR)hHelpMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_help")).c_str());

            // Textbox
            g_hEditorTextBox =  CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL| WS_HSCROLL| WS_BORDER, 0, 0, 0, 0, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            // FileExploerer

            // Terminal
            SetMenu(hwnd, g_hEditorMenu);
            break;
        }

        case WM_SIZE:
        {
            if(g_hEditorTextBox != NULL)
            {
                //InvalidateRect(g_hEditorTextBox, NULL, TRUE);
                //UpdateWindow(g_hEditorTextBox);
                MoveWindow(g_hEditorTextBox, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
                InvalidateRect(g_hEditorTextBox, NULL, FALSE);
            }

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