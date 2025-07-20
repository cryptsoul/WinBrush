#ifdef UNICODE 
#define UNICODE 
#endif

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain (HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow){

    const wchar_t CLASS_NAME[] = L"Paint windows class";

    WNDCLASS wc = {};

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
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

    MSG msg = {};
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
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, umsg, wp, lp);
}