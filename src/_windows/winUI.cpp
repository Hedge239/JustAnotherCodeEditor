#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"

#include <windows.h>


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

}