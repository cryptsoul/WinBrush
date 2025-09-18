#include <windows.h>
#include <Gdiplus.h>
#include "draw.h"
#include "canvas.h"


#include <cstdio>
#include <format>
#include <initializer_list>
#include <iostream>
#include <string>

#include <algorithm>
#undef min

ToolType currentTool = TOOL_PENCIL;
DrawingState gDrawingState = {nullptr, Gdiplus::Color(255, 0, 0, 0), 1};
bool newStroke = true;
static bool check = true;
static COLORREF customColors[16];

void Drawing (HWND hwnd, Gdiplus::Rect canvas, Gdiplus::Graphics* g, Gdiplus::Point pStart, Gdiplus::Point pEnd)
{    
    static Gdiplus::Point pPrevious;
    Gdiplus::Point one = {pStart.X - canvas.X, pStart.Y - canvas.Y};
    Gdiplus::Point two = {pEnd.X - canvas.X, pEnd.Y - canvas.Y};

    int width = abs(two.X - one.X);
    int height = abs(two.Y - one.Y);
    
    int side = std::min(abs(width), abs(height));

    Gdiplus::Point upLeft(std::min(one.X, two.X), std::min(one.Y, two.Y));

    int squareWidth = (two.X >= one.X) ? side : -side;
    int squareHeight = (two.Y >= one.Y) ? side : -side;
    
    Gdiplus::Point squareTopLeft;
    squareTopLeft.X = (two.X >= one.X) ? one.X : one.X - side;
    squareTopLeft.Y = (two.Y >= one.Y) ? one.Y : one.Y - side;


    if (newStroke) {
        pPrevious = one;
        newStroke  = false;
    }

    if(canvas.Contains(pEnd))
    {
        switch (currentTool){
            case TOOL_PENCIL:
            {
                g->DrawLine(gDrawingState.pen, pPrevious, two);
            }
            break;
            case TOOL_ERASER:
            {
                
                Gdiplus::Pen eraser(Gdiplus::Color(255, 250, 255, 255), gDrawingState.penWidth);
                g->DrawLine(&eraser, pPrevious, two);
            }
            break;
            case SHAPE_RECTANGLE:
            {

                g->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
                if(GetAsyncKeyState(VK_SHIFT)&0x8000){
                    g->DrawRectangle(gDrawingState.pen, squareTopLeft.X, squareTopLeft.Y, abs(side), abs(side));
                }
                else g->DrawRectangle(gDrawingState.pen, upLeft.X, upLeft.Y, width, height);
            }
            break;
            case SHAPE_ELLIPSE:
            {
                previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);

                if(GetAsyncKeyState(VK_SHIFT)&0x8000){
                    int side = (abs(width) < abs(height)) ? abs(width) : abs (height);
                    g->DrawEllipse(gDrawingState.pen, squareTopLeft.X, squareTopLeft.Y, abs(side), abs(side));
                }
                else g->DrawEllipse(gDrawingState.pen, upLeft.X, upLeft.Y, width, height);
            }
            break;
            case SHAPE_LINE:
            {
                previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
                g->DrawLine(gDrawingState.pen, one, two);
            }
            break;
        }
    }
    pPrevious = two;

    RECT newCanvas = ToRECT(canvas);
    InvalidateRect(hwnd, &newCanvas, FALSE);
}

RECT ToRECT(const Gdiplus::Rect& r){
    RECT rc;
    rc.left = r.X;
    rc.top = r.Y;
    rc.right = r.X + r.Width;
    rc.bottom = r.Y + r.Height;
    return rc;
}

void CustomColorBox(HWND hwnd)
{
    CHOOSECOLOR cc = {};
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = customColors;
    cc.rgbResult = RGB(0,0,0);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc))
    {
        COLORREF colorChoice = cc.rgbResult;
        int r = GetRValue(colorChoice);
        int g = GetGValue(colorChoice);
        int b = GetBValue(colorChoice);

        gDrawingState.penColor = Gdiplus::Color(255, r, g, b);
    }
}

void UpdatePen(){
    if (gDrawingState.pen) delete gDrawingState.pen;
    gDrawingState.pen = new Gdiplus::Pen(gDrawingState.penColor, gDrawingState.penWidth);
}