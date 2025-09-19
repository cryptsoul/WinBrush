#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <fstream>
#include <wrl/client.h>
#include "UI.h"
#include "canvas.h"

#define ARGB(a, r, g, b) Gdiplus::Color((a), (r), (g), (b))

ColorBox boxes[20];
Theme currentTheme;

Gdiplus::Rect canvas;

void Interface(Gdiplus::Graphics& g, HWND hwnd){
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    // draw canvasBitmap
    
    int border = 25; //border from canvas to window
    int center_X = (clientRect.right / 2) - (canvasBitmap->GetWidth() / 2);
    int center_Y = ((clientRect.bottom + 100) / 2) - (canvasBitmap->GetHeight() / 2); //toolbar line= 100;


    center_X = (center_X >= border ) ? center_X : border;
    center_Y = (center_Y >= 100+border) ? center_Y : 100+border;

    canvas = Gdiplus::Rect(center_X, center_Y, canvasBitmap->GetWidth(), canvasBitmap->GetHeight());

    g.DrawImage(previewBitmap, canvas);
    //toolbar 
    Gdiplus::SolidBrush toolbar(Gdiplus::Color::MakeARGB(255, 37, 41, 40));
    g.FillRectangle(&toolbar, 0, 0, clientRect.right, 100);

    //Draw lines
    Gdiplus::Pen divider(Gdiplus::Color(255, 55, 59, 58), 2);
    g.DrawLine(&divider, 150, 6, 150, 96);
    g.DrawLine(&divider, 448, 6, 448, 96);
    g.DrawLine(&divider, 660, 6, 660, 96);

    //basic colors
    Gdiplus::Color basicColor[]={
        ARGB(255, 0, 0, 0), ARGB(255, 127, 127, 127), ARGB(255, 136, 0, 21),
        ARGB(255, 237, 28, 36), ARGB(255, 255, 127, 39), ARGB(255, 255, 242, 0),
        ARGB(255, 34, 177, 76), ARGB(255, 0, 162, 232), ARGB(255, 63, 72, 204),
        ARGB(255, 163, 73, 164), ARGB(255, 255, 255, 255), ARGB(255, 195, 195, 195),
        ARGB(255, 185, 122, 87), ARGB(255, 255, 174, 201), ARGB(255, 255, 204, 14),
        ARGB(255, 239, 228, 176), ARGB(255, 181, 230, 29), ARGB(255, 153, 217, 234),
        ARGB(255, 112, 146, 190), ARGB(255, 200, 191, 231)
    };

    int x = 156, y = 4, colorBoxSize = 20, spacing = 4, box_x;
    for (int i = 0; i < 20; i++) {
        if(i<10){ 
            box_x = x + (i * (colorBoxSize + spacing ));
        }
        else {
            box_x = x + ((i-10) * (colorBoxSize + spacing));
            y = 28;
        }

        boxes[i].rect = Gdiplus::Rect(box_x, y, colorBoxSize, colorBoxSize);
        boxes[i].color = basicColor[i];

        Gdiplus::SolidBrush basicColorBrush(basicColor[i]);
        g.FillRectangle(&basicColorBrush, boxes[i].rect);
    }
}

std::wstring GetExeDir()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);

    std::wstring fullPath(path);
    size_t pos = fullPath.find_last_of(L"\\/");
    return fullPath.substr(0, pos);
}

void LoadThemes()
{
    std::wstring baseDir = GetExeDir() + L"\\..\\themes\\Classic-Light\\";

    currentTheme.pencilIcon = Gdiplus::Bitmap::FromFile((baseDir + L"pen.png").c_str());
    currentTheme.eraserIcon = Gdiplus::Bitmap::FromFile((baseDir +L"eraser.png").c_str());
    currentTheme.fillIcon = Gdiplus::Bitmap::FromFile((baseDir +L"fill.png").c_str());
    currentTheme.textIcon = Gdiplus::Bitmap::FromFile((baseDir +L"text.png").c_str());
    currentTheme.colorPickerIcon = Gdiplus::Bitmap::FromFile((baseDir +L"color-picker.png").c_str());
    currentTheme.magnifierIcon = Gdiplus::Bitmap::FromFile((baseDir +L"loupe.png").c_str());
    currentTheme.customColorIcon = Gdiplus::Bitmap::FromFile((baseDir +L"color-wheel.png").c_str());
}