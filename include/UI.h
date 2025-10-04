#pragma once
#include <windows.h>
#include <Gdiplus.h>

struct Theme{
    Gdiplus::Bitmap* pencilIcon;
    Gdiplus::Bitmap* eraserIcon;
    Gdiplus::Bitmap* fillIcon;
    Gdiplus::Bitmap* textIcon;
    Gdiplus::Bitmap* colorPickerIcon;
    Gdiplus::Bitmap* magnifierIcon;
    Gdiplus::Bitmap* customColorIcon;
};

struct ColorBox{
    Gdiplus::Rect rect;
    Gdiplus::Color color;
};

extern Theme currentTheme;
extern ColorBox boxes[20];
extern Gdiplus::Rect canvas;

void Interface(Gdiplus::Graphics&, HWND);
void LoadThemes();