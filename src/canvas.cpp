#include <windows.h>
#include <gdiplus.h>
#include <canvas.h>

Gdiplus::Bitmap* canvasBitmap = nullptr;
Gdiplus::Graphics* canvasGraphics = nullptr;
Gdiplus::Bitmap* previewBitmap = nullptr;
Gdiplus::Graphics* previewGraphics = nullptr;

void CreateCanvasBitmap(HWND hwnd) {

    //Create permanent memory DC + bitmap
    if(canvasBitmap) {
        delete canvasBitmap;
        canvasBitmap = nullptr;
    }
    if(canvasGraphics) {
        delete canvasGraphics;
        canvasGraphics = nullptr;
    }
    
    canvasBitmap = new Gdiplus::Bitmap(840, 620, PixelFormat32bppARGB);
    canvasGraphics = new Gdiplus::Graphics(canvasBitmap); 
    canvasGraphics->Clear(Gdiplus::Color(255, 255, 255, 255));

    //Create preview DC + bitmap
    if(previewBitmap) {
        delete previewBitmap;
        previewBitmap = nullptr;
    }
    if(previewGraphics) {
        delete previewGraphics;
        previewGraphics = nullptr;
    }

    previewBitmap = new Gdiplus::Bitmap(canvasBitmap->GetWidth(), canvasBitmap->GetHeight(), PixelFormat32bppARGB);
    previewGraphics = new Gdiplus::Graphics (previewBitmap); 
    previewGraphics->Clear(Gdiplus::Color(255, 255, 255, 255));
}