#include "JACE/_win/winUI.h"
#include "JACE/_win/win32.h"
#include "JACE/_win/resources.h"

#include "JACE/common/sessionManager.h"
#include "JACE/common/localesHandeler.h"
#include "JACE/common/logHandeler.h"
#include "JACE/common/fileHandeler.h"

#include <vector>
#include <windows.h>
#include <unordered_map>
#include <commctrl.h>
#include <string>
#include <algorithm>


// Global Variables //
#define CURSOR_REACH 10
#define PANEL_RESIZE_THRESHOLD 10
#define TABS_PANEL_SIZE 30

// Panels
bool g_isMovingLeftPanel = false;
bool g_isMovingLowerPanel = false;

int g_leftPanelWidth = 200;
int g_lowerPanelHeight = 100;

POINT g_previousPanelLocation = {0};

// Tabs
struct tabInfo
{
    std::string fileLocation;
    std::string storedText;
};

std::string g_currentTab;
std::vector<std::string> g_modifiedTabs;
std::unordered_map<std::string, tabInfo> g_tabMap;


// APPLICATION FUNCTIONS
void app_saveTabWithCustomLocation()
{
    //TODO le do do do, i think im starting to go crazy
}

void app_saveTabs(bool doAllTabs)
{
    //TODO
}

void app_CreateNewTab(HWND hMiddilePanel, std::string tabName, std::string fileLocation)
{
    HWND hEditorTextBox = GetDlgItem(hMiddilePanel, 10);
    HWND hTabManager = GetDlgItem(hMiddilePanel, 11);

    // Create Tab
    TCITEM tie;
    tie.mask = TCIF_TEXT;
    tie.pszText = (LPSTR)tabName.c_str();

    if(TabCtrl_GetItemCount(hTabManager) == 0)
    {
        TabCtrl_InsertItem(hTabManager, 0, &tie);
    }else
    {
        TabCtrl_InsertItem(hTabManager, TabCtrl_GetItemCount(hTabManager) + 1, &tie);
    }

    // Get text from file (if appicable)
    std::string fileText;

    if(!fileLocation.empty())
    {
        fileText = app::common::fileHandeler::GetTextFromFile(fileLocation);
    }

    // Store tab for later use
    g_tabMap[tabName] = {fileLocation, fileText};
}

void app_OpenTab(HWND hMiddilePanel, std::string tabName)
{
    HWND hEditorTextBox = GetDlgItem(hMiddilePanel, 10);

    if(g_tabMap.count(g_currentTab))
    {
        // Get Text from hEdtiorTextBox, and save text to previous tabInfo
        int textLength = GetWindowTextLength(hEditorTextBox) + 1;
        TCHAR* buffer = new TCHAR[textLength];

        GetWindowText(hEditorTextBox, buffer, textLength);
        g_tabMap[g_currentTab].storedText = std::string(buffer);

        delete[] buffer;
    }

    std::wstring wtext(g_tabMap[tabName].storedText.begin(), g_tabMap[tabName].storedText.end());
    SetWindowTextW(hEditorTextBox, wtext.c_str());

    // Change current tab after completion
    g_currentTab = tabName;
}

// MIDDLEPANNEL CALLBACKS //
LRESULT middlePanel_wm_WhenNotified(HWND hMiddilePanel, WPARAM wParam, LPARAM lParam)
{
    if(((LPNMHDR)lParam) -> idFrom == 11 && ((LPNMHDR)lParam) -> code == TCN_SELCHANGE)
    {
        TCHAR tabName[256];
        int tabIndex = TabCtrl_GetCurSel(((LPNMHDR)lParam)->hwndFrom);

        TCITEM tie;
        tie.mask = TCIF_TEXT;
        tie.pszText = tabName;
        tie.cchTextMax = sizeof(tabName)/sizeof(tabName[0]);

        if(TabCtrl_GetItem(((LPNMHDR)lParam)->hwndFrom, tabIndex, &tie))
            app_OpenTab(hMiddilePanel, tie.pszText);
    }

    return 0;
}

LRESULT middlePanel_wm_OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if(HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == 10)
    {   
        // Don't add to the modified tabs list if its allready there
        if(g_tabMap.count(g_currentTab) && std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), g_currentTab) == g_modifiedTabs.end())
        {
            g_modifiedTabs.push_back(g_currentTab);
        }
    }

    return 0;
}

