#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>


enum ToolType{
    TOOL_PENCIL,
    TOOL_FILL, 
    TOOL_TEXT,
    TOOL_ERASER,
    TOOL_COLOR_PICKER,
    TOOL_MAGNIFIER,

    SHAPE_RECTANGLE,
    SHAPE_ELLIPSE,
    SHAPE_LINE,

    TOOL_NONE
};

enum MenuAndButtonIDs {
    ID_MENU_NEW = 1,
    ID_MENU_OPEN,
    ID_MENU_SAVE,
    ID_BTN_CUSTOM_COLOR,
    ID_SLIDER,
    ID_BTN_RECTANGLE,
    ID_BTN_ELLIPSE,
    ID_BTN_LINE,
    ID_BTN_PENCIL,
    ID_BTN_ERASER,
    ID_BTN_FILL,
    ID_BTN_TEXT,
    ID_BTN_COLOR_PICKER,
    ID_BTN_MAGNIFIER
};

enum DrawTarget {
    BITMAP_PREVIEW,
    BITMAP_PERMANENT,
    BITMAP_WINDOW
};

HMENU hMenu;
static HWND hSlider;

static int penWidth = 1;
static COLORREF colorChoice = RGB(0, 0, 0);
static COLORREF customColors[16];
static HPEN hPenColor = CreatePen(PS_SOLID, penWidth, colorChoice);

static POINT pStart;

ToolType currentTool = TOOL_PENCIL;

