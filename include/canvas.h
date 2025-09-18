#pragma once
#include <Windows.h>
#include <gdiplus.h>  

extern Gdiplus::Bitmap* canvasBitmap;
extern Gdiplus::Graphics* canvasGraphics;
extern Gdiplus::Bitmap* previewBitmap;
extern Gdiplus::Graphics* previewGraphics;

void CreateCanvasBitmap(HWND);