LRESULT middlePanel_wm_OnFileDrop(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    HDROP hDrop = (HDROP)wParam;
    int fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

    if(fileCount > 0)
    {
        for(int i = 0; i < fileCount; ++i)
        {
            char filePath[MAX_PATH];

            if(DragQueryFile(hDrop, i, filePath, MAX_PATH) != 0)
            {
                // Extract file name
                std::string filePathString = filePath;
                std::string fileName = filePathString.substr(filePathString.find_last_of("\\/") + 1);

                app_CreateNewTab(hwnd, fileName, filePathString);
            }
        }
    }

    DragFinish(hDrop);
    return 0;
}

LRESULT CALLBACK cb_MiddlePanel(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg) 
    {
        case WM_NOTIFY:
            return middlePanel_wm_WhenNotified(hwnd, wParam, lParam);

        case WM_COMMAND:
            return middlePanel_wm_OnCommand(hwnd, wParam, lParam);

        case WM_DROPFILES:
            return middlePanel_wm_OnFileDrop(hwnd, wParam, lParam);
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

// WINDOW MANAGER FUNCTIONS //
LRESULT wm_OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    // MenuBar
    HMENU hEditorMenu = CreateMenu();

    // MenuBar - File
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

    // MenuBar - Edit
    HMENU hEditMenu = CreateMenu();
    AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hEditMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_edit", true)).c_str());

    // MenuBar - View
    HMENU hViewMenu = CreateMenu();
    AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hViewMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_view", true)).c_str());

    // MenuBar - Project - SOURCE CONTROL!!!!
    HMENU hProjectMenu = CreateMenu();
    AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hProjectMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_project", true)).c_str());

    // MenuBar - Extensions - I got to somehow let plugins add to this -_-
    HMENU hPluginMenu = CreateMenu();
    AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hPluginMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_plugin", true)).c_str());

    // MenuBar - Settings
    HMENU hSettingsMenu = CreateMenu();
    AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hSettingsMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_settings", true)).c_str());

    // MenuBar - Help
    HMENU hHelpMenu = CreateMenu();
    AppendMenuW(hEditorMenu, MF_POPUP, (UINT_PTR)hHelpMenu, app::win32::system::StringToWideString(app::common::Localisation::GetText("menu_help", true)).c_str());

    SetMenu(hwnd, hEditorMenu);

    // Window Panels //
    HWND hLeftPanel = CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
    HWND hLowerPanel = CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);
    HWND hMiddlePanel = CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);

    DragAcceptFiles(hMiddlePanel, TRUE);

    // Middle Panel elements
    HWND hTabManager = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TABCONTROL, "", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, hMiddlePanel, (HMENU)11, GetModuleHandle(NULL), NULL);
    HWND hEditorTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0, hMiddlePanel, (HMENU)10, GetModuleHandle(NULL), NULL);

    // Subclassing
    SetWindowSubclass(hMiddlePanel, cb_MiddlePanel, 0, 0);
    return 0;
}

LRESULT wm_OnSizeChange(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    // Set Panels Size
    HWND hLeftPanel = GetDlgItem(hwnd, 1);
    HWND hLowerPanel = GetDlgItem(hwnd, 2);
    HWND hMiddilePanel = GetDlgItem(hwnd, 3);

    MoveWindow(hLeftPanel, 0, 0, g_leftPanelWidth, HIWORD(lParam), TRUE);
    MoveWindow(hLowerPanel, g_leftPanelWidth, HIWORD(lParam) - g_lowerPanelHeight, LOWORD(lParam) - g_leftPanelWidth, g_lowerPanelHeight, TRUE);
    MoveWindow(hMiddilePanel, g_leftPanelWidth, 0, LOWORD(lParam) - g_leftPanelWidth, HIWORD(lParam) - g_lowerPanelHeight, TRUE);

    // Middle Panel
    RECT middlePanelRect;
    GetClientRect(hMiddilePanel, &middlePanelRect);

    HWND hTabManager = GetDlgItem(hMiddilePanel, 11);
    MoveWindow(hTabManager, 0, 0, middlePanelRect.right, TABS_PANEL_SIZE, TRUE);

    HWND hEditorTextBox = GetDlgItem(hMiddilePanel, 10);
    MoveWindow(hEditorTextBox, 0, TABS_PANEL_SIZE, middlePanelRect.right, middlePanelRect.bottom - 30, TRUE);

    return 0;
}

LRESULT wm_OnDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    HWND hMiddilePanel = GetDlgItem(hwnd, 3);
    app::common::log::LogToFile("application", "[Win32] Destroy Message Recived");

    // TODO

    RemoveWindowSubclass(hMiddilePanel, cb_MiddlePanel, 0);
    PostQuitMessage(0);
    return 0;
}


