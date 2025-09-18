#pragma once
#include <windows.h>
#include <Gdiplus.h>

void OpenFile(HWND, Gdiplus::Rect, Gdiplus::Graphics*, int, int);
void SaveFile(HWND, Gdiplus::Bitmap*);
int GetEncoderClsid(const WCHAR*, CLSID*);