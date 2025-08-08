#ifndef UNICODE
#define UNICODE
#endif

#define  IDM_FILE_NEW 1
#define  IDM_FILE_OPEN 2
#define  IDM_FILE_SAVE 3 

#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;



void CreateCanvasBitmap(HWND hWnd, RECT canvas, HDC &hMemDC, HBITMAP &hBitmap) {
    HDC hdc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hdc);
    hBitmap = CreateCompatibleBitmap(hdc, canvas.right - canvas.left, canvas.bottom - canvas.top);
    SelectObject(hMemDC, hBitmap);

    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    RECT bmpRect = {0, 0, canvas.right - canvas.left, canvas.bottom - canvas.top};
    FillRect(hMemDC, &bmpRect, whiteBrush);
    DeleteObject(whiteBrush);

    ReleaseDC(hWnd, hdc);
}

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
    
    static BOOL fDraw = FALSE;
    static POINT ptPrevious;

    static HDC hMemDC = NULL;
    static HBITMAP hBitmap = NULL;
    
    static RECT clientRect;
    static RECT canvas;

    struct ColorBox {
        RECT rect;
        COLORREF color;
    };
    static ColorBox boxes[20];
    static COLORREF colorChoic = RGB(0, 0, 0);
    static HPEN hPenColor = CreatePen(PS_SOLID, 2, colorChoic);


    switch (uMsg)
    {
        case WM_COMMAND:
        {
            switch (wp)
            {
            case  IDM_FILE_NEW:
                return 0;
            case   IDM_FILE_OPEN:
                return 0;
            case  IDM_FILE_SAVE:
                return 0;
            }
        }
        return 0;

        case WM_DESTROY:
        {
            if (hBitmap) DeleteObject(hBitmap);
            if (hMemDC) DeleteDC(hMemDC);
            PostQuitMessage(0);
        }
        return 0;

        case WM_CREATE:
        {
            AddMenus(hWnd);
            AddControls(hWnd);

            GetClientRect(hWnd, &clientRect);
            canvas = {50, 150, clientRect.right - 50, clientRect.bottom - 50};
            CreateCanvasBitmap(hWnd, canvas, hMemDC, hBitmap);
        }
        return 0;

        case WM_SIZE:
        {
            GetClientRect(hWnd, &clientRect);
            canvas = {50, 150, clientRect.right - 50, clientRect.bottom - 50};

            if (hBitmap) DeleteObject(hBitmap);
            if (hMemDC) DeleteDC(hMemDC);
            CreateCanvasBitmap(hWnd, canvas, hMemDC, hBitmap);

            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;
        
        case WM_LBUTTONDOWN:
        {
            POINT pt = {LOWORD(lp), HIWORD(lp)};
            if(PtInRect(&canvas, pt))
            {
                fDraw = TRUE;
                ptPrevious.x = pt.x - canvas.left;
                ptPrevious.y = pt.y - canvas.top;
            }
            else
            {
                int x = GET_X_LPARAM(lp);
                int y = GET_Y_LPARAM(lp);

                for (int i = 0; i<20; i++){
                    if(PtInRect(&boxes[i].rect, {x, y})){
                        colorChoic = boxes[i].color;
                        if (hPenColor) DeleteObject(hPenColor);
                        hPenColor = CreatePen(PS_SOLID, 2, colorChoic);
                        break;
                    }
                }
            }
        }
        return 0;
        
        case WM_MOUSEMOVE:
        {
            if(fDraw && (wp & MK_LBUTTON))
            {
                POINT pt = {LOWORD(lp), HIWORD(lp)};
                if(PtInRect(&canvas, pt))
                {
                    int x = pt.x - canvas.left;
                    int y = pt.y - canvas.top;
                    
                    SelectObject(hMemDC, hPenColor);

                    MoveToEx(hMemDC, ptPrevious.x, ptPrevious.y, NULL);
                    LineTo(hMemDC, x, y);

                    ptPrevious.x = x;
                    ptPrevious.y = y;

                    InvalidateRect(hWnd, &canvas, FALSE);
                    
                }
                else {

                }
            }
        }
        return 0;

        case WM_LBUTTONUP:
        {
            fDraw = FALSE;
        }
        return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // draw canvas bitmap
            BitBlt(hdc, canvas.left, canvas.top, canvas.right - canvas.left, canvas.bottom - canvas.top, hMemDC, 0, 0, SRCCOPY);

            //toolbar 
            RECT toolbar = {0, 0, clientRect.right, 100};
            FillRect(hdc, &toolbar, CreateSolidBrush(RGB(37,41,40)));

            //Draw lines
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(55,59,58));
            SelectObject(hdc, hPen);

            MoveToEx(hdc, 150, 6, NULL);
            LineTo(hdc, 150, 96);

            MoveToEx(hdc, 448, 6, NULL);
            LineTo(hdc, 448, 96);

            //basic colors
            COLORREF basicColor[]={
                RGB(0, 0, 0),   RGB(127, 127, 127),   RGB(136, 0, 21),
                RGB(237, 28, 36), RGB(255, 127, 39), RGB(255, 242, 0),
                RGB(34, 177, 76),   RGB(0, 162, 232),   RGB(63, 72, 204),
                RGB(163, 73, 164), RGB(255, 255, 255), RGB(195, 195, 195),
                RGB(185, 122, 87), RGB(255, 174, 201), RGB(255, 204, 14),
                RGB(239, 228, 176), RGB(181, 230, 29), RGB(153, 217, 234),
                RGB(112, 146, 190), RGB(200, 191, 231)
            };

            int x = 156, y = 4, colorBoxSize = 20, box_x;
            for (int i = 0; i < 20; i++) {
                if(i<10){ 
                    box_x = x + (i * (colorBoxSize + 4));
                }
                else {
                    y = 28; 
                    box_x = x + ((i-10) * (colorBoxSize + 4));
                }

                boxes[i].rect = {box_x, y, box_x + colorBoxSize, y+colorBoxSize};
                boxes[i].color = basicColor[i];

                HBRUSH brush = CreateSolidBrush(basicColor[i]);
                SelectObject(hdc, brush);
                Rectangle(hdc, box_x, y, box_x + colorBoxSize, y + colorBoxSize);
                DeleteObject(brush);
            }

            EndPaint(hWnd, &ps);
        }
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

    CreateWindowW( L"Button", L"edit color", WS_VISIBLE | WS_CHILD, 398, 4, 44, 44, hWnd, NULL, NULL, NULL);

    CreateWindowW( L"STATIC", L"px", WS_VISIBLE | WS_CHILD | WS_BORDER, 454, 4, 20, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW( L"EDIT", L"1", WS_VISIBLE | WS_CHILD | WS_BORDER, 474, 4, 100, 20, hWnd, NULL, NULL, NULL);
}