// WINDOW MANAGER INPUT CONTROL //
LRESULT wm_SetMouseCursor(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if(LOWORD(lParam) == HTCLIENT)
    {
        // Change cursor when over edges of panels
        HWND hLeftPanel = GetDlgItem(hwnd, 1);
        HWND hLowerPanel = GetDlgItem(hwnd, 2);

        POINT cursorPos;
        RECT lowerPanelRect, leftPanelRect, ClientRect;

        // Get CusorPosistion inside the Window
        GetCursorPos(&cursorPos);
        ScreenToClient(hwnd, &cursorPos);

        GetWindowRect(hLeftPanel, &leftPanelRect);
        GetWindowRect(hLowerPanel, &lowerPanelRect);
        GetClientRect(hwnd, &ClientRect);

        MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&leftPanelRect, 2);
        MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&lowerPanelRect, 2);

        if(cursorPos.x >= leftPanelRect.right - CURSOR_REACH && cursorPos.x <= leftPanelRect.right + CURSOR_REACH)
        {
            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
            return 0;
        }else if(cursorPos.y >= lowerPanelRect.top - CURSOR_REACH && cursorPos.y <= lowerPanelRect.top + CURSOR_REACH)
        {
            SetCursor(LoadCursor(NULL, IDC_SIZENS));
            return 0;
        }else
        {
            if(cursorPos.x <= 0 || cursorPos.x >= ClientRect.right - 1)
            {
                SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                return 0;
            }else if(cursorPos.y >= ClientRect.bottom - 1)
            {
                SetCursor(LoadCursor(NULL, IDC_SIZENS));
                return 0;
            }

            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return 0;
        }
    }

    return 0;
}

LRESULT wm_OnLeftMouseDown(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
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

    return 0; 
}

LRESULT wm_OnLeftMouseUp(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if(g_isMovingLeftPanel || g_isMovingLowerPanel)
    {
        g_isMovingLeftPanel = false;
        g_isMovingLowerPanel = false;
        ReleaseCapture();
    }

    return 0;
}

LRESULT wm_OnMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
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

    return 0;
}

// WINDOW MANAGER INPUT OUTPUT //
LRESULT wm_OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    switch(LOWORD(wParam))
    {
        // Toolbar - File
        case 9: // Quit
        {
            DestroyWindow(hwnd);
            break;
        }
        case 8: // Reload
        {
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
            break;
        }
        case 7: // Save As
        {
            app_saveTabWithCustomLocation();
            break;
        }
        case 6: // Save All
        {
            app_saveTabs(true);
            break;
        }
        case 5: // Save Current
        {
            app_saveTabs(false);
            break;
        }
    }

    return 0;
}

// MAIN WINDOW //
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_MOUSEMOVE:
        return wm_OnMouseMove(hwnd, wParam, lParam);

    case WM_LBUTTONDOWN:
        return wm_OnLeftMouseDown(hwnd, wParam, lParam);

    case WM_LBUTTONUP:
        return wm_OnLeftMouseUp(hwnd, wParam, lParam);

    case WM_SETCURSOR:
        return wm_SetMouseCursor(hwnd, wParam, lParam);

    case WM_COMMAND:
        return wm_OnCommand(hwnd, wParam, lParam);
    
    case WM_CREATE:
        return wm_OnCreate(hwnd, wParam, lParam);

    case WM_SIZE:
        return wm_OnSizeChange(hwnd, wParam, lParam);

    case WM_DESTROY:
        return wm_OnDestroy(hwnd, wParam, lParam);

    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void app::win32::UI::w32_createEditorWindow()
{
    // Start common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    // Define WindowClass
    std::wstring ApplicationName = app::win32::system::StringToWideString(app::common::Localisation::GetText("app_name", false));

    WNDCLASSW WindowClass = {0};
    WindowClass.lpfnWndProc = WindowProcedure;
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.lpszClassName = ApplicationName.c_str();
    WindowClass.hIcon = LoadIcon(WindowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION_ICON));
    RegisterClassW(&WindowClass);

    // Define Window Style
    DWORD WindowStyle;
    WindowStyle = WS_OVERLAPPEDWINDOW;

    // Create Window
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
        NULL);

    // Display Window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG wMsg;
    while (GetMessage(&wMsg, NULL, 0, 0))
    {
        TranslateMessage(&wMsg);
        DispatchMessage(&wMsg);
    }
}