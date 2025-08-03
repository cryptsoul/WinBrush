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
            
            //toolbar 
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            RECT toolbar = {0, 0, clientRect.right, 100};
            FillRect(hdc, &toolbar, CreateSolidBrush(RGB(37,41,40)));

            //Draw line
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(55,59,58));
            SelectObject(hdc, hPen);
            MoveToEx(hdc, 150, 6, NULL);
            LineTo(hdc, 150, 96);

            //basic color
            COLORREF basicColor[]={
                RGB(255, 0, 0),   RGB(0, 255, 0),   RGB(0, 0, 255),
                RGB(255, 255, 0), RGB(0, 255, 255), RGB(255, 0, 255),
                RGB(128, 0, 0),   RGB(0, 128, 0),   RGB(0, 0, 128),
                RGB(128, 128, 0), RGB(0, 128, 128), RGB(128, 0, 128),
                RGB(192, 192, 192), RGB(128, 128, 128),
                RGB(255, 128, 0), RGB(128, 255, 0), RGB(0, 128, 255),
                RGB(255, 0, 128), RGB(128, 0, 255), RGB(0, 255, 128)
            };

            int x = 156, y = 4, colorBoxSize = 20, box_X;
            for (int i = 0; i < 20; i++) {
                if(i<10){ 
                    box_X = x + (i * (colorBoxSize + 4));
                }
                else {
                    y = 28; 
                    box_X = x + ((i-10) * (colorBoxSize + 4));
                }
                HBRUSH brush = CreateSolidBrush(basicColor[i]);
                SelectObject(hdc, brush);
                Rectangle(hdc, box_X, y, box_X + colorBoxSize, y + colorBoxSize);
                DeleteObject(brush);
            }

            //the canvas for now 
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

    //tools
    CreateWindowW( L"Button", L"Pencil", WS_VISIBLE | WS_CHILD, 4, 4, 44, 44, hWnd, NULL, NULL, NULL);
    CreateWindowW( L"Button", L"Fill", WS_VISIBLE | WS_CHILD, 52, 4, 44, 44, hWnd, NULL, NULL, NULL);
    CreateWindowW( L"Button", L"Text", WS_VISIBLE | WS_CHILD, 100, 4, 44, 44, hWnd, NULL, NULL, NULL);
    CreateWindowW( L"Button", L"Eraser", WS_VISIBLE | WS_CHILD, 4, 52, 44, 44, hWnd, NULL, NULL, NULL);
    CreateWindowW( L"Button", L"Color picker", WS_VISIBLE | WS_CHILD, 52, 52, 44, 44, hWnd, NULL, NULL, NULL);
    CreateWindowW( L"Button", L"Magnifier", WS_VISIBLE | WS_CHILD, 100, 52, 44, 44, hWnd, NULL, NULL, NULL);

    //Shapes
}
