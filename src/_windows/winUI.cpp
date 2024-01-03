#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"

#include <windows.h>
#include <string>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
        {
            // TOPBAR //
            HMENU hMenu = CreateMenu();

            // File
            HMENU hFileMenu = CreateMenu();
                AppendMenuW(hFileMenu, MF_STRING, 1, L"Open File");
                AppendMenuW(hFileMenu, MF_STRING, 2, L"Open Folder");
                AppendMenuW(hFileMenu, MF_STRING, 3, L"Open Project");
                AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr);
                AppendMenuW(hFileMenu, MF_STRING, 4, L"Save File");
                AppendMenuW(hFileMenu, MF_STRING, 5, L"Save All");
                AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr);
                AppendMenuW(hFileMenu, MF_STRING, 6, L"Exit");
                AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
            // Edit
            HMENU hEditMenu = CreateMenu();
                AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"Edit");
                AppendMenuW(hEditMenu, MF_STRING, 1, L"Undo");
                AppendMenuW(hEditMenu, MF_STRING, 2, L"Redo");
                AppendMenuW(hEditMenu, MF_SEPARATOR, 0, nullptr);
                AppendMenuW(hEditMenu, MF_STRING, 3, L"Find Text");
                AppendMenuW(hEditMenu, MF_STRING, 4, L"Replace Text");
            // View
            HMENU hViewMenu = CreateMenu();
                AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"View");
            // Project
            HMENU hProjectMenu = CreateMenu();
                AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hProjectMenu, L"Project");
            // Extensions
            HMENU hPluginMenu = CreateMenu();
                AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hPluginMenu, L"Plugin");
            // Settings
            HMENU hSettingsMenu = CreateMenu();
                AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSettingsMenu, L"Settings");
            // Help
            HMENU hHelpMenu = CreateMenu();
                AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"Help");


            // SIDEBAR //

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

void app::win32::UI::w32CreateWindow()
{
    // Define the window class
    WNDCLASSW windowClass = {0};
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.lpszClassName = L"JustAnotherCodeEditor";
    RegisterClassW(&windowClass);

    // Define the style of the window
    DWORD windowStyle;
    windowStyle = WS_OVERLAPPEDWINDOW;

    // Create the window
    HWND hwnd = CreateWindowW(
        L"JustAnotherCodeEditor",       // ClassName
        L"JustAnotherCodeEditor",       // DisplayName
        windowStyle,
        0,
        0,
        500,                            // Width
        500,                            // Height
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    // Display the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Assign the window callback and apply loops to prevent closing
    MSG wMsg;
    while(GetMessage(&wMsg, NULL, 0, 0))
    {
        TranslateMessage(&wMsg);
        DispatchMessage(&wMsg);
    }
}