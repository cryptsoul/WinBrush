#pragma once
#include <Windows.h>
#include <gdiplus.h>  

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
};

struct DrawingState{
    Gdiplus::Pen* pen; 
    Gdiplus::Color penColor;
    int penWidth;
};
extern DrawingState gDrawingState;

extern ToolType currentTool;
extern bool newStroke;

void Drawing (HWND, Gdiplus::Rect, Gdiplus::Graphics*, Gdiplus::Point pStart, Gdiplus::Point pEnd);
RECT ToRECT(const Gdiplus::Rect&);
void UpdatePen();
void CustomColorBox(HWND);