static HDC hPermanentDC, hPreviewDC;
static HBITMAP hBitmap, hPreviewBitmap;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp);
void AddMenus(HWND);
void AddControls(HWND);
void CustomColorBox(HWND);
void drawing (HWND, RECT, POINT, HDC, POINT&);
void CreateCanvasBitmap(HWND, RECT, HDC&, HBITMAP&, HDC&, HBITMAP&);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow){

    const wchar_t CLASS_NAME[] = L"Main";

    WNDCLASS wc = {};

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(28,34,33));
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClassW(&wc)) return -1;

    HWND hwnd = CreateWindowW(CLASS_NAME, L"WinBrush", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

    if(hwnd == NULL) return 0;

    ShowWindow(hwnd, SW_MAXIMIZE);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0,0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp){
    
    static BOOL fDraw = FALSE;
    static POINT ptPrevious;
    POINT pt = {LOWORD(lp), HIWORD(lp)};

    
    static RECT clientRect;
    static RECT canvas;

    struct ColorBox {
        RECT rect;
        COLORREF color;
    };
    static ColorBox boxes[20];

    switch (uMsg)
    {
        case WM_COMMAND:
        {
            switch (wp)
            {
                case  ID_MENU_NEW:
                    return 0;
                case   ID_MENU_OPEN:
                    return 0;
                case  ID_MENU_SAVE:
                    return 0;
                case ID_BTN_CUSTOM_COLOR:
                {
                    CustomColorBox(hwnd);
                }
                    return 0;
                case  ID_BTN_RECTANGLE:
                {
                    currentTool = SHAPE_RECTANGLE;
                }
                    return 0;
                case  ID_BTN_ELLIPSE:
                {
                    currentTool = SHAPE_ELLIPSE;
                }
                    return 0;
                case  ID_BTN_LINE:
                {
                    currentTool = SHAPE_LINE;
                }
                return 0;
                case  ID_BTN_PENCIL:
                {
                    currentTool = TOOL_PENCIL;
                }
                return 0;
                case  ID_BTN_ERASER:
                {
                    currentTool = TOOL_ERASER;
                }
                return 0;
                case  ID_BTN_FILL:
                {
                    currentTool = TOOL_FILL;
                }
                return 0;
                case  ID_BTN_TEXT:
                {
                    currentTool = TOOL_TEXT;
                }
                return 0;
                case  ID_BTN_COLOR_PICKER:
                {
                    currentTool = TOOL_COLOR_PICKER;
                }
                return 0;
                case  ID_BTN_MAGNIFIER:
                {
                    currentTool = TOOL_MAGNIFIER;
                }
                return 0;
            }
            return 0;
        }
        case WM_DESTROY:
        {
            if (hBitmap) DeleteObject(hBitmap);
            if (hPermanentDC) DeleteDC(hPermanentDC);
            if (hPreviewBitmap) DeleteObject(hPreviewBitmap);
            if (hPreviewDC) DeleteDC(hPreviewDC);
            PostQuitMessage(0);
            
            return 0;
        }
        case WM_CREATE:
        {
            AddMenus(hwnd);
            AddControls(hwnd);
            
            GetClientRect(hwnd, &clientRect);
            canvas = {50, 150, clientRect.right - 50, clientRect.bottom - 50};

            CreateCanvasBitmap(hwnd, canvas, hPermanentDC, hBitmap, hPreviewDC, hPreviewBitmap);

            SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 20));
            SendMessage(hSlider, TBM_SETPOS, TRUE, penWidth);
            
            if (hPenColor) DeleteObject(hPenColor);
            hPenColor = CreatePen(PS_SOLID, penWidth, colorChoice);
            
            SendMessage(hSlider, TBM_SETTICFREQ, 1, 0);
            
            return 0;
        }
        case WM_SIZE:
        {
            GetClientRect(hwnd, &clientRect);
            canvas = {50, 150, clientRect.right - 50, clientRect.bottom - 50};

            if (hBitmap) DeleteObject(hBitmap);
            if (hPermanentDC) DeleteDC(hPermanentDC);
            CreateCanvasBitmap(hwnd, canvas, hPermanentDC, hBitmap, hPreviewDC, hPreviewBitmap);
            InvalidateRect(hwnd, NULL, TRUE);

            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            if(PtInRect(&canvas, pt))
            {
                fDraw = TRUE;
                ptPrevious.x = pt.x - canvas.left;
                ptPrevious.y = pt.y - canvas.top;

                pStart = pt;

                BitBlt(hPreviewDC, 0, 0, canvas.right - canvas.left, canvas.bottom - canvas.top, hPermanentDC, 0, 0, SRCCOPY);

                drawing(hwnd, canvas, pt, hPreviewDC, ptPrevious);
            }
            else
            {
                int x = GET_X_LPARAM(lp);
                int y = GET_Y_LPARAM(lp);

                for (int i = 0; i<20; i++){
                    if(PtInRect(&boxes[i].rect, {x, y})){
                        colorChoice = boxes[i].color;
                        if (hPenColor) DeleteObject(hPenColor);
                        hPenColor = CreatePen(PS_SOLID, penWidth, colorChoice);
                        break;
                    }
                }
            }
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            if(fDraw && (wp & MK_LBUTTON))
            {
                if(PtInRect(&canvas, pt)){
                    InvalidateRect(hwnd, &canvas, FALSE);
                }
                drawing(hwnd, canvas, pt, hPreviewDC, ptPrevious);
            }
            return 0;
        }
        case WM_LBUTTONUP:
        {
            fDraw = FALSE;
            BitBlt(hPermanentDC, 0, 0, canvas.right - canvas.left, canvas.bottom - canvas.top, hPreviewDC, 0, 0, SRCCOPY);

            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            BitBlt(hPreviewDC, 0, 0, canvas.right - canvas.left, canvas.bottom - canvas.top, hPermanentDC, 0, 0, SRCCOPY);
            InvalidateRect(hwnd, &canvas, FALSE);
            fDraw = FALSE;

            return 0;
        }
        case WM_HSCROLL:
        {
            if ((HWND)lp == hSlider) {
                penWidth = SendMessage(hSlider, TBM_GETPOS, 0, 0);
                if (hPenColor) DeleteObject(hPenColor);
                hPenColor = CreatePen(PS_SOLID, penWidth, colorChoice);
            }
            return 0;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // draw canvas bitmap (preview bitmap)
            BitBlt(hdc, canvas.left, canvas.top, canvas.right - canvas.left, canvas.bottom - canvas.top, hPreviewDC, 0, 0, SRCCOPY);

            //toolbar 
            RECT toolbar = {0, 0, clientRect.right, 100};
            HBRUSH toolbarBrush = CreateSolidBrush(RGB(37,41,40));
            FillRect(hdc, &toolbar, toolbarBrush);
            DeleteObject(toolbarBrush);

            //Draw lines
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(55,59,58));
            SelectObject(hdc, hPen);

            MoveToEx(hdc, 150, 6, NULL);
            LineTo(hdc, 150, 96);

            MoveToEx(hdc, 448, 6, NULL);
            LineTo(hdc, 448, 96);

            MoveToEx(hdc, 660, 6, NULL);
            LineTo(hdc, 660, 96);

            DeleteObject(hPen);

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

                HBRUSH basicColorBrush = CreateSolidBrush(basicColor[i]);
                SelectObject(hdc, basicColorBrush);
                Rectangle(hdc, box_x, y, box_x + colorBoxSize, y + colorBoxSize);
                DeleteObject(basicColorBrush);
            }

            EndPaint(hwnd, &ps);

            return 0;
        }
        case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT drawInfo = (LPDRAWITEMSTRUCT)lp;

            HDC hdc = drawInfo->hDC;
            RECT rc = drawInfo->rcItem;
            
            COLORREF bgColor = RGB(240, 240, 240); 
            if (drawInfo->itemState & ODS_SELECTED) bgColor = RGB(200, 200, 200);
            if (drawInfo->itemState & ODS_DISABLED) bgColor = RGB(210, 210, 210);

            HBRUSH bgColorBrush = CreateSolidBrush(bgColor);
            FillRect(drawInfo->hDC, &drawInfo->rcItem, bgColorBrush);
            DeleteObject(bgColorBrush);

            switch(drawInfo->CtlID){
                case ID_BTN_RECTANGLE:
                {
                    Rectangle(hdc, rc.left+5, rc.top+10, rc.right - 5, rc.bottom - 10);
                }
                break;
                case ID_BTN_ELLIPSE:
                {
                    Ellipse(hdc, rc.left+5, rc.top+10, rc.right - 5, rc.bottom - 10);
                }
                break;
                case ID_BTN_LINE:
                {
                    MoveToEx(hdc, rc.left+5, rc.top+10, NULL);
                    LineTo(hdc, rc.right - 5, rc.bottom - 10);
                }
            }
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wp, lp);
}

