#include "JACE/_win/MainWindow.h"
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


// Tabs
void app_RemapTabInfo(HWND hMiddilePanel, std::string oldTab, std::string newTabLocation)
{   
    // Were only not using g_currentTab, since this can happen on non current tabs
    HWND hTabManager = GetDlgItem(hMiddilePanel, 11);
    std::string newTabName = newTabLocation.substr(newTabLocation.find_last_of("\\/") + 1);

    app::common::log::LogToFile("application", "[Win32] Remapping TabInfo: " + oldTab + " --> " + newTabName);

    // Update TabName
    for(int i = 0; i < TabCtrl_GetItemCount(hTabManager); ++i)
    {
        TCITEM tie;
        tie.mask = TCIF_TEXT;
        tie.pszText = new TCHAR[MAX_PATH];
        tie.cchTextMax = MAX_PATH;

        TabCtrl_GetItem(hTabManager, i, &tie);

        // Update Tab name
        if(strcmp(oldTab.c_str(), tie.pszText) == 0)
        {
            app::common::log::LogToFile("application", "[Win32] Updated TabName: " + oldTab + " -> " + newTabName);

            strcpy(tie.pszText, newTabName.c_str());
            TabCtrl_SetItem(hTabManager, i, &tie);

            delete[] tie.pszText;
            break;
        }
    }

    if(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), oldTab) != g_modifiedTabs.end())
        {g_modifiedTabs.erase(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), oldTab));}

    g_tabMap[newTabName] = {newTabLocation, g_tabMap[oldTab].storedText};
    g_tabMap.erase(oldTab);
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

    if(!IsWindowVisible(hEditorTextBox))
    {
        ShowWindow(hEditorTextBox, SW_SHOW);
    }
}

void app_CloseTab(HWND hMiddilePanel, std::string tabName)
{
    if(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), tabName) != g_modifiedTabs.end())
    {
        if(g_tabMap.count(tabName))
        {
            HWND hEditorTextBox = GetDlgItem(hMiddilePanel, 10);

            int textLength = GetWindowTextLength(hEditorTextBox) + 1;
            TCHAR* buffer = new TCHAR[textLength];

            GetWindowText(hEditorTextBox, buffer, textLength);
            g_tabMap[tabName].storedText = std::string(buffer);

            delete[] buffer;
        }

        int msgBoxResult = MessageBoxW(NULL, app::win32::system::StringToWideString(app::common::Localisation::GetText("warning_unsavedChanges", true)).c_str(), app::win32::system::StringToWideString(app::common::Localisation::GetText("app_name", true)).c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);

        if(msgBoxResult == IDYES)
        {
            // Modified version of mode 1, to use tabName instead of the current tab
            app::common::log::LogToFile("application", "[Win32] Saving file: " + tabName);
            app::common::fileHandeler::UpdateFileText(g_tabMap[tabName].fileLocation, g_tabMap[tabName].storedText);
            g_modifiedTabs.erase(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), tabName));
        }

        g_modifiedTabs.erase(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), tabName));
    }
}


// File Management
void app_saveTabs(int mode, HWND hwnd)
{
    HWND hMiddilePanel = GetDlgItem(hwnd, 3);

    // We need to get the text from the current tab because it doesnt save to storedText unless they moved to a new tab
    if(g_tabMap.count(g_currentTab))
    {
        HWND hEditorTextBox = GetDlgItem(hMiddilePanel, 10);

        int textLength = GetWindowTextLength(hEditorTextBox) + 1;
        TCHAR* buffer = new TCHAR[textLength];

        GetWindowText(hEditorTextBox, buffer, textLength);
        g_tabMap[g_currentTab].storedText = std::string(buffer);

        delete[] buffer;
    }

    // Save Current
    if(mode == 1)
    {
        if(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), g_currentTab) != g_modifiedTabs.end())
        {
            app::common::log::LogToFile("application", "[Win32] Saving file: " + g_currentTab);
            app::common::fileHandeler::UpdateFileText(g_tabMap[g_currentTab].fileLocation, g_tabMap[g_currentTab].storedText);
            g_modifiedTabs.erase(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), g_currentTab));
        }
    }else if(mode == 2) // Save all
    {
        for(int i = g_modifiedTabs.size() - 1; i >= 0; --i)
        {
            std::string currentFile = g_modifiedTabs[i];

            app::common::log::LogToFile("application", "[Win32] Saving file: " + currentFile);
            app::common::fileHandeler::UpdateFileText(g_tabMap[currentFile].fileLocation, g_tabMap[currentFile].storedText);
            g_modifiedTabs.erase(g_modifiedTabs.begin() + i);
        }
    }else if(mode == 3) // Save to location
    {
        if(!g_currentTab.empty())
        {
            TCHAR TargetLocation[MAX_PATH] = {0};

            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
            ofn.lpstrFile = TargetLocation;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

            strncpy(TargetLocation, g_tabMap[g_currentTab].fileLocation.c_str(), MAX_PATH);

            if(GetSaveFileName(&ofn))
            {
                app::common::log::LogToFile("application", "[Win32] Saving file: " + g_tabMap[g_currentTab].fileLocation + " --> " + TargetLocation);
                app::common::fileHandeler::TransferFile(g_tabMap[g_currentTab].fileLocation, TargetLocation, g_tabMap[g_currentTab].storedText);

                // Basically remaps the tab...
                if(g_currentTab == std::string(TargetLocation).substr(std::string(TargetLocation).find_last_of("\\/") + 1))
                {
                    if(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), g_currentTab) != g_modifiedTabs.end())
                        {g_modifiedTabs.erase(std::find(g_modifiedTabs.begin(), g_modifiedTabs.end(), g_currentTab));}

                    g_tabMap[g_currentTab].fileLocation = TargetLocation;
                }else 
                {
                    app_RemapTabInfo(hMiddilePanel, g_currentTab, TargetLocation);
                    g_currentTab = std::string(TargetLocation).substr(std::string(TargetLocation).find_last_of("\\/") + 1);
                }
            }
        }
    }
}

