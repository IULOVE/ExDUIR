#include "help_ex.h"

#pragma region rollmenu obj value
// 属性_卷帘菜单_分组高度 = 40
#define RML_GH   0  
// 属性_卷帘菜单_项目高度 = 40
#define RML_IH   1  
// 属性_卷帘菜单_分组标题颜色 = 16777215
#define RML_GTC  2  
// 属性_卷帘菜单_鼠标点燃背景色 
#define RML_MLBC 3  
// 属性_卷帘菜单_子项选中背景色
#define RML_ISBC 4  
// 属性_卷帘菜单_子项选中标题色
#define RML_ISTC 5  
// 属性_卷帘菜单_当前选中项目
#define RML_CSI 6   
// 属性_卷帘菜单_当前选中分组
#define RML_CSG 7  
// 属性_卷帘菜单_当前悬浮分组,仅当鼠标悬浮分组,不包括子项目,分组索引从1开始, 0未悬浮分组
#define RML_CFG 8  
// 属性_卷帘菜单_当前点燃项目
#define RML_CLI 9  
// 属性_卷帘菜单_当前点燃分组,选中分组或其子项目
#define RML_CLG 10  
// 属性_卷帘菜单_项目数据  =>  ROLLMENU_DATA
#define RML_ARR 11  
#pragma endregion rollmenu obj value

void _rollmenu_register();
LRESULT CALLBACK _rollmenu_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _rollmenu_ondelmember(array_s* pArray, INT nIndex, void* pvItem, INT nType);
void _rollmenu_ondelitemmember(array_s* pArray, INT nIndex, void* pvItem, INT nType);
void _rollmenu_paint(HEXOBJ hObj);
