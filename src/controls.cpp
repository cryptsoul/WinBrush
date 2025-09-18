#include <windows.h>
#include <Gdiplus.h>
#include <commctrl.h>
#include "controls.h"

HWND hSlider = nullptr;

void AddMenus(HWND hwnd )
{
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu (hFileMenu, MF_STRING,  ID_MENU_NEW, L"New");
    AppendMenu (hFileMenu, MF_STRING,   ID_MENU_OPEN, L"Open");
    AppendMenu(hFileMenu, MF_STRING,  ID_MENU_SAVE, L"Save");

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