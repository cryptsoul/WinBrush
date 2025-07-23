#ifndef UNICODE
#define UNICODE
#endif
#define FILE_MENU_NEW 1
#define FILE_MENU_EXIT 2
#define GENERATE_BUTTON 3


#include <windows.h>
#include <stdlib.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;
HWND hName, hAge, hOut;

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
            case GENERATE_BUTTON:
                    wchar_t name[30], age[10], out[50];
                    GetWindowText(hName, name, 30);
                    GetWindowText(hAge, age, 10);

                    wcscpy(out, name);
                    wcscat(out, L" is ");
                    wcscat(out, age);
                    wcscat(out, L" years old.");

                    SetWindowText(hOut, out);
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

    AppendMenu (hSubMenu, MF_STRING, NULL, L"SubMenu Item");

    AppendMenu (hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open SubMenu");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"EXIT");

    
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hMenu, MF_STRING, NULL, L"Help");
 

    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd){
    CreateWindowW(L"Static", L"Name: ", WS_VISIBLE | WS_CHILD, 100, 50, 100, 25, hwnd, NULL, NULL, NULL);
    hName = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 205, 50, 100, 25, hwnd, NULL, NULL, NULL);
    
    CreateWindowW(L"Static", L"Age: ", WS_VISIBLE | WS_CHILD, 100, 100, 100, 25, hwnd, NULL, NULL, NULL);
    hAge = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 205, 100, 100, 25, hwnd, NULL, NULL, NULL);
    
    CreateWindowW(L"Button", L"Generate", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 150, 100, 25, hwnd, (HMENU)GENERATE_BUTTON, NULL, NULL);

    hOut = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 200, 400, 200, hwnd, NULL, NULL, NULL);
}