void CreateCanvasBitmap(HWND hwnd, RECT canvas, HDC &hPermanentDC, HBITMAP &hBitmap, HDC &hPreviewDC, HBITMAP &hPreviewBitmap) {
    HDC hdc = GetDC(hwnd);

    int width  = canvas.right - canvas.left;
    int height = canvas.bottom - canvas.top;

    //Create permanent memory DC + bitmap
    hPermanentDC = CreateCompatibleDC(hdc);
    hBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hPermanentDC, hBitmap);

    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    RECT bmpRect = {0, 0, width, height};
    FillRect(hPermanentDC, &bmpRect, whiteBrush);

    //Create preview DC + bitmap 
    hPreviewDC = CreateCompatibleDC(hdc);
    hPreviewBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hPreviewDC, hPreviewBitmap);

    FillRect(hPreviewDC, &bmpRect, whiteBrush);
   
    DeleteObject(whiteBrush);
    ReleaseDC(hwnd, hdc);
}

void AddMenus(HWND hwnd ){

    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu (hFileMenu, MF_STRING,  ID_MENU_NEW, L"New");
    AppendMenu (hFileMenu, MF_STRING,   ID_MENU_OPEN, L"Open");
    AppendMenu(hFileMenu, MF_STRING,  ID_MENU_SAVE, L"Save");

    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd){

    //tools
    CreateWindowW( L"Button", L"Pencil", WS_VISIBLE | WS_CHILD, 4, 4, 44, 44, hwnd, (HMENU)ID_BTN_PENCIL, NULL, NULL);
    CreateWindowW( L"Button", L"Fill", WS_VISIBLE | WS_CHILD, 52, 4, 44, 44, hwnd, (HMENU)ID_BTN_FILL, NULL, NULL);
    CreateWindowW( L"Button", L"Text", WS_VISIBLE | WS_CHILD, 100, 4, 44, 44, hwnd, (HMENU)ID_BTN_TEXT, NULL, NULL);
    CreateWindowW( L"Button", L"Eraser", WS_VISIBLE | WS_CHILD, 4, 52, 44, 44, hwnd, (HMENU)ID_BTN_ERASER, NULL, NULL);
    CreateWindowW( L"Button", L"Color picker", WS_VISIBLE | WS_CHILD, 52, 52, 44, 44, hwnd, (HMENU)ID_BTN_COLOR_PICKER, NULL, NULL);
    CreateWindowW( L"Button", L"Magnifier", WS_VISIBLE | WS_CHILD, 100, 52, 44, 44, hwnd, (HMENU)ID_BTN_MAGNIFIER, NULL, NULL);
    
    CreateWindowW( L"Button", L"edit color", WS_VISIBLE | WS_CHILD, 398, 4, 44, 44, hwnd, (HMENU)ID_BTN_CUSTOM_COLOR, NULL, NULL);
    
    hSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 454, 4, 200, 30, hwnd, (HMENU)ID_SLIDER, NULL, NULL);

    //custom controls
    CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | WS_BORDER |  BS_OWNERDRAW, 666, 4, 44, 44, hwnd, (HMENU)ID_BTN_RECTANGLE, NULL, NULL);
    CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | WS_BORDER |  BS_OWNERDRAW, 714, 4, 44, 44, hwnd, (HMENU)ID_BTN_ELLIPSE, NULL, NULL);
    CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | WS_BORDER |  BS_OWNERDRAW, 762, 4, 44, 44, hwnd, (HMENU)ID_BTN_LINE, NULL, NULL);
}

