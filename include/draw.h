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

struct FontSettings {
    Gdiplus::Font* font;
    Gdiplus::Color color;
};
extern FontSettings currentFont;

extern ToolType currentTool;
extern bool newStroke;
extern bool showPreview;

void Drawing (HWND, Gdiplus::Rect, Gdiplus::Graphics*, Gdiplus::Point pStart, Gdiplus::Point pEnd);
RECT ToRECT(const Gdiplus::Rect&);
void UpdatePen();
void CustomColorBox(HWND);
void CustomFontBox(HWND);
void CustomeLine(Gdiplus::Graphics* g, int x1, int y1, int x2, int y2, int r);
