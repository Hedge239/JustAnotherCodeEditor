#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"

#include <windows.h>
#include <string>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
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
    windowStyle &= ~WS_SYSMENU;

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