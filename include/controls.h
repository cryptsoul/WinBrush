#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <controls.h>

enum MenuAndButtonIDs {
    ID_MENU_NEW = 1,
    ID_MENU_OPEN,
    ID_MENU_SAVE,
    ID_BTN_CUSTOM_COLOR,
    ID_SLIDER,
    ID_BTN_RECTANGLE,
    ID_BTN_ELLIPSE,
    ID_BTN_LINE,
    ID_BTN_PENCIL,
    ID_BTN_ERASER,
    ID_BTN_FILL,
    ID_BTN_TEXT,
    ID_BTN_COLOR_PICKER,
    ID_BTN_MAGNIFIER
};

extern HWND hSlider;

void AddMenus(HWND);
void AddControls(HWND);