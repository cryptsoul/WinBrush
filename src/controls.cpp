#include <windows.h>
#include <Gdiplus.h>
#include <commctrl.h>
#include "controls.h"

HWND hSlider = nullptr;

void AddMenus(HWND hwnd )
{
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hEditMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();
    HMENU hExtrasMenu = CreateMenu();
    HMENU hThemesSubMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu (hFileMenu, MF_STRING,  ID_MENU_NEW, L"New");
    AppendMenu (hFileMenu, MF_STRING,   ID_MENU_OPEN, L"Open");
    AppendMenu(hFileMenu, MF_STRING,  ID_MENU_SAVE, L"Save");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"Edit");
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_UNDO, L"Undo");
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_REPEAT, L"Repeat");
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_HISTORY, L"History");
    AppendMenu (hEditMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_CUT, L"Cut");
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_COPY, L"Copy");
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_PASTE, L"Paste");
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_CLEAR_SELECT, L"Clear Selection");
    AppendMenu (hEditMenu, MF_STRING, ID_MENU_SELECT_ALL, L"Selest All");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"View");
    AppendMenu (hHelpMenu, MF_STRING, ID_MENU_HELP_TOPICS, L"Help Topics");
    AppendMenu (hHelpMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu (hHelpMenu, MF_STRING, ID_MENU_ABOUT, L"About WinBrush");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hExtrasMenu, L"Extras");
    AppendMenu (hExtrasMenu, MF_STRING, ID_MENU_HISTORY, L"History");
    AppendMenu (hExtrasMenu, MF_STRING, ID_MENU_GIF, L"Render History As GIF");
    AppendMenu (hExtrasMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu (hExtrasMenu, MF_CHECKED, ID_MENU_RANDOM, L"Draw Randomly");
    AppendMenu (hExtrasMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu (hExtrasMenu, MF_POPUP, (UINT_PTR)hThemesSubMenu, L"Themes");
    AppendMenu (hThemesSubMenu, MF_STRING, ID_MENU_THEMES, L"Classic Light");
    AppendMenu (hThemesSubMenu, MF_STRING, ID_MENU_THEMES, L"Classic Dark");
    AppendMenu (hExtrasMenu, MF_CHECKED, ID_MENU_ENLARGE_UI, L"Enlarge UI");

    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd)
{
    //tools
    CreateWindowW( L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 4, 4, 44, 44, hwnd, (HMENU)ID_BTN_PENCIL, NULL, NULL);
    CreateWindowW( L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 52, 4, 44, 44, hwnd, (HMENU)ID_BTN_FILL, NULL, NULL);
    CreateWindowW( L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 100, 4, 44, 44, hwnd, (HMENU)ID_BTN_TEXT, NULL, NULL);
    CreateWindowW( L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 4, 52, 44, 44, hwnd, (HMENU)ID_BTN_ERASER, NULL, NULL);
    CreateWindowW( L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 52, 52, 44, 44, hwnd, (HMENU)ID_BTN_COLOR_PICKER, NULL, NULL);
    CreateWindowW( L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 100, 52, 44, 44, hwnd, (HMENU)ID_BTN_MAGNIFIER, NULL, NULL);
    
    CreateWindowW( L"Button", L"", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 398, 4, 44, 44, hwnd, (HMENU)ID_BTN_CUSTOM_COLOR, NULL, NULL);
    
    hSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 454, 4, 200, 30, hwnd, (HMENU)ID_SLIDER, NULL, NULL);

    //shapes
    CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | WS_BORDER |  BS_OWNERDRAW, 666, 4, 44, 44, hwnd, (HMENU)ID_BTN_RECTANGLE, NULL, NULL);
    CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | WS_BORDER |  BS_OWNERDRAW, 714, 4, 44, 44, hwnd, (HMENU)ID_BTN_ELLIPSE, NULL, NULL);
    CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | WS_BORDER |  BS_OWNERDRAW, 762, 4, 44, 44, hwnd, (HMENU)ID_BTN_LINE, NULL, NULL);
}