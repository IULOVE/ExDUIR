#pragma once
#include "help_ex.h"
#pragma comment (lib, "Msimg32.lib")


// 调色板属性_鼠标开始位置X
#define PTL_BEGINX 0
// 调色板属性_鼠标开始位置Y
#define PTL_BEGINY 1
// 调色板属性_是否按下
#define PTL_DOWN 2
// 调色板属性_画刷句柄
#define PTL_PEN 3
// 调色板属性_图片句柄
#define PTL_IMAGE 4

void _palette_register();
LRESULT CALLBACK _palette_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _palette_genimage(HEXOBJ hObj);
void _palette_drawgradientrect(HDC hdc, INT left, INT top, INT right, INT bottom, EXARGB startColor, EXARGB endColor, BOOL horizontalGradient);
void _palette_paint(HEXOBJ hObj);