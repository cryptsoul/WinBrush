#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <controls.h>

enum MenuAndButtonIDs {
    ID_MENU_NEW = 1,
    ID_MENU_OPEN,
    ID_MENU_SAVE,
    ID_MENU_UNDO,
    ID_MENU_REPEAT,
    ID_MENU_HISTORY,
    ID_MENU_CUT,
    ID_MENU_COPY,
    ID_MENU_PASTE,
    ID_MENU_CLEAR_SELECT,
    ID_MENU_SELECT_ALL,
    ID_MENU_HELP_TOPICS,
    ID_MENU_ABOUT,
    ID_MENU_GIF,
    ID_MENU_RANDOM,
    ID_MENU_THEMES,
    ID_MENU_ENLARGE_UI,

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