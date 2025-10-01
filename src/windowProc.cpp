#include <windows.h>
#include <commctrl.h>

#include "windowProc.h"
#include "canvas.h"
#include "UI.h"
#include "controls.h"
#include "draw.h"
#include "File.h"
#include <windowsx.h>

static Gdiplus::Graphics* graphics = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp){
    
    static BOOL fDraw = FALSE;
    Gdiplus::Point pMouse = {LOWORD(lp), HIWORD(lp)};
    static Gdiplus::Point pStart;
    static Gdiplus::Point pEnd;

    switch (uMsg)
    {
        case WM_COMMAND:
        {
            switch (wp)
            {
                case  ID_MENU_NEW:
                {
                    CreateCanvasBitmap(hwnd);
                    InvalidateRect(hwnd, FALSE, FALSE);
                }
                    return 0;
                case   ID_MENU_OPEN:
                {
                    OpenFile(hwnd, canvas, previewGraphics, canvas.Width, canvas.Height);
                }
                    return 0;
                case  ID_MENU_SAVE:
                {
                    SaveFile(hwnd, canvasBitmap);
                }
                    return 0;
                case ID_BTN_CUSTOM_COLOR:
                {
                   CustomColorBox(hwnd);
                   UpdatePen();
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
            if (canvasBitmap){
                delete(canvasBitmap);
                canvasBitmap = nullptr;
            }
            if (canvasGraphics){
                delete(canvasGraphics);
                canvasGraphics = nullptr;
            }
            if (previewBitmap){
                delete(previewBitmap);
                previewBitmap = nullptr;
            }
            if (previewGraphics){
                delete(previewGraphics);
                previewGraphics = nullptr;
            }
            PostQuitMessage(0);
            
            return 0;
        }
        case WM_CREATE:
        {
            AddMenus(hwnd);
            AddControls(hwnd);
            LoadThemes();

            CreateCanvasBitmap(hwnd);

            SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 20));
            SendMessage(hSlider, TBM_SETTICFREQ, 1, 0);
            SendMessage(hSlider, TBM_SETPOS, TRUE, 1);
            
            UpdatePen();
            return 0;
        }
        case WM_SIZE:
        {
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            if(canvas.Contains(pMouse))
            {
                fDraw = TRUE;
                pStart = pMouse;
                pEnd = pMouse;
                newStroke = true;
                previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
                Drawing(hwnd, canvas, previewGraphics, pStart, pEnd);

                RECT rect = ToRECT(canvas);
                InvalidateRect(hwnd, &rect, FALSE);
            }
            else
            {
                int x = GET_X_LPARAM(lp);
                int y = GET_Y_LPARAM(lp);
                Gdiplus::Point clickPoint(x, y);
                for (int i = 0; i<20; i++){
                    if(boxes[i].rect.Contains(clickPoint)){
                        gDrawingState.penColor = boxes[i].color;
                        UpdatePen();
                        break;
                    }
                }
            }
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            pMouse = {LOWORD(lp), HIWORD(lp)};
            pEnd = pMouse;

            if(fDraw && (wp & MK_LBUTTON))
            {
                if (showPreview){
                    previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
                    RECT rect = ToRECT(canvas);
                    InvalidateRect(hwnd, &rect, FALSE);
                    showPreview = false;
                }
                if(canvas.Contains(pMouse))
                {
                    Drawing(hwnd, canvas, previewGraphics, pStart, pEnd);
                }
                else if (currentTool == TOOL_PENCIL || currentTool == TOOL_ERASER || currentTool == TOOL_COLOR_PICKER)
                {
                    newStroke = true;
                    pStart = pMouse;
                }
            }
            else if(canvas.Contains(pMouse) && (currentTool == TOOL_PENCIL || currentTool == TOOL_ERASER || currentTool == TOOL_COLOR_PICKER))
            { 
                if (showPreview){
                    previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
                    RECT rect = ToRECT(canvas);
                    InvalidateRect(hwnd, &rect, FALSE);
                    Drawing(hwnd, canvas, previewGraphics, pStart, pEnd);
                }
            }
            else {
                previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
                RECT rect = ToRECT(canvas);
                InvalidateRect(hwnd, &rect, FALSE);
            }
            return 0;
        }
        case WM_LBUTTONUP:
        {
            fDraw = FALSE;
            showPreview = true;
            if(currentTool != TOOL_COLOR_PICKER){
                canvasGraphics->DrawImage(previewBitmap, 0, 0, canvas.Width, canvas.Height);
            }
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
            RECT rect = ToRECT(canvas);
            InvalidateRect(hwnd, &rect, FALSE);
            fDraw = FALSE;

            return 0;
        }
        case WM_HSCROLL:
        {
            if ((HWND)lp == hSlider) {
                gDrawingState.penWidth = SendMessage(hSlider, TBM_GETPOS, 0, 0);
                UpdatePen();
            }
            return 0;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            Gdiplus::Graphics g(hdc);
            Interface(g, hwnd);

            EndPaint(hwnd, &ps);

            return 0; 
        } 
        case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT drawInfo = (LPDRAWITEMSTRUCT)lp;

            HDC hdc = drawInfo->hDC;
            RECT rc = drawInfo->rcItem;
            Gdiplus::Graphics graphics(hdc);


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
                break;
                case ID_BTN_PENCIL:
                {
                    graphics.DrawImage(currentTheme.pencilIcon, 5, 5, rc.right - rc.left - 10, rc.bottom - rc.top - 10);
                }
                break;
                case ID_BTN_ERASER:
                {
                    graphics.DrawImage(currentTheme.eraserIcon, 5, 5, rc.right - rc.left - 10, rc.bottom - rc.top - 10);
                }
                break;
                case ID_BTN_FILL:
                {
                    graphics.DrawImage(currentTheme.fillIcon, 5, 5, rc.right - rc.left - 10, rc.bottom - rc.top - 10);
                }
                break;
                case ID_BTN_TEXT:
                {
                    graphics.DrawImage(currentTheme.textIcon, 5, 5, rc.right - rc.left - 10, rc.bottom - rc.top - 10);
                }
                break;
                case ID_BTN_COLOR_PICKER:
                {
                    graphics.DrawImage(currentTheme.colorPickerIcon, 5, 5, rc.right - rc.left - 10, rc.bottom - rc.top - 10);
                }
                break;
                case ID_BTN_MAGNIFIER:
                {
                    graphics.DrawImage(currentTheme.magnifierIcon, 5, 5, rc.right - rc.left - 10, rc.bottom - rc.top - 10);
                }
                break;
                case ID_BTN_CUSTOM_COLOR:
                {
                    graphics.DrawImage(currentTheme.customColorIcon, 5, 5, rc.right - rc.left - 10, rc.bottom - rc.top - 10);
                }
                break;
            }
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wp, lp);
}