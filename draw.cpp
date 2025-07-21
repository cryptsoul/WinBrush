#ifndef UNICODE
#define UNICODE
#endif
#define FILE_MENU_NEW 1
#define FILE_MENU_EXIT 2

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
HMENU hMenu;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Paint windows class";

    WNDCLASSW wc = {};
    wc.hCursor = LoadCursorW(NULL, L"IDC_ARROW");
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClass(&wc)){
        return -1;
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Paint",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInst,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp){
    switch (umsg)
    {
        case WM_COMMAND:

            switch (wp)
            {
            case FILE_MENU_EXIT :
            DestroyWindow(hwnd);
                break;

            case FILE_MENU_NEW :
                MessageBeep(MB_ICONASTERISK); // MB_OK
                break;
            }

        case WM_CREATE:
            AddMenus(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, umsg, wp, lp);
}

void AddMenus(HWND hwnd ){
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hSubMenu = CreateMenu();

    AppendMenu (hSubMenu, MF_STRING, NULL, L"SubMenu Item");

    AppendMenu (hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open SubMenu");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"EXIT");

    
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hMenu, MF_STRING, NULL, L"Help");
 

    SetMenu(hwnd, hMenu);
}