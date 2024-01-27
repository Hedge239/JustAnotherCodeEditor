#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"

#include "JACE/common/sessionManager.h"
#include "JACE/common/localesHandeler.h"

#include "JACE/_UI/themeManager.h"

#include <windows.h>
#include <string>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    
}