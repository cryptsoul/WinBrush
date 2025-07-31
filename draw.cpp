#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

void AddMenus(HWND);

HMENU hMenu;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow){

    const wchar_t CLASS_NAME[] = L"Main";

    WNDCLASS wc = {};

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClassW(&wc)) return -1;

    HWND hWnd = CreateWindowW(CLASS_NAME, L"WinBrush", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

    if(hWnd == NULL) return 0;

    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0,0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp){
    switch (uMsg)
    {
        case WM_COMMAND:
            switch (wp)
            {
            case 1:
                /* code */
                return 0;
            }
        return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            AddMenus(hWnd);
            return 0;
    }
    return DefWindowProc(hWnd, uMsg, wp, lp);
}

void AddMenus(HWND hWnd ){

    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu (hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu (hMenu, MF_STRING, NULL, L"Help");

    SetMenu(hWnd, hMenu);
}