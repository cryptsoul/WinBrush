#ifndef UNICODE
#define UNICODE
#endif
#define FILE_MENU_NEW 1
#define FILE_MENU_EXIT 2
#define CHANGE_TITLE 3
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;
HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Paint windows class";

    WNDCLASSW wc = {};
    wc.hCursor = LoadCursorW(NULL, L"IDC_ARROW");
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClassW(&wc)){
        return -1;
    }

    HWND hwnd = CreateWindowExW(
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
            case FILE_MENU_EXIT:
            DestroyWindow(hwnd);
                break;

            case FILE_MENU_NEW:
                MessageBeep(MB_ICONASTERISK); // MB_OK
                break;
            case CHANGE_TITLE: 
                wchar_t text[100];
                GetWindowTextW(hEdit, text, 100);
                SetWindowText(hwnd, text);
                break;
            }
        break;
        case WM_CREATE:
            AddMenus(hwnd);
            AddControls(hwnd);
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

    AppendMenu (hSubMenu, MF_STRING, CHANGE_TITLE, L"Change Title");

    AppendMenu (hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open SubMenu");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"EXIT");

    
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hMenu, MF_STRING, NULL, L"Help");
 

    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd){

    CreateWindowExW(
        0,
        L"Static",
        L"Enter text here :",
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        200, 100, 100, 50,
        hwnd,
        NULL,
        NULL,
        NULL
    );

    hEdit = CreateWindowExW(
        0,
        L"Edit",
        L"...",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        200, 152, 100, 50,
        hwnd,
        NULL,
        NULL,
        NULL
    );
}