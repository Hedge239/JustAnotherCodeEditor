#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"
#include "JACE/_win/resources.h"

#include "JACE/common/sessionManager.h"
#include "JACE/common/localesHandeler.h"

#include "JACE/_UI/themeManager.h"

#include <windows.h>
#include <commctrl.h>
#include <string>


// GLOBAL VARIABLES // Primary Elements 1 - 9 (Panels, etc) Child Elements 10 - 100
#define CURSOR_REACH 10
#define PANEL_RESIZE_THRESHOLD 10 // In Miliseconds
#define TABS_PANEL_SIZE 30

bool g_isMovingLeftPanel = false;
bool g_isMovingLowerPanel = false;

int g_leftPanelWidth = 200;
int g_lowerPanelHeight = 100;

POINT g_previousPanelLocation = {0};


void createNewTab(HWND hTabs, std::string tabName)
{
    TCITEM tie;
    tie.mask = TCIF_TEXT;
    tie.pszText = (LPSTR)tabName.c_str();

    TabCtrl_InsertItem(hTabs, 0, &tie);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        // WINDOW OPERATIONS & COMMANDS //
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
                    // Works!?! MAYBE!?!?
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }
            }
        }

        // WINDOW INPUT - MOUSE //
        case WM_SETCURSOR:
        {
            if(LOWORD(lParam) == HTCLIENT)
            {
                // Change cursor when over edges of panels
                HWND hLeftPanel = GetDlgItem(hwnd, 1);
                HWND hLowerPanel = GetDlgItem(hwnd, 2);

                // Get CusorPosistion inside the Window
                POINT cursorPos;
                GetCursorPos(&cursorPos);
                ScreenToClient(hwnd, &cursorPos);

                RECT lowerPanelRect, leftPanelRect, hwndClientRect;
                GetWindowRect(hLeftPanel, &leftPanelRect);
                GetWindowRect(hLowerPanel, &lowerPanelRect);
                GetClientRect(hwnd, &hwndClientRect);

                MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&leftPanelRect, 2);
                MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&lowerPanelRect, 2);

                if(cursorPos.x >= leftPanelRect.right - CURSOR_REACH && cursorPos.x <= leftPanelRect.right + CURSOR_REACH)
                {
                    SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                    break;
                }else if(cursorPos.y >= lowerPanelRect.top - CURSOR_REACH && cursorPos.y <= lowerPanelRect.top + CURSOR_REACH)
                {
                    SetCursor(LoadCursor(NULL, IDC_SIZENS));
                    break;
                }else
                {
                    if(cursorPos.x <= 0 || cursorPos.x >= hwndClientRect.right - 1)
                    {
                        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                        break;
                    }else if(cursorPos.y >= hwndClientRect.bottom - 1)
                    {
                        SetCursor(LoadCursor(NULL, IDC_SIZENS));
                        break;
                    }

                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                    break;
                }
            }

            break;
        }

        case WM_MOUSEMOVE:
        {
            // Calculate new size
            static DWORD lastResizeTime = 0;
            const DWORD currentTime = GetTickCount();

            if ((wParam & MK_LBUTTON) && (currentTime - lastResizeTime > PANEL_RESIZE_THRESHOLD))
            {
                if(g_isMovingLeftPanel || g_isMovingLowerPanel)
                {
                    if(g_isMovingLeftPanel)     // Get new width of the Left Panel
                       {g_leftPanelWidth += LOWORD(lParam) - g_previousPanelLocation.x;}

                    if(g_isMovingLowerPanel)    // Get new height of the lower panel
                       {g_lowerPanelHeight -= HIWORD(lParam) - g_previousPanelLocation.y;}

                    g_previousPanelLocation = {LOWORD(lParam), HIWORD(lParam)};

                    // Force the window to adjust & resize
                    RECT clientRect;
                    GetClientRect(hwnd, &clientRect);

                    SendMessage(hwnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(clientRect.right, clientRect.bottom));
                }

                lastResizeTime = currentTime;
            }

            break;
        }

        case WM_LBUTTONDOWN:
        {
            // Find where cursor is and capture mouse input
            HWND hLeftPanel = GetDlgItem(hwnd, 1);
            HWND hLowerPanel = GetDlgItem(hwnd, 2);

            RECT lowerPanelRect, leftPanelRect;
            GetWindowRect(hLeftPanel, &leftPanelRect);
            GetWindowRect(hLowerPanel, &lowerPanelRect);

            ScreenToClient(hwnd, (LPPOINT)&leftPanelRect.left);
            ScreenToClient(hwnd, (LPPOINT)&leftPanelRect.right);
            ScreenToClient(hwnd, (LPPOINT)&lowerPanelRect.left);
            ScreenToClient(hwnd, (LPPOINT)&lowerPanelRect.right);

            if(LOWORD(lParam) >= leftPanelRect.right - CURSOR_REACH && LOWORD(lParam) <= leftPanelRect.right + CURSOR_REACH)
            {
                g_isMovingLeftPanel = true;
                g_previousPanelLocation = {LOWORD(lParam), HIWORD(lParam)};

                SetCapture(hwnd);
            }else if(HIWORD(lParam) >= lowerPanelRect.top - CURSOR_REACH && HIWORD(lParam) <= lowerPanelRect.top + CURSOR_REACH)
            {
                g_isMovingLowerPanel = true;
                g_previousPanelLocation = {LOWORD(lParam), HIWORD(lParam)};

                SetCapture(hwnd);
            }

            break;
        }

        case WM_LBUTTONUP:
        {
            if(g_isMovingLeftPanel || g_isMovingLowerPanel)
            {
                g_isMovingLeftPanel = false;
                g_isMovingLowerPanel = false;
                ReleaseCapture();
            }

            break;
        }

        // WINDOW CREATION & MANAGEMENT //
        case WM_CREATE:
        {
            // Create MenuBar - TOPBAR //
            HMENU hEditorMenu = CreateMenu();

            // File
            HMENU hFileMenu = CreateMenu();
            AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hFileMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file", true)).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 1, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_openfile", true)).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 2, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_openfolder", true)).c_str());
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hFileMenu, MF_STRING, 3, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_closefile", true)).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 4, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_closefolder", true)).c_str());
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hFileMenu, MF_STRING, 5, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_savecurrent", true)).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 6, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_saveall", true)).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 7, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_saveas", true)).c_str());
            AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hFileMenu, MF_STRING, 8, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_reload", true)).c_str());
            AppendMenuW(hFileMenu, MF_STRING, 9, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_file_quit", true)).c_str());
            
            // Edit
            HMENU hEditMenu = CreateMenu();
            AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hEditMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_edit", true)).c_str());
            
            // View
            HMENU hViewMenu = CreateMenu();
            AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hViewMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_view", true)).c_str());
            
            // Project - SOURCE CONTROL!!!!
            HMENU hProjectMenu = CreateMenu();
            AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hProjectMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_project", true)).c_str());
            
            // Extensions - I got to somehow let plugins add to this -_-
            HMENU hPluginMenu = CreateMenu();
            AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hPluginMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_plugin", true)).c_str());
            
            // Settings
            HMENU hSettingsMenu = CreateMenu();
            AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hSettingsMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_settings", true)).c_str());
            
            // Help
            HMENU hHelpMenu = CreateMenu();
            AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hHelpMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_help", true)).c_str());

            // Window Elements - woop //
            HWND hLeftPanel = CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
            HWND hLowerPanel = CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);
            HWND hMiddlePanel = CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);

            // Textbox
            HWND hEditorTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0, hMiddlePanel, (HMENU)10, GetModuleHandle(NULL), NULL);

            // Tabs
            HWND hTabs = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TABCONTROL, "", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, hMiddlePanel, (HMENU)11, GetModuleHandle(NULL), NULL);

            createNewTab(hTabs, "New File");

            // FileExploerer
            
            // Terminal
            SetMenu(hwnd, hEditorMenu);
            break;
        }

        case WM_SIZE:
        {
            // Panels Adjustment
            HWND hLeftPanel = GetDlgItem(hwnd, 1);
            HWND hLowerPanel = GetDlgItem(hwnd, 2);
            HWND hMiddilePanel = GetDlgItem(hwnd, 3);

            MoveWindow(hLeftPanel, 0, 0, g_leftPanelWidth, HIWORD(lParam), TRUE);
            MoveWindow(hLowerPanel, g_leftPanelWidth, HIWORD(lParam) - g_lowerPanelHeight, LOWORD(lParam) - g_leftPanelWidth, g_lowerPanelHeight, TRUE);
            MoveWindow(hMiddilePanel, g_leftPanelWidth, 0, LOWORD(lParam) - g_leftPanelWidth, HIWORD(lParam) - g_lowerPanelHeight, TRUE);

            // Middle Panel Children //
            RECT middlePanelRect;
            GetClientRect(hMiddilePanel, &middlePanelRect);

            HWND hTabs = GetDlgItem(hMiddilePanel, 11);
            MoveWindow(hTabs, 0, 0, middlePanelRect.right, TABS_PANEL_SIZE, TRUE);

            HWND hEditorTextBox = GetDlgItem(hMiddilePanel, 10);
            MoveWindow(hEditorTextBox, 0, TABS_PANEL_SIZE, middlePanelRect.right, middlePanelRect.bottom, TRUE);

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
    // Init Common Controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    // Define application name - Because windows sucks
    std::wstring ApplicationName =  app::win32::system::StringToWideString(app::common::Localisation::GetText("app_name", false));

    // Define the Window Class
    WNDCLASSW WindowClass = {0};
    WindowClass.lpfnWndProc = WindowProcedure;
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.lpszClassName = ApplicationName.c_str();
    WindowClass.hIcon = LoadIcon(WindowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION_ICON));
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