void CustomColorBox(HWND hwnd){
    CHOOSECOLOR cc = {};
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = customColors;
    cc.rgbResult = colorChoice;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc))
    {
        colorChoice = cc.rgbResult;
        if (hPenColor) DeleteObject(hPenColor);
        hPenColor = CreatePen(PS_SOLID, penWidth, colorChoice);
    }
}

void drawing (HWND hwnd, RECT canvas, POINT pt, HDC hdc, POINT& ptPrevious){
    int mouse_x, mouse_y, x1, y1, x2, y2;

    mouse_x = pt.x - canvas.left;
    mouse_y = pt.y - canvas.top;
    
    

    x1 = pStart.x - canvas.left;
    y1 = pStart.y - canvas.top;
    x2 = pt.x - canvas.left;
    y2 = pt.y - canvas.top;

    SelectObject(hdc, hPenColor);

    if(PtInRect(&canvas, pt))
    {
        switch (currentTool){
            case TOOL_PENCIL:
            {
                MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
                LineTo(hdc, mouse_x, mouse_y);
                InvalidateRect(hwnd, &canvas, FALSE);
            }
            break;
            case TOOL_ERASER:
            {
                HPEN hEraser = CreatePen(PS_SOLID, penWidth,RGB(255,255,255)) ;
                SelectObject(hdc, hEraser);
                MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL);
                LineTo(hdc, mouse_x, mouse_y);
                InvalidateRect(hwnd, &canvas, FALSE);
                DeleteObject(hEraser);
            }
            break;
            case SHAPE_RECTANGLE:
            {
                BitBlt(hPreviewDC, 0, 0, canvas.right - canvas.left, canvas.bottom - canvas.top, hPermanentDC, 0, 0, SRCCOPY);
                InvalidateRect(hwnd, &canvas, FALSE);

                if(GetAsyncKeyState(VK_SHIFT)&0x8000){
                    int width = x2 - x1;
                    int height = y2 - y1;

                    int side = (abs(width) < abs(height)) ? abs(width) : abs (height);

                    if(width < 0) x2 = x1 - side;
                    else x2 = x1 + side;

                    if(height < 0) y2 = y1 - side;
                    else y2 = y1 + side;
                }
                Rectangle(hdc, x1, y1, x2, y2); 
            }
            break;
            case SHAPE_ELLIPSE:
            {
                BitBlt(hPreviewDC, 0, 0, canvas.right - canvas.left, canvas.bottom - canvas.top, hPermanentDC, 0, 0, SRCCOPY);
                InvalidateRect(hwnd, &canvas, FALSE);

                if(GetAsyncKeyState(VK_SHIFT)&0x8000){
                    int width = x2 - x1;
                    int height = y2 - y1;

                    int side = (abs(width) < abs(height)) ? abs(width) : abs (height);

                    if(width < 0) x2 = x1 - side;
                    else x2 = x1 + side;

                    if(height < 0) y2 = y1 - side;
                    else y2 = y1 + side;
                }
                Ellipse(hdc, x1, y1, x2, y2);
            }
            break;
            case SHAPE_LINE:
            {
                BitBlt(hPreviewDC, 0, 0, canvas.right - canvas.left, canvas.bottom - canvas.top, hPermanentDC, 0, 0, SRCCOPY);
                InvalidateRect(hwnd, &canvas, FALSE);

                MoveToEx(hdc, x1, y1, NULL);
                LineTo(hdc, x2, y2);
            }
            break;
        }
    }
    ptPrevious.x = mouse_x;
    ptPrevious.y = mouse_y;
}

