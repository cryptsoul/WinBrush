#ifndef UNICODE
#define UNICODE
#endif

#define  IDM_FILE_NEW 1
#define  IDM_FILE_OPEN 2
#define  IDM_FILE_SAVE 3 

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow){

    const wchar_t CLASS_NAME[] = L"Main";

    WNDCLASS wc = {};

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(28,34,33));
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClassW(&wc)) return -1;

    HWND hWnd = CreateWindowW(CLASS_NAME, L"WinBrush", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

    if(hWnd == NULL) return 0;

    ShowWindow(hWnd, SW_MAXIMIZE);

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
            case  IDM_FILE_NEW:
                /* code */
                return 0;
            case   IDM_FILE_OPEN:
                /* code */
                return 0;
            case  IDM_FILE_SAVE:
                /* code */
                return 0;

            }
        return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            AddMenus(hWnd);
            AddControls(hWnd);
            return 0;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT clientRect;

            GetClientRect(hWnd, &clientRect);
            RECT toolbar = {0, 0, clientRect.right, 100};
            FillRect(hdc, &toolbar, CreateSolidBrush(RGB(37,41,40)));

            RECT canvas = {50, 150, clientRect.right - 50, clientRect.bottom - 50};
            FillRect(hdc, &canvas, CreateSolidBrush(RGB(255,255,255)));
            return 0;
    }
    return DefWindowProc(hWnd, uMsg, wp, lp);
}

void AddMenus(HWND hWnd ){

    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    
    AppendMenu (hFileMenu, MF_STRING,  IDM_FILE_NEW, L"New");
    AppendMenu (hFileMenu, MF_STRING,   IDM_FILE_OPEN, L"Open");
    AppendMenu(hFileMenu, MF_STRING,  IDM_FILE_SAVE, L"Save");

    SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd){

}