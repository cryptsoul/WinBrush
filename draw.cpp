#ifndef UNICODE
#define UNICODE
#endif
#define OPEN_FILE_BUTTON 1

#include <windows.h>
#include<stdio.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);

HWND hMainWindow, hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Paint windows class";

    WNDCLASSW wc = {};
    wc.hCursor = LoadCursorW(NULL, L"IDC_ARROW");
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClassW(&wc)) return -1;
    
    hMainWindow = CreateWindowExW(0, CLASS_NAME, L"Paint", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, NULL, NULL );

    ShowWindow(hMainWindow, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void display_file(wchar_t* path){
    FILE *file = _wfopen (path, L"rb");
    if(!file) return;

    fseek(file, 0, SEEK_END);
    int _size = ftell(file);
    rewind(file);

    char* buffer = new char[_size+1];
    fread(buffer, 1, _size, file);
    buffer[_size] = '\0';

    int wlen = MultiByteToWideChar(CP_ACP, 0, buffer, -1, NULL, 0);
    wchar_t* wbuffer = new wchar_t[wlen];
    MultiByteToWideChar(CP_ACP, 0, buffer, -1, wbuffer, wlen);
    
    SetWindowText(hEdit, wbuffer);
}

void openFile(HWND hwnd){
    OPENFILENAME ofn;

    wchar_t file_name[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile [0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = L"All files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetOpenFileNameW(&ofn);

    display_file(ofn.lpstrFile);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp){

    switch (umsg)
    {
        case WM_COMMAND:
        
        switch (wp)
        {
            case OPEN_FILE_BUTTON:
                openFile(hwnd);
            return 0;
            
        }
        break;
        case WM_CREATE:
            AddControls(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, umsg, wp, lp);
}

void AddControls(HWND hwnd){
    CreateWindowW(L"Button", L"Open File ", WS_VISIBLE | WS_CHILD, 10, 10, 150, 35, hwnd, (HMENU)OPEN_FILE_BUTTON, NULL, NULL);
    hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL, 10, 50, 400, 300, hwnd, NULL, NULL, NULL);
}