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
#undef max

ToolType currentTool = TOOL_PENCIL;
DrawingState gDrawingState = {nullptr, Gdiplus::Color(255, 0, 0, 0), 1};
bool newStroke = true;
bool showPreview = true;
static COLORREF customColors[16];

void Drawing (HWND hwnd, Gdiplus::Rect canvas, Gdiplus::Graphics* g, Gdiplus::Point pStart, Gdiplus::Point pEnd)
{    
    static Gdiplus::Point pPrevious;
    Gdiplus::Point canvasStart = {pStart.X - canvas.X, pStart.Y - canvas.Y};
    Gdiplus::Point canvasEnd = {pEnd.X - canvas.X, pEnd.Y - canvas.Y};

    int width = abs(canvasEnd.X - canvasStart.X);
    int height = abs(canvasEnd.Y - canvasStart.Y);
    
    int side = std::min(abs(width), abs(height));

    Gdiplus::Point upLeft(std::min(canvasStart.X, canvasEnd.X), std::min(canvasStart.Y, canvasEnd.Y));

    int squareWidth = (canvasEnd.X >= canvasStart.X) ? side : -side;
    int squareHeight = (canvasEnd.Y >= canvasStart.Y) ? side : -side;
    
    Gdiplus::Point squareTopLeft;
    squareTopLeft.X = (canvasEnd.X >= canvasStart.X) ? canvasStart.X : canvasStart.X - side;
    squareTopLeft.Y = (canvasEnd.Y >= canvasStart.Y) ? canvasStart.Y : canvasStart.Y - side;


    if (newStroke) {
        pPrevious = canvasStart;
        newStroke  = false;
    }

    int circleRadius = gDrawingState.penWidth / 2;


    int addX = canvasEnd.X - pPrevious.X;
    int addY = canvasEnd.Y - pPrevious.Y;
    
    if(canvas.Contains(pEnd))
    {
        switch (currentTool){
            case TOOL_PENCIL:
            {
                if (!(showPreview))
                {
                    CustomeLine(g, pPrevious.X, pPrevious.Y, canvasEnd.X, canvasEnd.Y, circleRadius);
                }
                else g->DrawEllipse(gDrawingState.pen, pPrevious.X - circleRadius, pPrevious.Y - circleRadius, gDrawingState.penWidth, gDrawingState.penWidth);
            }
            break;
            case TOOL_ERASER:
            {
                gDrawingState.penColor = Gdiplus::Color(255, 250, 255, 255);
                UpdatePen();
 
                if (!(showPreview))
                {
                    CustomeLine(g, pPrevious.X , pPrevious.Y, canvasEnd.X, canvasEnd.Y, circleRadius);
                }
                else g->DrawEllipse(gDrawingState.pen, pPrevious.X - circleRadius, pPrevious.Y - circleRadius,   gDrawingState.penWidth, gDrawingState.penWidth);
            }
            break;
            case TOOL_COLOR_PICKER:
            {
                previewGraphics->DrawImage(canvasBitmap, 0, 0, canvas.Width, canvas.Height);
                RECT rect = ToRECT(canvas);
                InvalidateRect(hwnd, &rect, FALSE);

                Gdiplus::Color c;
                canvasBitmap->GetPixel(canvasEnd.X, canvasEnd.Y, &c);
                Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0), 2);
                Gdiplus::SolidBrush brush(c);

                Gdiplus::Rect r(canvasEnd.X + 10, canvasEnd.Y - 25, 25, 25);    
                            
                g->FillRectangle(&brush, r);
                g->DrawRectangle(&pen, r);
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
                g->DrawLine(gDrawingState.pen, canvasStart, canvasEnd);
            }
            break;
        }
    }
    pPrevious = canvasEnd;
    if(!showPreview){
        RECT newCanvas = ToRECT(canvas);
        InvalidateRect(hwnd, &newCanvas, FALSE);
    } 
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

void CustomeLine(Gdiplus::Graphics* g, int x1, int y1, int x2, int y2, int r){
    if (x1 == x2) {
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            g->DrawEllipse(gDrawingState.pen, x1 - r, y - r, gDrawingState.penWidth, gDrawingState.penWidth);
        }
    } else if (y1 == y2) {
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            g->DrawEllipse(gDrawingState.pen, x - r, y1 - r, gDrawingState.penWidth, gDrawingState.penWidth);
        }
    } else {
        float m = (float)(y2 - y1) / (x2 - x1);
        float b = y1 - m * x1;
        
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            int y = (int)(m * x + b);
            g->DrawEllipse(gDrawingState.pen, x - r, y - r, gDrawingState.penWidth, gDrawingState.penWidth);
        }
    }
}   