void app_openFile(HWND hwnd)
{
    HWND hMiddilePanel = GetDlgItem(hwnd, 3);
    TCHAR TargetLocation[MAX_PATH] = {0};

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = TargetLocation;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if(GetOpenFileName(&ofn))
    {
        app_CreateNewTab(hMiddilePanel, std::string(TargetLocation).substr(std::string(TargetLocation).find_last_of("\\/") + 1), TargetLocation);
        app_OpenTab(hMiddilePanel, std::string(TargetLocation).substr(std::string(TargetLocation).find_last_of("\\/") + 1));
    }
}

void app_closeFile(HWND hwnd)
{

}


// application management
void app_AfterCreation(HWND hwnd)
{
    HWND hMiddilePanel = GetDlgItem(hwnd, 3);
    HWND hEditorTextBox = GetDlgItem(hMiddilePanel, 10);

    if(g_currentTab.empty())
    {
        ShowWindow(hEditorTextBox, SW_HIDE);
    }
}

void app_BeforeExit(HWND hwnd)
{

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
    std::string fileName;
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
                fileName = filePathString.substr(filePathString.find_last_of("\\/") + 1);

                app_CreateNewTab(hwnd, fileName, filePathString);
            }
        }
    }
    
    DragFinish(hDrop);
    app_OpenTab(hwnd, fileName);
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

    // Middle Panel
    DragAcceptFiles(hMiddlePanel, TRUE);

    HWND hTabManager = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TABCONTROL, "", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, hMiddlePanel, (HMENU)11, GetModuleHandle(NULL), NULL);
    HWND hEditorTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL, 0, 0, 0, 0, hMiddlePanel, (HMENU)10, GetModuleHandle(NULL), NULL);

    SetWindowSubclass(hMiddlePanel, cb_MiddlePanel, 0, 0);

    app_AfterCreation(hwnd);
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

    app_BeforeExit(hwnd);
    RemoveWindowSubclass(hMiddilePanel, cb_MiddlePanel, 0);
    PostQuitMessage(0);

    app::common::log::LogToFile("application", "[Win32] Destroying Main Window");
    app::common::log::LogToFile("application", "[Win32] Removing WindowSubclasses");
    return 0;
}

LRESULT wm_OnSystemCommand(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(wParam == SC_CLOSE)
    {
        // Check if user has unsaved tabs
        if(!g_modifiedTabs.empty())
        {
            int msgBoxResult = MessageBoxW(NULL, app::win32::system::StringToWideString(app::common::Localisation::GetText("warning_unsavedChanges", true)).c_str(), app::win32::system::StringToWideString(app::common::Localisation::GetText("app_name", true)).c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);

            if(msgBoxResult == IDYES)
            {
                app_saveTabs(2, hwnd);
                DestroyWindow(hwnd);
            }else if(msgBoxResult == IDNO)
            {
                DestroyWindow(hwnd);
            }
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
            app_saveTabs(3, hwnd);
            break;
        }
        case 6: // Save All
        {
            app_saveTabs(2, hwnd);
            break;
        }
        case 5: // Save Current
        {
            app_saveTabs(1, hwnd);
            break;
        }
        case 4: // Close Folder
        {
            break;
        }
        case 3: // Close File
        {
            break;
        }
        case 2: // Open Folder
        {
            break;
        }
        case 1: // Open File
        {
            app_openFile(hwnd);
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

    case WM_SYSCOMMAND:
        return wm_OnSystemCommand(hwnd, uMsg, wParam, lParam);

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