#include <windows.h>
#include <Gdiplus.h>
#include "File.h"
#include "canvas.h"
#include "draw.h"
#include <commdlg.h>
#include <commctrl.h>
#include <wrl/client.h>
#include <fstream>
#include <shobjidl.h>

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;
    UINT size = 0;

    Gdiplus::GetImageEncodersSize(&num, &size);
    if(size == 0) return -1;

    Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j=0; j<num; ++j){
        if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0){
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }

    free(pImageCodecInfo);
    return -1;
}

void OpenFile(HWND hwnd, Gdiplus::Rect canvas, Gdiplus::Graphics* g, int width, int height) {
    Microsoft::WRL::ComPtr<IFileOpenDialog> pFileOpen;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpen));
    
    if (SUCCEEDED(hr)) {
        COMDLG_FILTERSPEC rgSpec[] = {
            { L"Image Files", L"*.png;*.jpg;*.bmp;*.gif" }
        };
        pFileOpen->SetFileTypes(1, rgSpec);

        hr = pFileOpen->Show(hwnd);

        if (SUCCEEDED(hr)) {
            Microsoft::WRL::ComPtr<IShellItem> pItem;
            hr = pFileOpen->GetResult(&pItem);

            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                if (SUCCEEDED(hr)) {
                    Gdiplus::Bitmap* loadedImage = new Gdiplus::Bitmap(pszFilePath);
                    
                    if (loadedImage->GetLastStatus() == Gdiplus::Ok) {
                        HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
                        g->DrawImage(loadedImage, 0, 0, width, height);
                    } else {
                        MessageBox(hwnd, L"Failed to load image", L"Error", MB_OK);
                    }
                    
                    RECT rect = ToRECT(canvas);
                    InvalidateRect(hwnd, &rect, FALSE);
                    CoTaskMemFree(pszFilePath);
                }
            }
        }
    }
    
}

void SaveFile(HWND hwnd, Gdiplus::Bitmap* canvasBitmap)
{
    Microsoft::WRL::ComPtr<IFileSaveDialog> pFileSave;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileSave));
    if (SUCCEEDED(hr))
    {
        COMDLG_FILTERSPEC rgSpec[]={
            { L"PNG Files", L"*.png" },
            { L"JPEG Files", L"*.jpg" },
            { L"Bitmap Files", L"*.bmp" }
        };
        pFileSave->SetFileTypes(3, rgSpec);
        pFileSave->SetDefaultExtension(L"png");

        hr = pFileSave->Show(hwnd);

        if(SUCCEEDED(hr)){
            Microsoft::WRL::ComPtr<IShellItem> pItem;
            hr = pFileSave->GetResult(&pItem);

            if(SUCCEEDED(hr)){
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                if(SUCCEEDED(hr)){
                    CLSID encoderClsid;
                    GetEncoderClsid(L"image/png", &encoderClsid);
                    canvasBitmap->Save(pszFilePath, &encoderClsid, NULL);

                    MessageBox((hwnd), L"File saved!", L"Success", MB_OK);
                    CoTaskMemFree(pszFilePath);
                }
            }
        }
    }
}