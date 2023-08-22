#include "test_obj.h"
#include "resource.h"

HEXDUI m_hExDuiButton;

LRESULT CALLBACK OnButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 201) //通过组件ID判断按钮
	{
		Ex_ObjEnable(hObj, FALSE); //禁用自身
		Ex_ObjSetPadding(hObj, 0, 20, 5, 5, 5, TRUE);

	}
	else if (nID == 202)
	{
		Ex_ObjEnable(Ex_ObjGetFromID(m_hExDuiButton, 201), TRUE); //通过组件ID取按钮句柄，解除按钮1禁用
	}
	else if (nID == 203)
	{
		Ex_ObjSetText(hObj, L"自身文本被改动", TRUE); //改动自身文本
	}
	else if (nID == 204)
	{
		auto text_length = Ex_ObjGetTextLength(Ex_ObjGetFromID(m_hExDuiButton, 201)); //取按钮1文本长度
		std::wstring str;
		str.resize(text_length * 2 + 2);
		Ex_ObjGetText(Ex_ObjGetFromID(m_hExDuiButton, 201), str.c_str(), text_length * 2 + 2);
		Ex_ObjSetText(hObj, (L"按钮1文本:" + str).c_str(), TRUE);
	}
	else if (nID == 205)
	{
		if (nCode == NM_CUSTOMDRAW)
		{
			EX_PAINTSTRUCT ps{ 0 };
			RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
			EXARGB crBkg = 0;
			if ((ps.dwState & STATE_DOWN) != 0)
			{
				crBkg = ExRGB2ARGB(255, 51);
			}
			else if ((ps.dwState & STATE_HOVER) != 0)
			{
				crBkg = ExRGB2ARGB(16754943, 51);
			}
			else
			{
				crBkg = ExRGB2ARGB(16777215, 51);
			}
			HEXBRUSH hBrush = _brush_create(crBkg);
			if (hBrush)
			{
				_canvas_fillrect(ps.hCanvas, hBrush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
				_brush_destroy(hBrush);
			}
		}
	}
	else if (nID == 206)
	{
		if (wParam != 0)
		{
			Ex_MessageBox(hObj, L"开启", L"取开关状态", MB_USERICON, EMBF_CENTEWINDOW);
		}
		else
		{
			Ex_MessageBox(hObj, L"关闭", L"取开关状态", MB_ICONWARNING, EMBF_CENTEWINDOW);
		}
	}
	return 0;
}

LRESULT CALLBACK OnButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND) //组件WM_ERASEBKGND消息wParam是hCanvas,lParam是EX_PAINTSTRUCT
	{
		EX_PAINTSTRUCT ps{ 0 };
		RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
		EXARGB crBkg = 0;
		if ((ps.dwState & STATE_DOWN) != 0)
		{
			crBkg = ExRGB2ARGB(255, 51);
		}
		else if ((ps.dwState & STATE_HOVER) != 0)
		{
			crBkg = ExRGB2ARGB(16754943, 51);
		}
		else
		{
			crBkg = ExRGB2ARGB(16777215, 51);
		}
		HEXBRUSH hBrush = _brush_create(crBkg);
		if (hBrush)
		{
			_canvas_fillrect(ps.hCanvas, hBrush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
			_brush_destroy(hBrush);
		}
		*lpResult = 1;
		return 1;
	}
	return 0;
}

void test_button(HWND hWnd)
{
	HWND hWnd_button = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试按钮开关", 0, 0, 300, 200, 0, 0);
	m_hExDuiButton = Ex_DUIBindWindowEx(hWnd_button, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(m_hExDuiButton, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	std::vector<HEXOBJ> buttons;
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"禁用自身", -1, 10, 30, 120, 30, m_hExDuiButton, 201, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"解除按钮1禁用", -1, 10, 70, 120, 30, m_hExDuiButton, 202, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"改动自身文本", -1, 10, 110, 120, 30, m_hExDuiButton, 203, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"取按钮1文本", -1, 10, 150, 120, 30, m_hExDuiButton, 204, DT_VCENTER | DT_CENTER, 0, 0, NULL));

	for (auto button : buttons)
	{
		Ex_ObjHandleEvent(button, NM_CLICK, OnButtonEvent);
	}

	HEXOBJ hobj_customdrawbutton = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_CUSTOMDRAW | EOS_EX_COMPOSITED, L"button", L"重画按钮1", -1, 150, 30, 120, 30, m_hExDuiButton, 205, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(hobj_customdrawbutton, NM_CUSTOMDRAW, OnButtonEvent); //第一种重画背景方式,全部自带组件都可以采用这样的方式重画,注意带上扩展风格

	Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_CUSTOMDRAW | EOS_EX_COMPOSITED, L"button", L"重画按钮2", -1, 150, 70, 120, 30, m_hExDuiButton, 0, DT_VCENTER | DT_CENTER, 0, 0, OnButtonMsgProc); //第二种重画背景方式,全部自带组件都可以采用这样的方式重画,注意带上扩展风格

	HEXOBJ hObj_switch = Ex_ObjCreate(L"Switch", L"已开启|已关闭", -1, 150, 110, 80, 30, m_hExDuiButton);

	HEXOBJ hObj_switch2 = Ex_ObjCreateEx(-1, L"Switch", 0, -1, 150, 150, 60, 30, m_hExDuiButton, 206, -1, 0, 0, 0);
	Ex_ObjSendMessage(hObj_switch2, BM_SETCHECK, 1, 0); // 设置选中状态
	Ex_ObjHandleEvent(hObj_switch2, NM_CHECK, OnButtonEvent);
	EX_OBJ_PROPS switchprops = { 0 };
	switchprops.crBkgNormal = ExARGB(255, 255, 255, 100);
	switchprops.crBkgDownOrChecked = ExARGB(200, 50, 100, 0);
	switchprops.crBorderNormal = ExARGB(255, 255, 255, 255);
	switchprops.radius = 15;
	switchprops.strokeWidth = 1;
	Ex_ObjSendMessage(hObj_switch2, WM_EX_PROPS, 0, (LPARAM)&switchprops);

	Ex_DUIShowWindow(m_hExDuiButton, SW_SHOWNORMAL, 0, 0, 0);
}

void test_label(HWND hWnd)
{
	HWND hWnd_label = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试基础组件", 0, 0, 400, 400, 0, 0);
	HEXDUI hExDui_label = Ex_DUIBindWindowEx(hWnd_label, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_label, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj_label = Ex_ObjCreateEx(-1, L"static", NULL, EOS_BORDER | EOS_VISIBLE, 10, 30, 180, 150, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/Loading.gif", &imgdata);
	Ex_ObjSetBackgroundImage(hObj_label, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);

	Ex_ObjSetRadius(hObj_label, 10, 10, 15, 10, TRUE);



	EX_BACKGROUNDIMAGEINFO bkg{ 0 };
	Ex_ObjGetBackgroundImage(hObj_label, &bkg);
	OUTPUTW(L"背景信息:", bkg.x, bkg.y, bkg.dwAlpha, bkg.dwRepeat, bkg.hImage, bkg.curFrame, bkg.maxFrame);
	RECT rect;
	Ex_ObjGetRect(hObj_label, &rect);
	OUTPUTW(L"标签矩形:", rect.right, rect.bottom);

	HEXOBJ hObj_label2 = Ex_ObjCreateEx(-1, L"static", NULL, EOS_BORDER | EOS_VISIBLE, 200, 30, 180, 150, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ReadFile(L"res/409.dds", &imgdata);
	Ex_ObjSetBackgroundImage(hObj_label2, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);

	HEXOBJ hObj_label4 = Ex_ObjCreateEx(-1, L"static", L"基础组件可以填充动画,支持PNG,GIF,JPG,BMP,DDS,ICO,标签可以自动换行", -1, 200, 200, 180, 90, hExDui_label, 0, DT_WORDBREAK, 0, 0, NULL);
	Ex_ObjSetFontFromFamily(hObj_label4, L"宋体", 14, EFS_BOLD, FALSE);
	Ex_ObjSetColor(hObj_label4, COLOR_EX_TEXT_NORMAL, ExARGB(133, 33, 53, 255), TRUE);

	Ex_DUIShowWindow(hExDui_label, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnCheckButtonCheckedEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam != 0)
	{
		BOOL fChecked = TRUE; //弹出的信息框里面复选框是选中状态
		Ex_MessageBoxEx(hObj, L"选中啦", L"取CheckButton状态", MB_USERICON, L"不再提示", &fChecked, 10, EMBF_CENTEWINDOW, 0);
	}
	else
	{
		Ex_MessageBox(hObj, L"取消选中啦", L"取CheckButton状态", MB_ICONWARNING, EMBF_CENTEWINDOW);
	}
	return 0;
}

void test_checkbutton(HWND hWnd)
{
	HWND hWnd_checkbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试单选框复选框", 0, 0, 300, 250, 0, 0);
	HEXDUI hExDui_checkbutton = Ex_DUIBindWindowEx(hWnd_checkbutton, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_checkbutton, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	Ex_ObjCreateEx(-1, L"checkbutton", L"复选框", -1, 10, 30, 60, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ObjCreateEx(-1, L"radiobutton", L"单选框1", -1, 10, 60, 80, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ObjCreateEx(-1, L"radiobutton", L"单选框2", -1, 100, 60, 80, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);

	EX_OBJ_PROPS CheckButtonExProps = { 0 };
	HEXOBJ hObj_checkbutton1 = Ex_ObjCreate(L"CheckButtonEx", L"扩展复选框", -1, 10, 90, 120, 30, hExDui_checkbutton);
	Ex_ObjSetColor(hObj_checkbutton1, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), TRUE);
	CheckButtonExProps.crBkgDownOrChecked = ExARGB(215, 117, 23, 250);
	CheckButtonExProps.crBorderNormal = ExARGB(222, 222, 222, 150);
	CheckButtonExProps.crBorderHover = ExARGB(222, 222, 222, 200);
	CheckButtonExProps.crBorderDownOrChecked = ExARGB(222, 222, 222, 250);
	Ex_ObjSendMessage(hObj_checkbutton1, WM_EX_PROPS, 0, (LPARAM)&CheckButtonExProps);
	Ex_ObjSendMessage(hObj_checkbutton1, BM_SETCHECK, 1, 0);

	HEXOBJ hObj_checkbutton2 = Ex_ObjCreate(L"CheckButtonEx", L"扩展复选框", -1, 10, 120, 120, 30, hExDui_checkbutton);
	CheckButtonExProps.crBkgDownOrChecked = ExARGB(251, 171, 213, 250);
	CheckButtonExProps.crBorderNormal = ExARGB(22, 222, 222, 150);
	CheckButtonExProps.crBorderHover = ExARGB(22, 222, 222, 200);
	CheckButtonExProps.crBorderDownOrChecked = ExARGB(222, 22, 222, 250);
	Ex_ObjSendMessage(hObj_checkbutton2, WM_EX_PROPS, 0, (LPARAM)&CheckButtonExProps);
	Ex_ObjHandleEvent(hObj_checkbutton2, NM_CHECK, OnCheckButtonCheckedEvent);

	HEXOBJ hObj_checkbutton3 = Ex_ObjCreate(L"CheckButtonEx", L"三态选择框", -1, 10, 150, 100, 30, hExDui_checkbutton);
	Ex_ObjSendMessage(hObj_checkbutton3, BM_SETCHECK, 2, 0);

	Ex_ObjCreate(L"RadioButtonEx", L"扩展单选框", -1, 150, 120, 120, 30, hExDui_checkbutton);
	HEXOBJ hObj_radiobutton2 = Ex_ObjCreate(L"RadioButtonEx", L"扩展单选框2", -1, 150, 150, 120, 30, hExDui_checkbutton);
	Ex_ObjSendMessage(hObj_radiobutton2, BM_SETCHECK, 1, 0);

	HEXOBJ hObj_checkbox1 = Ex_ObjCreate(L"CheckBox", L"记住账号", -1, 10, 200, 100, 30, hExDui_checkbutton);
	Ex_ObjHandleEvent(hObj_checkbox1, NM_CHECK, OnCheckButtonCheckedEvent);
	Ex_ObjSetProp(hObj_checkbox1, ECBP_TEXT, ExARGB(120, 40, 160, 255));
	Ex_ObjSetProp(hObj_checkbox1, ECBP_CRHOVERCHECK, ExARGB(66, 66, 66, 255));
	EX_OBJ_PROPS prop;
	prop.crBkgNormal = ExARGB(120, 80, 55, 255);
	prop.crBkgHover = ExARGB(150, 100, 80, 255);
	prop.crBkgDownOrChecked = ExARGB(250, 150, 180, 255);
	Ex_ObjSendMessage(hObj_checkbox1, WM_EX_PROPS, 0, (size_t)&prop);
	HEXOBJ hObj_checkbox2 = Ex_ObjCreate(L"CheckBox", L"记住密码", -1, 150, 200, 100, 30, hExDui_checkbutton);
	Ex_ObjSendMessage(hObj_checkbox2, BM_SETCHECK, 1, 0);

	Ex_DUIShowWindow(hExDui_checkbutton, SW_SHOWNORMAL, 0, 0, 0);
}

HEXFONT m_hEditFont;
std::vector<HEXOBJ> m_hEditButton;
HEXDUI m_hExDuiEdit;

LRESULT CALLBACK OnEditWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_CLOSE)
	{
		if (m_hEditFont)
		{
			_font_destroy(m_hEditFont); //此处不销毁会在引擎销毁时统一销毁
		}
		m_hEditFont = 0;
	}
	return 0;
}

LRESULT CALLBACK OnEditButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	HEXOBJ hEdit = Ex_ObjGetFromID(m_hExDuiEdit, 101);
	if (hEdit)
	{
		if (nID == 201) //全选
		{
			EX_CHARRANGE charRange;
			charRange.cpMin = 0;
			charRange.cpMax = -1;
			Ex_ObjSendMessage(hEdit, EM_EXSETSEL, 0, (LPARAM)&charRange);
		}
		else if (nID == 202) //取消选择
		{
			Ex_ObjSendMessage(hEdit, EM_SETSEL, 0, 0);
		}
		else if (nID == 203) //置选择
		{
			Ex_ObjSendMessage(hEdit, EM_SETSEL, 1, 15);
		}
		else if (nID == 204) //左对齐
		{
			Ex_ObjEditSetSelParFormat(hEdit, PFM_ALIGNMENT, NULL, NULL, NULL, NULL, PFA_LEFT);
		}
		else if (nID == 205) //居中对齐
		{
			Ex_ObjEditSetSelParFormat(hEdit, PFM_ALIGNMENT, NULL, NULL, NULL, NULL, PFA_CENTER);
		}
		else if (nID == 206) //右对齐
		{
			Ex_ObjEditSetSelParFormat(hEdit, PFM_ALIGNMENT, NULL, NULL, NULL, NULL, PFA_RIGHT);
		}
		else if (nID == 207) //首行缩进
		{
			Ex_ObjEditSetSelParFormat(hEdit, PFM_STARTINDENT, NULL, 20);
		}
		else if (nID == 208) //右侧缩进
		{
			Ex_ObjEditSetSelParFormat(hEdit, PFM_RIGHTINDENT, NULL, NULL, 20);
		}
		else if (nID == 209) //非首行缩进
		{
			Ex_ObjEditSetSelParFormat(hEdit, PFM_OFFSET, NULL, NULL, NULL, 50);
		}
		else if (nID == 210) //项目符号
		{
			Ex_ObjEditSetSelParFormat(hEdit, PFM_NUMBERING, PFN_LCROMAN);
		}
		else if (nID == 211) //文本颜色
		{
			Ex_ObjEditSetSelCharFormat(hEdit, CFM_COLOR, ExRGB2ARGB(16711680, 255));
		}
		else if (nID == 212) //加粗
		{
			Ex_ObjEditSetSelCharFormat(hEdit, CFM_BOLD, NULL, NULL, NULL, NULL, TRUE);
		}
		else if (nID == 213) //倾斜
		{
			Ex_ObjEditSetSelCharFormat(hEdit, CFM_ITALIC, NULL, NULL, NULL, NULL, NULL, TRUE);
		}
		else if (nID == 214) //下划线
		{
			Ex_ObjEditSetSelCharFormat(hEdit, CFM_UNDERLINE, NULL, NULL, NULL, NULL, NULL, NULL, TRUE);
		}
		else if (nID == 215) //删除线
		{
			Ex_ObjEditSetSelCharFormat(hEdit, CFM_STRIKEOUT, NULL, NULL, NULL, NULL, NULL, NULL, NULL, TRUE);
		}
		else if (nID == 216) //超链接
		{
			Ex_ObjEditSetSelCharFormat(hEdit, CFM_LINK, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, TRUE);
		}
		else if (nID == 217) //幼圆30
		{
			Ex_ObjEditSetSelCharFormat(hEdit, CFM_SIZE | CFM_FACE, NULL, L"幼圆", 30);
		}
		else if (nID == 218) //撤销
		{
			Ex_ObjSendMessage(hEdit, EM_UNDO, 0, 0);
		}
		else if (nID == 219) //重做
		{
			Ex_ObjSendMessage(hEdit, EM_REDO, 0, 0);
		}
		else if (nID == 220) //复制
		{
			Ex_ObjSendMessage(hEdit, WM_COPY, 0, 0);
		}
		else if (nID == 221) //剪切
		{
			Ex_ObjSendMessage(hEdit, WM_CUT, 0, 0);
		}
		else if (nID == 222) //粘贴
		{
			Ex_ObjSendMessage(hEdit, WM_PASTE, 0, 0);
		}
		else if (nID == 223) //删除
		{
			Ex_ObjSendMessage(hEdit, WM_CLEAR, 0, 0);
		}
		else if (nID == 224) //取行数
		{
			OUTPUTW(L"编辑框行数:", Ex_ObjSendMessage(hEdit, EM_GETLINECOUNT, 0, 0));
		}
		else if (nID == 225) //寻找文本
		{
			EX_TEXTRANGE textRange;
			textRange.chrg.cpMin = HIWORD(Ex_ObjSendMessage(hEdit, EM_GETSEL, 0, 0));
			textRange.chrg.cpMax = -1;
			textRange.pwzText = L"a"; //欲寻找文本
			textRange.chrg.cpMin = Ex_ObjSendMessage(hEdit, EM_FINDTEXTW, FR_DOWN, (LPARAM)&textRange);
			if (textRange.chrg.cpMin != -1)
			{
				textRange.chrg.cpMax = textRange.chrg.cpMin + lstrlenW(textRange.pwzText);
				Ex_ObjSendMessage(hEdit, EM_SETSEL, textRange.chrg.cpMin, textRange.chrg.cpMax);
			}
		}
		else if (nID == 226) //替换文本
		{
			EX_SETTEXTEX textformat;
			textformat.flags = 2;       //ST_SELECTION替换选择并保持富文本格式
			textformat.codePage = 1200; //Unicode code page
			Ex_ObjSendMessage(hEdit, EM_SETTEXTEX, (WPARAM)&textformat, (LPARAM)L"选中替换为我");
		}

	}
	return 0;
}

LRESULT CALLBACK OnEditNotifyEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == EN_SELCHANGE)
	{
		OUTPUTW(L"选中区域改变:", ((EX_SELCHANGE*)lParam)->chrg.cpMin, ((EX_SELCHANGE*)lParam)->chrg.cpMax);
	}
	else if (nCode == EN_LINK)
	{
		if (((EX_ENLINK*)lParam)->msg == WM_LBUTTONDOWN)
		{
			EX_TEXTRANGE TextRange;
			TextRange.chrg = ((EX_ENLINK*)lParam)->chrg;
			TextRange.pwzText = (LPCWSTR)Ex_AllocBuffer((TextRange.chrg.cpMax - TextRange.chrg.cpMin + 2) * 2);
			Ex_ObjSendMessage(hObj, EM_GETTEXTRANGE, 0, (LPARAM)&TextRange);
			OUTPUTW(L"链接被按下:", TextRange.chrg.cpMax, TextRange.chrg.cpMin, TextRange.pwzText);
			Ex_FreeBuffer((LPVOID)TextRange.pwzText);
		}
	}
	return 0;
}

void test_edit(HWND hWnd)
{
	HWND hWnd_edit = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试编辑框", 0, 0, 850, 350, 0, 0);
	m_hExDuiEdit = Ex_DUIBindWindowEx(hWnd_edit, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(m_hExDuiEdit, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ hObj_edit1 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED | EOS_EX_CUSTOMDRAW, L"edit", L"", EOS_VISIBLE | EES_HIDESELECTION, 10, 30, 150, 30, m_hExDuiEdit, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ObjSendMessage(hObj_edit1, EM_SETCUEBANNER, ExARGB(0, 0, 0, 100), (LPARAM)L"背景图片编辑框");
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/editbkg.jpg", &imgdata);
	Ex_ObjSendMessage(hObj_edit1, EM_SETCUEBANNER, ExARGB(150, 150, 150, 255), (LPARAM)L"背景图片编辑框");
	Ex_ObjSetBackgroundImage(hObj_edit1, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_DEFAULT, 255, TRUE);
	HEXOBJ hObj_edit2 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"测试密码输入编辑框", EOS_VISIBLE | EES_USEPASSWORD, 10, 70, 150, 30, m_hExDuiEdit, 0, DT_SINGLELINE, 0, 0, NULL);
	Ex_ObjSendMessage(hObj_edit2, EM_SETCUEBANNER, ExARGB(0, 0, 0, 100), (LPARAM)L"测试密码输入编辑框");
	HEXOBJ hObj_edit3 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"", EOS_VISIBLE | EES_NUMERICINPUT, 10, 110, 150, 30, m_hExDuiEdit, 0, DT_SINGLELINE, 0, 0, NULL);
	Ex_ObjSendMessage(hObj_edit3, EM_SETCUEBANNER, ExARGB(0, 0, 0, 100), (LPARAM)L"测试数值输入编辑框");
	HEXOBJ hObj_edit4 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"测试只读编辑框", EOS_VISIBLE | EES_READONLY, 10, 150, 150, 30, m_hExDuiEdit, 0, DT_SINGLELINE, 0, 0, NULL);

	HEXOBJ hObj_edit5 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED | EOS_EX_TABSTOP | EOS_EX_CUSTOMDRAW, L"edit", L"透明圆角编辑框", EOS_VISIBLE | EES_HIDESELECTION, 10, 190, 150, 44, m_hExDuiEdit, 0, DT_VCENTER, 0, 0, NULL);
	m_hEditFont = _font_createfromfamily(L"微软雅黑", 24, EFS_UNDERLINE | EFS_ITALIC);
	Ex_ObjSetFont(hObj_edit5, m_hEditFont, FALSE);
	Ex_ObjSetColor(hObj_edit5, COLOR_EX_BACKGROUND, ExARGB(200, 120, 130, 100), FALSE);
	Ex_ObjSetColor(hObj_edit5, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16872215, 100), FALSE);
	Ex_ObjSetRadius(hObj_edit5, 10, 10, 10, 0, FALSE);

	HEXOBJ hobj_edit_multiline = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED, L"edit", L"测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\n", EOS_VISIBLE | EOS_VSCROLL, 10, 245, 150, 100, m_hExDuiEdit, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ObjSendMessage(hobj_edit_multiline, EM_SETSEL, -1, -1);                        //移动到最后一行
	Ex_ObjSendMessage(hobj_edit_multiline, EM_REPLACESEL, -1, (LPARAM)L"新添加一行"); //添加一行
	Ex_ObjSetFocus(hobj_edit_multiline);                                              //添加焦点自动滚动到最后一行

	HEXOBJ hObj_edit7 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"edit", NULL, EOS_VISIBLE | EOS_VSCROLL | EOS_HSCROLL | EES_RICHTEXT | EES_PARSEURL | EES_ALLOWTAB | EES_NEWLINE, 180, 30, 300, 300, m_hExDuiEdit, 101, DT_LEFT | DT_TOP, 0, 0, NULL);
	std::vector<CHAR> rtf;
	Ex_ReadFile(L"res/test.rtf", &rtf);

	Ex_ObjSendMessage(hObj_edit7, EM_LOAD_RTF, rtf.size(), (size_t)rtf.data());
	Ex_ObjHandleEvent(hObj_edit7, EN_SELCHANGE, OnEditNotifyEvent);
	Ex_ObjHandleEvent(hObj_edit7, EN_LINK, OnEditNotifyEvent);

	std::vector<std::wstring> buttonTitles = { L"全选", L"取消选择", L"置选择", L"左对齐 ", L"居中对齐", L"右对齐", L"首行缩进 ", L"右侧缩进", L"非首行缩进", L"项目符", L"文本蓝色 ", L"加粗", L"倾斜", L"下划线", L"删除线", L"超链接", L"幼圆30", L"撤销 ", L"重做", L"复制", L"剪切", L"粘贴", L"删除", L"取行数 ", L"寻找文本", L"替换文本" };
	m_hEditButton.resize(buttonTitles.size());
	HEXOBJ hObj_page = Ex_ObjCreate(L"Page", NULL, -1, 500, 30, 350, 300, m_hExDuiEdit);
	HEXLAYOUT hLayoutPage = _layout_create(ELT_FLOW, hObj_page);
	BOOL bWrapLine;
	for (int i = 0; i < buttonTitles.size(); i++)
	{
		bWrapLine = buttonTitles[i].find_last_of(' ') == buttonTitles[i].length() - 1;
		std::wstring title;
		if (bWrapLine)
		{
			title = buttonTitles[i].substr(0, buttonTitles[i].length() - 1);
		}
		else
		{
			title = buttonTitles[i];
		}
		m_hEditButton[i] = Ex_ObjCreateEx(EOS_EX_AUTOSIZE, L"button", title.c_str(), -1, 10, 30, 10, 10, hObj_page, 201 + i, -1, 0, 0, 0);
		_layout_setchildprop(hLayoutPage, m_hEditButton[i], ELCP_MARGIN_RIGHT, 5);
		_layout_setchildprop(hLayoutPage, m_hEditButton[i], ELCP_MARGIN_BOTTOM, 5);
		if (bWrapLine)
		{
			_layout_setchildprop(hLayoutPage, m_hEditButton[i], ELCP_FLOW_NEW_LINE, 1);
		}
		Ex_ObjHandleEvent(m_hEditButton[i], NM_CLICK, OnEditButtonEvent);
	}
	Ex_ObjLayoutSet(hObj_page, hLayoutPage, TRUE);

	Ex_DUIShowWindow(m_hExDuiEdit, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnListButtonWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR notify{ 0 };
		RtlMoveMemory(&notify, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (notify.nCode == NM_CREATE)
		{
			Ex_ObjSetColor(notify.hObjFrom, COLOR_EX_TEXT_NORMAL, ExRGBA(210, 120, 55, 255), TRUE); //改变菜单项目字体正常颜色
			Ex_ObjSetColor(notify.hObjFrom, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16711680, 255), TRUE);  //改变菜单项目字体悬浮颜色
			Ex_ObjSetColor(notify.hObjFrom, COLOR_EX_BACKGROUND, ExRGBA(110, 120, 55, 255), TRUE);  //改变菜单项目背景颜色
		}
	}
	else if (uMsg == MN_SELECTITEM && (DWORD)wParam == -1) //恢复正常状态
	{
		POINT point = { 0 };
		GetCursorPos(&point);
		HWND currentWnd = WindowFromPoint(point);
		ScreenToClient(currentWnd, &point);
		HEXOBJ hObj = Ex_DUIGetObjFromPoint((EXHANDLE)currentWnd, point.x, point.y);
		Ex_ObjPostMessage(hObj, LBM_SELECTITEM, 0, 0);
	}
	return 0;
}

LRESULT CALLBACK OnListButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == LBM_DOWNITEM)
	{
		RECT rcWindow{ 0 };
		RECT rcObj{ 0 };
		GetWindowRect(hWnd, &rcWindow);
		Ex_ObjGetRectEx(hObj, &rcObj, 2);
		Ex_TrackPopupMenu((HMENU)lParam, TPM_RECURSE, rcWindow.left + rcObj.left + wParam, rcWindow.top + Ex_Scale(rcObj.bottom), 0, hObj, NULL, OnListButtonWndMsgProc, EMNF_NOSHADOW);
		*lpResult = 1;
		return 1;
	}

	return 0;
}

LRESULT CALLBACK OnListButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == LBN_CLICK)
	{
		OUTPUTW(L"点击", wParam, lParam);
	}
	else if (nCode == LBN_CHECK)
	{
		OUTPUTW(L"选择", wParam, lParam);
	}
	return 0;
}

void test_listbutton(HWND hWnd)
{
	HWND hWnd_listbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试列表按钮", 0, 0, 480, 200, 0, 0);
	HEXDUI hExDui_listbutton = Ex_DUIBindWindowEx(hWnd_listbutton, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_listbutton, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HMENU hMenu = LoadMenuW(GetModuleHandleW(0), (LPWSTR)IDR_MENU1);

	//创建正常菜单条
	HEXOBJ hObj = 0;
	hObj = Ex_ObjCreate(L"Menubar", 0, -1, 0, 30, 220, 22, hExDui_listbutton);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), FALSE);       //文本色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16774117, 255), FALSE); //点燃背景色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16765337, 255), FALSE);  //按下背景色

	EX_LISTBUTTON_ITEMINFO item1 = { 0 };
	item1.wzText = L"文件(&F)";
	item1.nMenu = GetSubMenu(hMenu, 0);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item1);
	item1.wzText = L"编辑(&E)";
	item1.nMenu = GetSubMenu(hMenu, 1);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item1);
	item1.wzText = L"选项(&O)";
	item1.nMenu = GetSubMenu(hMenu, 2);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item1);
	item1.wzText = L"帮助(&H)";
	item1.nMenu = GetSubMenu(hMenu, 3);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 1, (size_t)&item1);

	//创建自定义回调菜单条

	hObj = Ex_ObjCreateEx(-1, L"Menubar", 0, -1, 0, 60, 220, 22, hExDui_listbutton, 0, -1, 0, 0, OnListButtonMsgProc);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGBA(110, 120, 55, 255), FALSE);   //改变菜单按钮背景色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), FALSE); //改变菜单按钮字体正常色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExARGB(255, 255, 255, 55), FALSE);   //改变菜单按钮字体热点色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExARGB(255, 255, 255, 100), FALSE);   //改变菜单按钮字体按下色
	item1.wzText = L"文件(&F)";
	item1.nMenu = GetSubMenu(hMenu, 0);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item1);
	item1.wzText = L"编辑(&E)";
	item1.nMenu = GetSubMenu(hMenu, 1);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item1);
	item1.wzText = L"选项(&O)";
	item1.nMenu = GetSubMenu(hMenu, 2);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item1);
	item1.wzText = L"帮助(&H)";
	item1.nMenu = GetSubMenu(hMenu, 3);
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 1, (size_t)&item1);

	//创建工具条
	hObj = Ex_ObjCreate(L"Toolbar", 0, -1, 0, 90, 400, 22, hExDui_listbutton);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), FALSE);       //文本色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16774117, 255), FALSE); //点燃背景色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16765337, 255), FALSE);  //按下背景色
	HEXIMAGELIST hImageList = _imglist_create(18, 18);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"buttonex\\4.png", &imgdata);
	size_t nImageIndex = _imglist_add(hImageList, imgdata.data(), imgdata.size(), 0);
	Ex_ObjSendMessage(hObj, LVM_SETIMAGELIST, 0, (LPARAM)hImageList);
	EX_LISTBUTTON_ITEMINFO item2 = { 0 };
	item2.nType = 1;
	item2.nImage = nImageIndex;
	item2.wzText = NULL;
	item2.wzTips = L"普通按钮1";
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item2);
	item2.nType = 1;
	item2.nImage = 0;
	item2.wzText = L"普通按钮不带图标";
	item2.wzTips = L"普通按钮不带图标";
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item2);
	item2.nType = 2;
	item2.nImage = nImageIndex;
	item2.wzText = NULL;
	item2.wzTips = L"选择按钮1";
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item2);
	item2.nType = 2;
	item2.nImage = 0;
	item2.wzText = L"选择按钮不带图标";
	item2.wzTips = L"选择按钮不带图标";
	item2.dwState = STATE_DOWN;
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item2);
	item2.nType = 0;
	item2.nImage = 0;
	item2.wzText = NULL;
	item2.wzTips = NULL;
	item2.dwState = STATE_NORMAL;
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item2);
	item2.nType = 1;
	item2.nImage = nImageIndex;
	item2.wzText = L"禁用按钮带图标";
	item2.wzTips = L"禁用按钮带图标";
	item2.dwState = STATE_DISABLE;
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 1, (size_t)&item2);
	Ex_ObjHandleEvent(hObj, LBN_CLICK, OnListButtonEvent);
	Ex_ObjHandleEvent(hObj, LBN_CHECK, OnListButtonEvent);

	//创建状态条
	hObj = Ex_ObjCreate(L"Statusbar", 0, -1, 0, 120, 300, 22, hExDui_listbutton);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(12557930, 255), FALSE);   //背景色
	Ex_ObjSetColor(hObj, COLOR_EX_BORDER, ExARGB(255, 255, 255, 255), FALSE);      //分割色
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), FALSE); //文本色
	EX_LISTBUTTON_ITEMINFO item3 = { 0 };
	item3.wzText = L"左对齐";
	item3.nWidth = 100;
	item3.TextFormat = DT_LEFT;
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item3);
	item3.wzText = L"居中";
	item3.nWidth = 100;
	item3.TextFormat = DT_CENTER;
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 0, (size_t)&item3);
	item3.wzText = L"右对齐";
	item3.nWidth = 100;
	item3.nIndex = 2;
	item3.TextFormat = DT_RIGHT;
	Ex_ObjSendMessage(hObj, LVM_INSERTITEM, 1, (size_t)&item3);

	//设置菜单条目图标
	if (hMenu)
	{
		HMENU hMenu_sub = GetSubMenu(hMenu, 0);
		if (hMenu_sub)
		{
			MENUITEMINFOW minfo{ 0 };
			minfo.cbSize = sizeof(MENUITEMINFOW);
			minfo.fMask = MIIM_BITMAP;
			GetMenuItemInfoW(hMenu_sub, 1, TRUE, &minfo);
			if (minfo.hbmpItem)
			{
				DeleteObject(minfo.hbmpItem);
			}
			std::vector<CHAR> imgdata;
			HBITMAP hbitmap = 0;
			HEXIMAGE hImg = 0;
			HEXIMAGE hImgSmall = 0;
			_img_createfromfile(L"res/rotateimgbox.jpg", &hImg);
			_img_scale(hImg, 24, 24, &hImgSmall); //注意菜单条目高度跟图像高度有关，因此缩放到24
			imgdata.resize(24 * 24 * 4);
			_img_savetomemory(hImgSmall, imgdata.data());
			Ex_LoadBitMapFromMemory(imgdata.data(), imgdata.size(), &hbitmap);
			_img_destroy(hImg);
			_img_destroy(hImgSmall);
			minfo.hbmpItem = hbitmap;
			SetMenuItemInfoW(hMenu_sub, 1, TRUE, &minfo);
		}
	}
	Ex_DUIShowWindow(hExDui_listbutton, SW_SHOWNORMAL, 0, 0, 0);
}

void test_custombkg(HWND hWnd)
{
	HWND hWnd_custombkg = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"", 0, 0, 175, 200, 0, 0);
	HEXDUI hExDui_custombkg = Ex_DUIBindWindowEx(hWnd_custombkg, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW, 0, 0);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/custombkg.png", &imgdata);
	RECT grid = { 45, 40, 15, 15 };
	Ex_ObjSetBackgroundImage(hExDui_custombkg, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, &grid, BIF_DEFAULT, 220, TRUE);
	Ex_ObjCreateEx(EOS_EX_TOPMOST, L"sysbutton", L"", EOS_VISIBLE | EWS_BUTTON_CLOSE, 140, 8, 30, 30, hExDui_custombkg, 300, 0, 0, 0, NULL);
	Ex_DUIShowWindow(hExDui_custombkg, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<LISTVIEW_ITEM> m_listViewItemInfo;

LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (hObj == ni.hObjFrom)
		{
			if (ni.nCode == NM_CALCSIZE)
			{
				((EX_LISTVIEW_INFO*)ni.lParam)->widthItem = 120;
				((EX_LISTVIEW_INFO*)ni.lParam)->heightItem = 40;
				((EX_LISTVIEW_INFO*)ni.lParam)->widthInterval = 0;
				((EX_LISTVIEW_INFO*)ni.lParam)->heightInterval = 0;
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == NM_CUSTOMDRAW)
			{
				EX_CUSTOMDRAW cd{ 0 };
				RtlMoveMemory(&cd, (LPVOID)ni.lParam, sizeof(EX_CUSTOMDRAW));

				EXARGB crItemBkg = 0;
				if ((cd.dwState & STATE_SELECT) != 0)
				{
					crItemBkg = ExRGB2ARGB(16777215, 255);
				}
				else if ((cd.dwState & STATE_HOVER) != 0)
				{
					crItemBkg = ExRGB2ARGB(16777215, 150);
				}
				if (crItemBkg != 0)
				{
					HEXBRUSH hBrush = _brush_create(crItemBkg);
					_canvas_fillrect(cd.hCanvas, hBrush, cd.rcPaint.left, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);
					_brush_destroy(hBrush);
				}
				_canvas_drawtext(cd.hCanvas, Ex_ObjGetFont(hObj), m_listViewItemInfo[cd.iItem - 1].color, m_listViewItemInfo[cd.iItem - 1].text, -1, DT_SINGLELINE | DT_VCENTER, cd.rcPaint.left + m_listViewItemInfo[cd.iItem - 1].depth * 5, cd.rcPaint.top, cd.rcPaint.right, cd.rcPaint.bottom);

				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == LVN_ITEMCHANGED)
			{
				//wParam 新选中项,lParam 旧选中项
				OUTPUTW(L"改变选中ID:", ni.idFrom, L"新选中项:", ni.wParam, L"旧选中项:", ni.lParam);
			}
		}
	}
	else if (uMsg == WM_DESTROY)
	{
		for (auto index : m_listViewItemInfo)
		{
			Ex_FreeBuffer((LPVOID)index.text); //复制的文本要销毁
		}
	}
	return 0;
}

LRESULT CALLBACK OnScrollBarMsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_MOUSEHOVER)
	{
		Ex_ObjPostMessage(hObj, SBM_SETVISIBLE, 0, 1); //显示滚动条
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjPostMessage(hObj, SBM_SETVISIBLE, 0, 0); //隐藏滚动条
	}
	else if (uMsg == SBM_SETVISIBLE)
	{
		Ex_ObjSetLong(hObj, EOL_ALPHA, lParam != 0 ? 255 : 0);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	return 0;
}

void test_listview(HWND hWnd)
{
	HWND hWnd_listview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试列表框", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_listview = Ex_DUIBindWindowEx(hWnd_listview, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_listview, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hobj_listview = Ex_ObjCreateEx(EOS_EX_COMPOSITED, L"listview", NULL, EOS_VISIBLE | ELVS_VERTICALLIST | EOS_VSCROLL, 30, 30, 550, 450, hExDui_listview, 0, -1, 0, 0, OnListViewMsgProc);
	Ex_ObjSetColor(hobj_listview, COLOR_EX_BACKGROUND, ExARGB(120, 255, 240, 150), TRUE);
	const INT itemCount = 100;
	m_listViewItemInfo.resize(itemCount);
	for (INT index = 0; index < m_listViewItemInfo.size(); index++)
	{
		m_listViewItemInfo[index].color = ExRGB2ARGB(Random(0, 16777215), 255);
		auto str = L"列表项" + std::to_wstring(index);
		m_listViewItemInfo[index].text = StrDupW(str.c_str());
		m_listViewItemInfo[index].depth = index % 5;
	}
	Ex_ObjSendMessage(hobj_listview, LVM_SETITEMCOUNT, itemCount, itemCount);
	HEXOBJ hObj_scroll = Ex_ObjScrollGetControl(hobj_listview, SB_VERT);
	Ex_ObjPostMessage(hObj_scroll, SBM_SETVISIBLE, 0, 0);            //隐藏滚动条
	Ex_ObjSetLong(hObj_scroll, EOL_OBJPROC, (size_t)OnScrollBarMsg); //改变滚动条回调
	Ex_DUIShowWindow(hExDui_listview, SW_SHOWNORMAL, 0, 0, 0);
}

void test_groupbox(HWND hWnd)
{
	HWND hWnd_groupbox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试分组框", 0, 0, 400, 300, 0, 0);
	HEXDUI hExDui_groupbox = Ex_DUIBindWindowEx(hWnd_groupbox, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_groupbox, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj_groupbox = Ex_ObjCreate(L"groupbox", L"分组框", -1, 30, 30, 230, 230, hExDui_groupbox);
	Ex_ObjSetColor(hObj_groupbox, COLOR_EX_TEXT_NORMAL, ExARGB(255, 55, 55, 255), FALSE);
	Ex_ObjSetColor(hObj_groupbox, COLOR_EX_BORDER, ExARGB(55, 0, 250, 255), FALSE);
	Ex_ObjSetLong(hObj_groupbox, GROUPBOX_TEXT_OFFSET, 50);
	Ex_ObjSetLong(hObj_groupbox, GROUPBOX_RADIUS, 30);
	Ex_ObjSetLong(hObj_groupbox, GROUPBOX_STROKEWIDTH, 3);
	Ex_DUIShowWindow(hExDui_groupbox, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<HEXOBJ> m_hNavBtn(4);
std::vector<HEXOBJ> m_hPageNavBtn(4);
INT m_nCurIndex = 1;
HEXLAYOUT m_hLayout;

LRESULT CALLBACK OnNavButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (lParam != 0)
	{
		m_nCurIndex = Ex_ObjGetLong(hObj, EOL_LPARAM);
		_layout_setprop(m_hLayout, ELP_PAGE_CURRENT, m_nCurIndex + 1);
		_layout_update(m_hLayout);
	}
	return 0;
}

LRESULT CALLBACK OnNavButtonMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_DESTROY)
	{

	}

	return 0;
}

void test_navbutton(HWND hWnd)
{
	HWND hWnd_navbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试选项卡", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_navbutton = Ex_DUIBindWindowEx(hWnd_navbutton, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);

	for (INT i = 0; i < 4; i++)
	{
		HEXIMAGE hImage = 0;
		HEXIMAGE hImg1 = 0;
		HEXIMAGE hImg2 = 0;
		auto str = L"Tab" + std::to_wstring(i);
		m_hNavBtn[i] = Ex_ObjCreate(L"NavButtonEx", str.c_str(), -1, 20 + i * 85, 40, 80, 80, hExDui_navbutton);

		auto file = L"./navbtn/大图标" + std::to_wstring(i + 1) + L".png";
		_img_createfromfile(file.c_str(), &hImage);

		Ex_ObjSendMessage(m_hNavBtn[i], WM_SETICON, 0, hImage);
		Ex_ObjSetColor(m_hNavBtn[i], COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);

		_img_createfromfile(L"./navbtn/顶部按钮背景C.png", &hImg1);

		Ex_ObjSendMessage(m_hNavBtn[i], BM_SETIMAGE, 1, hImg1);

		_img_createfromfile(L"./navbtn/顶部按钮背景D.png", &hImg2);

		Ex_ObjSendMessage(m_hNavBtn[i], BM_SETIMAGE, 2, hImg2);

		Ex_ObjInvalidateRect(m_hNavBtn[i], 0);
		Ex_ObjSetLong(m_hNavBtn[i], EOL_LPARAM, i);
		Ex_ObjHandleEvent(m_hNavBtn[i], NM_CHECK, OnNavButtonCheckEvent);
	}
	HEXOBJ hPageNavbtnContainer = Ex_ObjCreate(L"page", 0, -1, 20, 120, 760, 600 - 120 - 20, hExDui_navbutton);
	//创建一个页面布局,绑定容器页面
	m_hLayout = _layout_create(ELT_PAGE, hPageNavbtnContainer);
	for (INT i = 0; i < 4; i++)
	{
		auto str = L"页面" + std::to_wstring(i);
		m_hPageNavBtn[i] = Ex_ObjCreateEx(-1, L"static", str.c_str(), -1, 0, 0, 760, 600 - 120 - 20, hPageNavbtnContainer, 0, DT_CENTER | DT_VCENTER, 0, 0, 0);
		Ex_ObjSetFontFromFamily(m_hPageNavBtn[i], 0, 40, 0, TRUE);
		Ex_ObjSetColor(m_hPageNavBtn[i], COLOR_EX_BACKGROUND, ExRGB2ARGB(Random(0, 16777215), 255), TRUE);
		Ex_ObjSetColor(m_hPageNavBtn[i], COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
		//布局添加子页面
		_layout_addchild(m_hLayout, m_hPageNavBtn[i]);
	}
	//容器页面设置布局对象
	Ex_ObjLayoutSet(hPageNavbtnContainer, m_hLayout, TRUE);
	//设置布局显示页面索引,从1开始
	m_nCurIndex = 1;
	_layout_setprop(m_hLayout, ELP_PAGE_CURRENT, m_nCurIndex);


	Ex_ObjSendMessage(m_hNavBtn[0], BM_SETCHECK, 1, 1);
	Ex_DUISetLong(hExDui_navbutton, EWL_CRBKG, ExRGB2ARGB(0, 255));
	Ex_DUIShowWindow(hExDui_navbutton, SW_SHOWNORMAL, 0, 0, 0);
}

void test_absolute(HWND hWnd)
{
	HWND hWnd_absolute = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试绝对布局", 0, 0, 450, 300, 0, 0);
	HEXDUI hExDui_absolute = Ex_DUIBindWindowEx(hWnd_absolute, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_absolute, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_ABSOLUTE, hExDui_absolute);

	/*HEXOBJ hObj = Ex_ObjCreate(L"Static", L"左20顶50底20宽200", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, 50);
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 20);

	HEXOBJ hObj2 = Ex_ObjCreate(L"Static", L"顶50右20左220高50%内边距10", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, 50);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 240);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PS, 50);
	_layout_setchildprop(hLayout, hObj2, ELCP_MARGIN_BOTTOM, 45);

	HEXOBJ hObj3 = Ex_ObjCreate(L"Static", L"底20右20左220高50%内边距10", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 20);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 240);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PS, 50);
	_layout_setchildprop(hLayout, hObj3, ELCP_MARGIN_TOP, 45);*/

	HEXOBJ hObj1 = Ex_ObjCreate(L"Static", L"固定在右下角50,50的位置，不变大小", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj1, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj1, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, 50);  //设置距离右边为50像素
	_layout_absolute_setedge(hLayout, hObj1, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 50); // 设置距离底边为50像素

	HEXOBJ hObj2 = Ex_ObjCreate(L"Static", L"固定在左下角50,50的位置，宽度为40%,高度为50", -1, 0, 0, 200, 100, hExDui_absolute);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(0, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 50);   //设置距离左边为50像素
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 50); // 设置距离底边为50像素
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_WIDTH, ELCP_ABSOLUTE_TYPE_PS, 40);  // 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj2, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PX, 50);

	HEXOBJ hObj3 = Ex_ObjCreate(L"Static", L"距离四边均为20%", -1, 0, 0, 0, 0, hExDui_absolute);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PS, 20);   //注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PS, 20);    // 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PS, 20);  // 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj3, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PS, 20); // 注意单位是PS（百分比）

	HEXOBJ hObj4 = Ex_ObjCreate(L"Static", L"居中于窗口,宽度为窗口的30%,高度为100像素", -1, 0, 0, 0, 0, hExDui_absolute);
	Ex_ObjSetColor(hObj4, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PS, 50);         //注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PS, 50);          // 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_WIDTH, ELCP_ABSOLUTE_TYPE_PS, 30);        // 注意单位是PS（百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_HEIGHT, ELCP_ABSOLUTE_TYPE_PX, 100);      //
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_OFFSET_H, ELCP_ABSOLUTE_TYPE_OBJPS, -50); //  水平偏移控件-50%的控件宽度 注意单位是OBJ_PS（控件尺寸的百分比）
	_layout_absolute_setedge(hLayout, hObj4, ELCP_ABSOLUTE_OFFSET_V, ELCP_ABSOLUTE_TYPE_OBJPS, -50); //  水平偏移控件-50%的控件高度 注意单位是OBJ_PS（控件尺寸的百分比）

	Ex_ObjLayoutSet(hExDui_absolute, hLayout, TRUE);

	Ex_DUIShowWindow(hExDui_absolute, SW_SHOWNORMAL, 0, 0, 0);
}

void test_relative(HWND hWnd)
{
	HWND hWnd_relative = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试相对布局", 0, 0, 600, 400, 0, 0);
	HEXDUI hExDui_relative = Ex_DUIBindWindowEx(hWnd_relative, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_relative, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_RELATIVE, hExDui_relative);
	_layout_setprop(hLayout, ELP_PADDING_LEFT, 10);
	_layout_setprop(hLayout, ELP_PADDING_TOP, 25);
	_layout_setprop(hLayout, ELP_PADDING_RIGHT, 10);
	_layout_setprop(hLayout, ELP_PADDING_BOTTOM, 10);

	HEXOBJ hObj1 = Ex_ObjCreateEx(-1, L"Static", L"控件A：父容器的左下角", -1, 0, 0, 200, 150, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj1, COLOR_EX_BACKGROUND, ExARGB(255, 0, 0, 100), TRUE);
	_layout_setchildprop(hLayout, hObj1, ELCP_RELATIVE_LEFT_ALIGN_OF, -1);   //左侧与父容器对齐
	_layout_setchildprop(hLayout, hObj1, ELCP_RELATIVE_BOTTOM_ALIGN_OF, -1); // 底边与父容器对齐

	HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"Static", L"控件B：父容器居中顶部", -1, 0, 0, 200, 150, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
	_layout_setchildprop(hLayout, hObj2, ELCP_RELATIVE_TOP_ALIGN_OF, -1);   //顶部与父容器对齐
	_layout_setchildprop(hLayout, hObj2, ELCP_RELATIVE_CENTER_PARENT_H, 1); // 水平居中于父容器

	HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"Static", L"控件C：右侧与A对齐,宽度150,在A和B之间", -1, 0, 0, 150, 150, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
	_layout_setchildprop(hLayout, hObj3, ELCP_RELATIVE_TOP_OF, hObj1);         //在A控件顶部
	_layout_setchildprop(hLayout, hObj3, ELCP_RELATIVE_BOTTOM_OF, hObj2);      // 在B控件底部
	_layout_setchildprop(hLayout, hObj3, ELCP_RELATIVE_RIGHT_ALIGN_OF, hObj1); // 在B右侧对齐于A控件

	HEXOBJ hObj4 = Ex_ObjCreateEx(-1, L"Static", L"控件D：高度100,在A和父控件右边界之间,在父容器底部", -1, 0, 0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj4, COLOR_EX_BACKGROUND, ExRGB2ARGB(16754943, 100), TRUE);
	_layout_setchildprop(hLayout, hObj4, ELCP_RELATIVE_RIGHT_OF, hObj1);     //在A控件右边
	_layout_setchildprop(hLayout, hObj4, ELCP_RELATIVE_BOTTOM_ALIGN_OF, -1); // 底部对齐于父容器
	_layout_setchildprop(hLayout, hObj4, ELCP_RELATIVE_RIGHT_ALIGN_OF, -1);  // 右侧对齐于父容器

	HEXOBJ hObj5 = Ex_ObjCreateEx(-1, L"Static", L"控件E：与D同宽,在D和B之间", -1, 0, 0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj5, COLOR_EX_BACKGROUND, ExRGB2ARGB(8445952, 100), TRUE);
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_TOP_OF, hObj4);         //在D顶部
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_BOTTOM_OF, hObj2);      // 在B底部
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_LEFT_ALIGN_OF, hObj4);  // 左侧对齐于D
	_layout_setchildprop(hLayout, hObj5, ELCP_RELATIVE_RIGHT_ALIGN_OF, hObj4); // 右侧对齐于D

	HEXOBJ hObj6 = Ex_ObjCreateEx(-1, L"Static", L"控件F：150宽度,垂直方向对齐于DE,右对齐于DE", -1, 0, 0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj6, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 100), TRUE);
	_layout_setchildprop(hLayout, hObj6, ELCP_RELATIVE_TOP_ALIGN_OF, hObj5);    //顶部对齐于E
	_layout_setchildprop(hLayout, hObj6, ELCP_RELATIVE_BOTTOM_ALIGN_OF, hObj4); // 底部对齐于D
	_layout_setchildprop(hLayout, hObj6, ELCP_RELATIVE_RIGHT_ALIGN_OF, hObj4);  // 右对齐于D

	Ex_ObjLayoutSet(hExDui_relative, hLayout, TRUE);

	Ex_DUIShowWindow(hExDui_relative, SW_SHOWNORMAL, 0, 0, 0);
}

void test_linear(HWND hWnd)
{
	HWND hWnd_linear = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试线性布局", 0, 0, 600, 400, 0, 0);
	HEXDUI hExDui_linear = Ex_DUIBindWindowEx(hWnd_linear, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_linear, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_LINEAR, hExDui_linear);
	_layout_setprop(hLayout, ELP_LINEAR_DIRECTION, ELP_DIRECTION_H);       //设置布局方向为水平
	_layout_setprop(hLayout, ELP_LINEAR_DALIGN, ELP_LINEAR_DALIGN_CENTER); // 设置布局方向对齐方式为居中

	HEXOBJ hObj1 = Ex_ObjCreateEx(-1, L"Static", L"容器1", -1, 0, 0, 200, 300, hExDui_linear, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj1, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
	_layout_setchildprop(hLayout, hObj1, ELCP_LINEAR_ALIGN, ELCP_LINEAR_ALIGN_CENTER); //设置居中于父
	_layout_setchildprop(hLayout, hObj1, ELCP_MARGIN_RIGHT, 10);                       // 设置右边10像素间隔

	HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"Static", L"容器2", -1, 0, 0, 400, 200, hExDui_linear, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
	_layout_setchildprop(hLayout, hObj2, ELCP_LINEAR_ALIGN, ELCP_LINEAR_ALIGN_CENTER); //设置居中于父
	_layout_setchildprop(hLayout, hObj2, ELCP_MARGIN_RIGHT, 10);                       // 设置右边10像素间隔

	HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"Static", L"容器3", -1, 0, 0, 100, 100, hExDui_linear, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
	_layout_setchildprop(hLayout, hObj3, ELCP_LINEAR_ALIGN, ELCP_LINEAR_ALIGN_CENTER); //设置居中于父

	Ex_ObjLayoutSet(hExDui_linear, hLayout, TRUE);
	Ex_DUIShowWindow(hExDui_linear, SW_SHOWNORMAL, 0, 0, 0);
}

void test_flow(HWND hWnd)
{
	HWND hWnd_flow = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试流式布局", 0, 0, 600, 400, 0, 0);
	HEXDUI hExDui_flow = Ex_DUIBindWindowEx(hWnd_flow, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_flow, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_FLOW, hExDui_flow);
	_layout_setprop(hLayout, ELP_FLOW_DIRECTION, ELP_DIRECTION_V); //设置布局方向为垂直

	_layout_setprop(hLayout, ELP_PADDING_LEFT, 30);
	_layout_setprop(hLayout, ELP_PADDING_TOP, 30); //设置距离顶边30
	_layout_setprop(hLayout, ELP_PADDING_RIGHT, 30);
	_layout_setprop(hLayout, ELP_PADDING_BOTTOM, 30);

	HEXOBJ hObj = 0;
	for (INT i = 0; i < 20; i++)
	{
		hObj = Ex_ObjCreate(L"Static", L"test", -1, 0, 0, Random(50, 150), Random(50, 150), hExDui_flow);
		Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
		_layout_setchildprop(hLayout, hObj, ELCP_MARGIN_RIGHT, 10);
		_layout_setchildprop(hLayout, hObj, ELCP_MARGIN_BOTTOM, 10);
		if (i % 10 == 0)
		{
			_layout_setchildprop(hLayout, hObj, ELCP_FLOW_NEW_LINE, 1);
		}
	}
	Ex_ObjLayoutSet(hExDui_flow, hLayout, TRUE);

	Ex_DUIShowWindow(hExDui_flow, SW_SHOWNORMAL, 0, 0, 0);
}

void test_table(HWND hWnd)
{
	HWND hWnd_table = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试表格布局", 0, 0, 400, 400, 0, 0);
	HEXDUI hExDui_table = Ex_DUIBindWindowEx(hWnd_table, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MAX | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_table, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXLAYOUT hLayout = _layout_create(ELT_TABLE, hExDui_table);

	_layout_setprop(hLayout, ELP_PADDING_LEFT, 10);
	_layout_setprop(hLayout, ELP_PADDING_TOP, 30);
	_layout_setprop(hLayout, ELP_PADDING_RIGHT, 10);
	_layout_setprop(hLayout, ELP_PADDING_BOTTOM, 10);
	INT row[4] = { 50, -30, 75, -20 }; //4行,正数为像素,负数为百分比
	INT cell[3] = { 100, 75, -50 };    //3列,正数为像素,负数为百分比
	HEXOBJ hObj = 0;
	_layout_table_setinfo(hLayout, row, 4, cell, 3);
	for (INT i = 1; i <= 4; i++)
	{
		for (INT j = 1; j <= 3; j++)
		{
			auto str = std::to_wstring(i) + L"," + std::to_wstring(j);
			hObj = Ex_ObjCreateEx(-1, L"static", (LPCWSTR)str.c_str(), -1, 0, 0, 200, 150, hExDui_table, 0, DT_VCENTER, 0, 0, 0);
			Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), FALSE);
			_layout_setchildprop(hLayout, hObj, ELCP_TABLE_ROW, i);  // 设置所属行
			_layout_setchildprop(hLayout, hObj, ELCP_TABLE_CELL, j); //设置所属列
		}
	}
	HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"static", L"(2,1)[占2行]", -1, 0, 0, 200, 150, hExDui_table, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(65535, 150), TRUE);
	_layout_setchildprop(hLayout, hObj2, ELCP_TABLE_CELL, 2);
	_layout_setchildprop(hLayout, hObj2, ELCP_TABLE_ROW, 1);
	_layout_setchildprop(hLayout, hObj2, ELCP_TABLE_ROW_SPAN, 2); //设置跨行数

	HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"static", L"(1,3)[占3列2行]", -1, 0, 0, 200, 150, hExDui_table, 0, DT_VCENTER, 0, 0, 0);
	Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711935, 120), TRUE);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_CELL, 1);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_ROW, 3);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_ROW_SPAN, 2);
	_layout_setchildprop(hLayout, hObj3, ELCP_TABLE_CELL_SPAN, 3); //设置跨列数

	Ex_ObjLayoutSet(hExDui_table, hLayout, TRUE);

	Ex_DUIShowWindow(hExDui_table, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<HEXOBJ> m_hComboBoxButton(7);
HEXOBJ m_hComboBox;

LRESULT CALLBACK OnComboBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (hObj == m_hComboBoxButton[0])
	{
		size_t count = Ex_ObjSendMessage(m_hComboBox, CB_GETCOUNT, 0, 0);
		std::wstring text = L"测试添加" + std::to_wstring(count + 1);
		Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (LPARAM)text.c_str());
	}
	else if (hObj == m_hComboBoxButton[1])
	{
		std::wstring text = L"插入项目";
		Ex_ObjSendMessage(m_hComboBox, CB_INSERTSTRING, 2, (LPARAM)text.c_str());
	}
	else if (hObj == m_hComboBoxButton[2])
	{
		size_t len = Ex_ObjGetTextLength(m_hComboBox) * 2 + 2;
		std::wstring text;
		text.resize(len);
		Ex_ObjGetText(m_hComboBox, text.c_str(), len);
		OUTPUTW(text);
	}
	else if (hObj == m_hComboBoxButton[3])
	{
		std::wstring text = L"置内容";
		Ex_ObjSetText(m_hComboBox, text.c_str(), TRUE);
	}
	else if (hObj == m_hComboBoxButton[4])
	{
		Ex_ObjSendMessage(m_hComboBox, CB_RESETCONTENT, 0, 0);
	}
	else if (hObj == m_hComboBoxButton[5])
	{
		Ex_ObjSendMessage(m_hComboBox, CB_SHOWDROPDOWN, TRUE, 0);
	}
	else if (hObj == m_hComboBoxButton[6])
	{
		Ex_ObjSendMessage(m_hComboBox, CB_DELETESTRING, 2, 0);
	}
	return 0;
}

void test_combobox(HWND hWnd)
{
	HWND hWnd_combobox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试组合框", 0, 0, 450, 300, 0, 0);
	HEXDUI hExDui_combobox = Ex_DUIBindWindowEx(hWnd_combobox, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, 0);
	Ex_DUISetLong(hExDui_combobox, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	m_hComboBox = Ex_ObjCreateEx(-1, L"combobox", L"测试组合框", EOS_VISIBLE | ECS_ALLOWEDIT, 10, 30, 200, 30, hExDui_combobox, 0, DT_VCENTER, 0, 0, NULL);
	Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"英文字母abc");
	Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"数字123");
	Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"中文");
	Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"特殊字符[！（）");

	m_hComboBoxButton[0] = Ex_ObjCreate(L"button", L"添加项目", -1, 230, 30, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[0], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[1] = Ex_ObjCreate(L"button", L"插入项目", -1, 230, 70, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[1], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[2] = Ex_ObjCreate(L"button", L"取内容", -1, 230, 110, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[2], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[3] = Ex_ObjCreate(L"button", L"置内容", -1, 340, 30, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[3], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[4] = Ex_ObjCreate(L"button", L"清空表项", -1, 340, 70, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[4], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[5] = Ex_ObjCreate(L"button", L"弹出列表", -1, 340, 110, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[5], NM_CLICK, OnComboBoxButtonEvent);

	m_hComboBoxButton[6] = Ex_ObjCreate(L"button", L"删除项目", -1, 230, 150, 100, 30, hExDui_combobox);
	Ex_ObjHandleEvent(m_hComboBoxButton[6], NM_CLICK, OnComboBoxButtonEvent);

	Ex_DUIShowWindow(hExDui_combobox, SW_SHOWNORMAL, 0, 0, 0);
}

HWND m_hWndAni = 0;
HEXDUI m_hExDuiAni = 0;
HEXEASING m_hEasing = nullptr;

LRESULT CALLBACK OnAniWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_CLOSE)
	{
		AniShow(FALSE);
	}
	return 0;
}

size_t CALLBACK OnAniEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
{
	INT index = nCurrent * 255;
	Ex_DUISetLong(m_hExDuiAni, EWL_CRBKG, ExARGB(150, 150, 150, index));
	Ex_DUISetLong(m_hExDuiAni, EWL_ALPHA, index);
	SetWindowPos((HWND)Ex_DUIGetLong(m_hExDuiAni, EWL_HWND), 0, p1 + p2 * nCurrent, p3 + p4 * nCurrent, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	return 0;
}

void AniShow(BOOL fShow)
{
	Ex_DUISetLong(m_hExDuiAni, EWL_CRBKG, ExARGB(150, 150, 150, fShow ? 0 : 255));
	Ex_DUISetLong(m_hExDuiAni, EWL_ALPHA, fShow ? 0 : 255);
	Ex_DUIShowWindow(m_hExDuiAni, SW_SHOW, 0, 0, 0);

	RECT rc{ 0 };
	GetWindowRect((HWND)Ex_DUIGetLong(m_hExDuiAni, EWL_HWND), &rc);
	_easing_create(ET_InOutQuint, 0, ES_SINGLE | ES_CALLFUNCTION | (fShow ? 0 : ES_REVERSE), (size_t)OnAniEasing, 500, 20, EES_PLAY, 0, 1, rc.left, 0, rc.top - 100, 100);
	Ex_DUIShowWindow(m_hExDuiAni, fShow ? SW_SHOW : SW_HIDE, 0, 0, 0);
}

size_t CALLBACK OnBtnEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
{
	Ex_ObjSetPos(p1, 0, 0, 0, nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
	return 0;
}

size_t CALLBACK OnBtnEasing4(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4)
{
	RECT rc{ 0 };
	GetWindowRect(m_hWndAni, &rc);
	MoveWindow(m_hWndAni, rc.left, rc.top, nCurrent, rc.bottom - rc.top, TRUE);
	return 0;
}

LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 10001)
	{
		if (nCode == NM_CLICK)
		{
			_easing_create(ET_InOutCubic, 0, ES_SINGLE | ES_THREAD | ES_CALLFUNCTION, (size_t)OnBtnEasing, 200, 20, EES_PLAY, 150, 300, hObj, 0, 0, 0);
		}
	}
	else if (nID == 10002)
	{
		if (nCode == NM_CLICK)
		{
			_easing_create(ET_InOutCubic, 0, MAKELONG(ES_MANYTIMES | ES_BACKANDFORTH | ES_THREAD | ES_DISPATCHNOTIFY, 10 * 2), hObj, 150, 20, EES_PLAY, 150, 300, 0, 0, 0, 0);
		}
		else if (nCode == NM_EASING)
		{
			EX_EASINGINFO pEasingInfo{ 0 };
			RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
			Ex_ObjSetPos(hObj, 0, 0, 0, pEasingInfo.nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
		}
	}
	else if (nID == 10003)
	{
		if (nCode == NM_CLICK)
		{
			if (_easing_getstate(m_hEasing) == EES_PAUSE)
			{
				_easing_setstate(m_hEasing, EES_PLAY);
			}
			else
			{
				_easing_setstate(m_hEasing, EES_PAUSE);
			}
		}
		else if (nCode == NM_DESTROY)
		{
			_easing_setstate(m_hEasing, EES_STOP);
		}
		else if (nCode == NM_EASING)
		{
			EX_EASINGINFO pEasingInfo{ 0 };
			RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
			Ex_ObjSetPos(hObj, 0, 0, 0, pEasingInfo.nCurrent, 50, SWP_NOMOVE | SWP_NOZORDER);
		}
	}
	else if (nID == 10004)
	{
		if (nCode == NM_CLICK)
		{
			_easing_create(ET_InOutCirc, 0, MAKELONG(ES_MANYTIMES | ES_BACKANDFORTH | ES_CALLFUNCTION, 4), (size_t)OnBtnEasing4, 400, 25, 0, 400, 150, 0, 0, 0, 0);
		}
	}
	return 0;
}

void test_ani(HWND hWnd)
{
	m_hWndAni = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试缓动窗口", 0, 0, 400, 300, 0, 0);
	m_hExDuiAni = Ex_DUIBindWindowEx(m_hWndAni, 0, EWS_NOINHERITBKG | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_SIZEABLE | EWS_TITLE | EWS_HASICON | EWS_NOSHADOW, 0, OnAniWndMsgProc);
	HEXOBJ hObj_button1 = Ex_ObjCreateEx(-1, L"button", L"点击就动1次", -1, 10, 40, 120, 50, m_hExDuiAni, 10001, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	HEXOBJ hObj_button2 = Ex_ObjCreateEx(-1, L"button", L"来回", -1, 10, 100, 120, 50, m_hExDuiAni, 10002, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	HEXOBJ hObj_button3 = Ex_ObjCreateEx(-1, L"button", L"点击动/停", -1, 10, 160, 120, 50, m_hExDuiAni, 10003, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	HEXOBJ hObj_button4 = Ex_ObjCreateEx(-1, L"button", L"点击窗口动", -1, 10, 220, 120, 50, m_hExDuiAni, 10004, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(hObj_button1, NM_CLICK, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj_button2, NM_CLICK, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj_button2, NM_EASING, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj_button3, NM_CLICK, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj_button3, NM_EASING, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj_button3, NM_DESTROY, OnAniButtonEvent);
	Ex_ObjHandleEvent(hObj_button4, NM_CLICK, OnAniButtonEvent);
	m_hEasing = _easing_create(ET_InOutCirc, 0, ES_CYCLE | ES_BACKANDFORTH | ES_THREAD | ES_DISPATCHNOTIFY, hObj_button3, 200, 20, EES_PAUSE, 150, 300, 0, 0, 0, 0);
	AniShow(TRUE);
}

LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND) //wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
	{
		_canvas_setantialias(wParam, TRUE);
		RECT rc{ 0 };
		Ex_DUIGetClientRect(hExDui, &rc);
		constexpr EX_STOPPTS arrStopPts[]{
			{0.0,ExRGBA(10, 127, 213, 220)},
			{1.0, ExRGBA(200, 10, 10, 220)}
		};
		HEXBRUSH hBrush = _brush_createlinear_ex(0, 0, rc.right, rc.bottom, arrStopPts, 2);
		_canvas_fillellipse(wParam, hBrush, LOWORD(lParam) / 2, HIWORD(lParam) / 2, LOWORD(lParam) / 2 - 2, HIWORD(lParam) / 2 - 2);
		_brush_destroy(hBrush);
		*lpResult = 1;
		return 1;
	}
	return 0;
}

void test_customredraw(HWND hWnd)
{
	//异型窗口采用重画背景形式，才不会产生锯齿。用于需要圆角，不规则图形的窗口。
	HWND hWnd_customredraw = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"", 0, 0, 300, 200, 0, 0);
	HEXDUI hExDui_customredraw = Ex_DUIBindWindowEx(hWnd_customredraw, 0, EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW, 0, OnCustomRedrawWndMsgProc);
	Ex_DUISetLong(hExDui_customredraw, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_ObjCreateEx(EOS_EX_TOPMOST, L"sysbutton", L"", EOS_VISIBLE | EWS_BUTTON_CLOSE, (300 - 32) / 2, (200 - 32) / 2, 32, 32, hExDui_customredraw, 0, 0, 0, 0, NULL); //改变标题栏按钮位置
	Ex_DUIShowWindow(hExDui_customredraw, SW_SHOWNORMAL, 0, 0, 0);
}

void test_messagebox(HWND hWnd)
{
	HWND hWnd_messagebox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试消息框", 0, 0, 200, 200, 0, 0);
	HEXDUI hExDui_messagebox = Ex_DUIBindWindowEx(hWnd_messagebox, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE, 0, 0);
	Ex_DUISetLong(hExDui_messagebox, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_MessageBox(hExDui_messagebox, L"内容", L"标题", MB_YESNO | MB_ICONQUESTION, EMBF_CENTEWINDOW);
	Ex_DUIShowWindow(hExDui_messagebox, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnColorButtonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEHOVER)
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_SETFOCUS)
	{
		Ex_ObjSetUIState(hObj, STATE_FOCUS, FALSE, 0, TRUE);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		Ex_ObjSetUIState(hObj, STATE_FOCUS, TRUE, 0, TRUE);
	}
	else if (uMsg == WM_SYSCOLORCHANGE)
	{
		if (wParam == COLOR_EX_CBTN_CRBKG_NORMAL)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_NORMAL, lParam);
		}
		else if (wParam == COLOR_EX_CBTN_CRBKG_HOVER)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_HOVER, lParam);
		}
		else if (wParam == COLOR_EX_CBTN_CRBKG_DOWN)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_DOWN, lParam);
		}
		else if (wParam == COLOR_EX_CBTN_CRBKG_FOCUS)
		{
			Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_FOCUS, lParam);
		}
	}
	else if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps))
		{
			INT crText = 0;
			INT crBkg = 0;
			if ((ps.dwState & STATE_DOWN) == STATE_DOWN)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN);
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_DOWN);
			}
			else if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER);
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_HOVER);
			}
			else if ((ps.dwState & STATE_FOCUS) == STATE_FOCUS)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_FOCUS);
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_FOCUS);
			}
			if (crBkg == 0)
			{
				crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_NORMAL);
			}
			if (crText == 0)
			{
				crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
			}
			_canvas_clear(ps.hCanvas, crBkg);
			_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), crText, (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, 0, ps.uWidth, ps.uHeight);
			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void ColorButton2::Create(EXHANDLE handle, INT left, INT top, INT width, INT height, LPCWSTR title, INT style, INT styleEx, INT nID, INT dwTextFormat, LPARAM lParam, MsgPROC lpMsgProc)
{
	mhObj = Ex_ObjCreateEx(styleEx, L"colorbutton", title, style, left, top, width, height, handle, nID, dwTextFormat, lParam, 0, lpMsgProc);
}

void ColorButton2::SetBkgNormalColor(EXARGB color, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_NORMAL, color, redraw);
}

void ColorButton2::SetBkgHoverColor(EXARGB color, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_HOVER, color, redraw);
}

void ColorButton2::SetBkgDownColor(EXARGB color, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_DOWN, color, redraw);
}

void ColorButton2::SetTextHoverColor(EXARGB color, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_FOCUS, color, redraw);
}

void ColorButton2::SetBkgFocusColor(EXARGB color, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetColor(mhObj, COLOR_EX_TEXT_HOVER, color, redraw);
}

void ColorButton2::SetTextDownColor(EXARGB color, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetColor(mhObj, COLOR_EX_TEXT_DOWN, color, redraw);
}

void ColorButton2::SetTextFocusColor(EXARGB color, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetColor(mhObj, COLOR_EX_TEXT_FOCUS, color, redraw);
}

void ColorButton2::SetRadius(FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL redraw)
{
	if (mhObj == 0)
		return;
	Ex_ObjSetRadius(mhObj, topleft, topright, bottomright, bottomleft, redraw);
}

void test_colorbutton(HWND hWnd)
{
	HWND hWnd_colorbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试自定义按钮", 0, 0, 300, 150, 0, 0);
	HEXDUI hExDui_colorbutton = Ex_DUIBindWindowEx(hWnd_colorbutton, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_colorbutton, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_ObjRegister(L"colorbutton", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, DT_SINGLELINE | DT_CENTER | DT_VCENTER, 4 * sizeof(size_t), 0, 0, OnColorButtonProc);

	HEXOBJ hObj_colorbutton1 = Ex_ObjCreateEx(-1, L"colorbutton", L"按钮1", -1, 50, 50, 100, 50, hExDui_colorbutton, 0, -1, 0, 0, NULL);
	Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
	Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_HOVER, ExRGB2ARGB(65535, 255), FALSE);
	Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_DOWN, ExRGB2ARGB(65280, 255), FALSE);
	Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_FOCUS, ExRGB2ARGB(16777680, 255), FALSE);

	Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(255, 255), FALSE);
	Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16754943, 255), FALSE);
	Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_TEXT_FOCUS, ExRGB2ARGB(65280, 255), TRUE);

	Ex_ObjSetRadius(hObj_colorbutton1, 25, 25, 25, 25, TRUE);

	//下面演示类封装组件
	ColorButton2 button;
	button.Create(hExDui_colorbutton, 160, 50, 100, 50, L"按钮2");
	button.SetBkgNormalColor(ExRGB2ARGB(16777215, 255), FALSE);
	button.SetBkgHoverColor(ExRGB2ARGB(65535, 255), FALSE);
	button.SetBkgDownColor(ExRGB2ARGB(65280, 255), FALSE);
	button.SetBkgFocusColor(ExRGB2ARGB(16777680, 255), FALSE);
	button.SetTextHoverColor(ExRGB2ARGB(255, 255), FALSE);
	button.SetTextDownColor(ExRGB2ARGB(16754943, 255), FALSE);
	button.SetTextFocusColor(ExRGB2ARGB(65280, 255), TRUE);
	button.SetRadius(25, 25, 25, 25, TRUE);
	Ex_DUIShowWindow(hExDui_colorbutton, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hReportListView;
HEXIMAGELIST m_hReportListViewImgList;

LRESULT CALLBACK OnReportListViewItemChecked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == RLVN_CHECK)
	{
		auto str = L"第" + std::to_wstring(wParam) + L"项选择框状态发生变化,选中状态：" + std::to_wstring(lParam);
		OUTPUTW(str);
	}
	return 0;
}

LRESULT CALLBACK OnReportListViewItemChange(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == LVN_ITEMCHANGED)
	{
		auto str = L"你选择了第" + std::to_wstring(wParam) + L"项";
		OUTPUTW(str);
	}
	return 0;
}

LRESULT CALLBACK OnReportListViewColumnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == RLVN_COLUMNCLICK)
	{
		auto str = L"你点击了第" + std::to_wstring(wParam) + L"列表头";
		OUTPUTW(str);
	}
	return 0;
}

LRESULT CALLBACK OnReportListViewButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	Ex_ObjSendMessage(m_hReportListView, LVM_DELETECOLUMN, 1, 2);
	return 0;
}


void test_reportlistview(HWND hWnd)
{
	HWND hWnd_reportlistview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试报表列表", 0, 0, 400, 400, 0, 0);
	HEXDUI hExDui_reportlistview = Ex_DUIBindWindowEx(hWnd_reportlistview, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_reportlistview, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	m_hReportListView = Ex_ObjCreateEx(-1, L"ReportListView", L"ReportListView", EOS_BORDER | EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL | ERLS_EDIT | ERLS_DRAWVERTICALLINE, 25, 50, 350, 250, hExDui_reportlistview, 0, -1, 0, 0, NULL);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 100), FALSE);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_BORDER, ExRGBA(120, 120, 120, 255), FALSE);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_RLV_HEAD, ExRGB2ARGB(16777215, 250), FALSE);
	Ex_ObjSetColor(m_hReportListView, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(12632256, 50), FALSE);

	m_hReportListViewImgList = _imglist_create(30, 30);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"./icon/1.png", &imgdata);
	_imglist_add(m_hReportListViewImgList, imgdata.data(), imgdata.size(), 0);
	_imglist_add(m_hReportListViewImgList, imgdata.data(), imgdata.size(), 0);
	_imglist_add(m_hReportListViewImgList, imgdata.data(), imgdata.size(), 0);
	_imglist_add(m_hReportListViewImgList, imgdata.data(), imgdata.size(), 0);
	Ex_ObjSendMessage(m_hReportListView, LVM_SETIMAGELIST, 0, (LPARAM)m_hReportListViewImgList);

	EX_REPORTLIST_COLUMNINFO col = { 0 };
	col.pwzText = L"第一列";
	col.nWidth = 75;
	col.crText = ExRGB2ARGB(255, 255);
	col.dwStyle = 0;
	col.dwTextFormat = DT_LEFT;
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	col.pwzText = L"固定列宽";
	col.nWidth = 110;
	col.crText = ExRGB2ARGB(16711680, 255);
	col.dwStyle = ERLV_CS_LOCKWIDTH;
	col.dwTextFormat = DT_LEFT;
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	col.pwzText = L"居中可点击";
	col.nWidth = 100;
	col.crText = ExRGB2ARGB(65535, 255);
	col.dwStyle = ERLV_CS_CLICKABLE | ERLV_CS_COLOUR;
	col.dwTextFormat = DT_CENTER | DT_VCENTER;
	col.crBkg = ExRGBA(120, 230, 180, 255);
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	col.pwzText = L"可排序";
	col.nWidth = 60;
	col.crText = ExRGB2ARGB(16777215, 255);
	col.dwStyle = ERLV_CS_CLICKABLE | ERLV_CS_SORTABLE;
	col.dwTextFormat = DT_RIGHT | DT_VCENTER;
	Ex_ObjSendMessage(m_hReportListView, LVM_INSERTCOLUMN, 0, (size_t)&col);

	EX_REPORTLIST_ROWINFO row = { 0 };
	EX_REPORTLIST_ITEMINFO item = { 0 };
	EX_REPORTLIST_CELLINFO cell = { 0 };
	for (INT i = 1; i <= 10000; i++)
	{
		//先插入表项
		row.lParam = i + 1;
		item.nImageIndex = i;
		// 下面这句控制项目是否带选择框及整行背景色
		item.dwStyle = (i % 3 == 0 ? ERLV_RS_CHECKBOX | ERLV_RS_CHECKBOX_CHECK | ERLV_RS_ROWCOLOUR : 0);
		item.rowBkgCr = ExRGBA(31, 100, 200, 255);
		item.iRow = Ex_ObjSendMessage(m_hReportListView, LVM_INSERTITEM, 0, (size_t)&row);
		//先插入表项
		Ex_ObjSendMessage(m_hReportListView, LVM_SETITEM, 0, (size_t)&item); //wParam为是否立即更新
		cell.iCol = 1;
		cell.iRow = i;
		std::wstring wstr = L"第" + std::to_wstring(i) + L"项";
		cell.pwzText = wstr.c_str();
		cell.cellStyle = 0;
		Ex_ObjSendMessage(m_hReportListView, LVM_SETCELL, 0, (size_t)&cell); //wParam为是否立即更新

		cell.iCol = 2;
		cell.iRow = i;
		cell.pwzText = L"第二列";
		cell.cellStyle = ERLV_RS_CELLCOLOUR;
		cell.cellBkgCr = ExRGBA(130, 130, 25, 255);
		Ex_ObjSendMessage(m_hReportListView, LVM_SETCELL, 0, (size_t)&cell); //wParam为是否立即更新

		cell.iCol = 3;
		cell.iRow = i;
		cell.pwzText = L"第三列";
		cell.cellStyle = ERLV_RS_CELLTEXTCOLOUR;
		cell.cellTextCr = ExRGBA(130, 25, 130, 255);
		Ex_ObjSendMessage(m_hReportListView, LVM_SETCELL, 0, (size_t)&cell); //wParam为是否立即更新

		cell.iCol = 4;
		cell.iRow = i;
		auto str = std::to_wstring(Random(0, 1000));
		cell.pwzText = str.c_str();
		cell.cellStyle = ERLV_RS_CELLFONT;
		cell.cellFont = _font_createfromfamily(L"微软雅黑", 20, 0);
		Ex_ObjSendMessage(m_hReportListView, LVM_SETCELL, 0, (size_t)&cell); //wParam为是否立即更新
	}
	Ex_ObjSendMessage(m_hReportListView, LVM_UPDATE, 0, 0); //整体更新,以加快绘制速度
	Ex_ObjHandleEvent(m_hReportListView, LVN_ITEMCHANGED, OnReportListViewItemChange);
	Ex_ObjHandleEvent(m_hReportListView, RLVN_COLUMNCLICK, OnReportListViewColumnClick);
	Ex_ObjHandleEvent(m_hReportListView, RLVN_CHECK, OnReportListViewItemChecked);

	HEXOBJ hObj_button = Ex_ObjCreate(L"button", L"删除列", -1, 20, 330, 100, 30, hExDui_reportlistview);
	Ex_ObjHandleEvent(hObj_button, NM_CLICK, OnReportListViewButtonEvent);
	Ex_DUIShowWindow(hExDui_reportlistview, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hListViewIcon = 0;
HEXIMAGELIST m_hImageListIcon = 0;

LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE)
	{
		HDC dc = GetDC(NULL);
		FLOAT dpix = (FLOAT)GetDeviceCaps(dc, 88) / 96;
		ReleaseDC(0, dc);
		//因为LOWORD(lParam)是DPI缩放后的窗口坐标,而Ex_ObjMove接受缩放前坐标，因此这里需要除以dpix
		Ex_ObjMove(m_hListViewIcon, 25, 50, (LOWORD(lParam) - 50) / dpix, (HIWORD(lParam) - 75) / dpix, TRUE);
	}
	else if (uMsg == WM_DESTROY)
	{
		_imglist_destroy(m_hImageListIcon);
		m_hListViewIcon = 0; //重置0,不然再次创建会有问题。
		m_hImageListIcon = 0;
	}
	return 0;
}

void test_iconlistview(HWND hWnd)
{
	HWND hWnd_iconlistview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试图标列表", 0, 0, 500, 300, 0, 0);
	HEXDUI hExDui_iconlistview = Ex_DUIBindWindowEx(hWnd_iconlistview, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, OnIconWndMsgProc);

	m_hListViewIcon = Ex_ObjCreateEx(-1, L"iconlistview", L"iconlistview", EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL | EILVS_BUTTON, 25, 50, 400, 225, hExDui_iconlistview, 0, -1, 0, 0, NULL);

	Ex_ObjSendMessage(m_hListViewIcon, ILVM_SETITEMSIZE, 0, MAKELONG(70, 75)); //设置表项尺寸为70,75
	//创建添加图片组信息

	m_hImageListIcon = _imglist_create(36, 36);
	std::vector<CHAR> imgdata;
	for (INT i = 1; i <= 3; i++)
	{
		auto str = L"./icon/" + std::to_wstring(i) + L".png";
		Ex_ReadFile(str.c_str(), &imgdata);
		_imglist_add(m_hImageListIcon, imgdata.data(), imgdata.size(), 0);
	}
	//设置列表的图片组
	Ex_ObjSendMessage(m_hListViewIcon, LVM_SETIMAGELIST, 0, (size_t)m_hImageListIcon);
	EX_ICONLISTVIEW_ITEMINFO ilvi{ 0 };

	for (INT i = 1; i <= 1000; i++)
	{
		ilvi.nIndex = i;
		auto str = L"第" + std::to_wstring(i) + L"项";
		ilvi.pwzText = str.c_str();
		ilvi.nImageIndex = i % 3;
		if (ilvi.nImageIndex == 0)
			ilvi.nImageIndex = 3;
		Ex_ObjSendMessage(m_hListViewIcon, LVM_INSERTITEM, 0, (size_t)&ilvi);
	}
	Ex_ObjSendMessage(m_hListViewIcon, LVM_UPDATE, 0, 0);

	Ex_DUISetLong(hExDui_iconlistview, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_DUIShowWindow(hExDui_iconlistview, SW_SHOWNORMAL, 0, 0, 0);
}

void test_treelistview(HWND hWnd)
{
	HWND hWnd_treelistview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试树形列表", 0, 0, 350, 350, 0, 0);
	HEXDUI hExDui_treelistview = Ex_DUIBindWindowEx(hWnd_treelistview, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_treelistview, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ hObj_treeview = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"treeview", L"fasdf", EOS_BORDER | EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL, 50, 50, 250, 250, hExDui_treelistview, 0, DT_VCENTER, 0, 0, NULL);
	EX_TREEVIEW_INSERTINFO ti = { 0 };

	Ex_ObjSetColor(hObj_treeview, COLOR_EX_BACKGROUND, ExARGB(255, 255, 255, 125), FALSE);
	Ex_ObjSetColor(hObj_treeview, COLOR_EX_BORDER, ExARGB(255, 255, 255, 255), TRUE);
	ti.fExpand = TRUE;
	ti.pwzText = L"节点1";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点2";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点3";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点3-1";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点3-2";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点3-3";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点3-3-1";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点3-3-2";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.itemParent = 0;
	ti.pwzText = L"节点4";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-1";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2-1";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2-2";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2-2-1";
	Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2-2-2";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2-2-2-1";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2-2-2-1-1";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	ti.pwzText = L"节点5-2-2-2-1-1-1";
	ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(hObj_treeview, TVM_INSERTITEM, 0, (size_t)&ti);

	Ex_ObjSendMessage(hObj_treeview, TVM_UPDATE, 0, 0);

	Ex_DUIShowWindow(hExDui_treelistview, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT ps{ 0 };
		Ex_ObjBeginPaint(hObj, &ps);
		_canvas_clear(ps.hCanvas, ExRGB2ARGB(16777215, 100));
		DOUBLE nCurrent = (DOUBLE)Ex_ObjGetLong(hObj, EOL_LPARAM) / 100;

		HEXMATRIX mx = _matrix_create();

		_matrix_translate(mx, (FLOAT)ps.uWidth / 2, (FLOAT)ps.uHeight / 2);
		_matrix_rotate(mx, (FLOAT)nCurrent * 90);
		_matrix_scale(mx, 1 + (FLOAT)nCurrent * 0.25, 1 + (FLOAT)nCurrent * 0.25);
		_matrix_translate(mx, -(FLOAT)ps.uWidth / 2, -(FLOAT)ps.uHeight / 2);
		_canvas_settransform(ps.hCanvas, mx);

		HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));
		output(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));
		_canvas_fillellipse(ps.hCanvas, hBrush, (FLOAT)ps.uWidth / 2, (FLOAT)ps.uHeight / 2, 75.f, 50.f);
		_brush_destroy(hBrush);

		_canvas_settransform(ps.hCanvas, 0);
		_matrix_destroy(mx);
		Ex_ObjEndPaint(hObj, &ps);
		*lpResult = 1;
		return 1;
	}
	else if (uMsg == WM_EX_EASING)
	{
		EX_EASINGINFO es{ 0 };
		RtlMoveMemory(&es, (void*)lParam, sizeof(EX_EASINGINFO));
		if (es.nProgress == 1) //如果进度=1则表示缓动结束
		{
			Ex_ObjSetLong(hObj, EOL_USERDATA, 0); //设置控件当前缓动指针为空
		}
		Ex_ObjSetLong(hObj, EOL_LPARAM, es.nCurrent);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		HEXEASING hEasing = _easing_create(ET_OutElastic, 0, ES_SINGLE | ES_THREAD | ES_DISPATCHNOTIFY, hObj, 500, 20, EES_PLAY, 0, 100, 0, 0, 0, 0);
		HEXEASING oldhEasing = (HEXEASING)Ex_ObjSetLong(hObj, EOL_USERDATA, (size_t)hEasing);
		if (oldhEasing != 0) //如果前一个缓动未结束,则停止前面的缓动
		{
			_easing_setstate(oldhEasing, EES_STOP);
		}
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, FALSE); //设置悬浮状态
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		HEXEASING hEasing = _easing_create(ET_OutElastic, 0, ES_SINGLE | ES_THREAD | ES_DISPATCHNOTIFY | ES_REVERSE, hObj, 500, 20, EES_PLAY, 0, 100, 0, 0, 0, 0);
		HEXEASING oldhEasing = (HEXEASING)Ex_ObjSetLong(hObj, EOL_USERDATA, (size_t)hEasing);
		if (oldhEasing != 0) //如果前一个缓动未结束,则停止前面的缓动
		{
			_easing_setstate(oldhEasing, EES_STOP);
		}
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, FALSE); //删除悬浮状态
	}
	return 0;
}

void test_matrix(HWND hWnd)
{
	HWND hWnd_matrix = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试矩阵", 0, 0, 400, 400, 0, 0);
	HEXDUI hExDui_matrix = Ex_DUIBindWindowEx(hWnd_matrix, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_matrix, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"static", 0, -1, 50, 50, 200, 250, hExDui_matrix, 0, DT_VCENTER, 0, 0, OnMatrixMsgProc);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 255), TRUE);

	Ex_DUIShowWindow(hExDui_matrix, SW_SHOWNORMAL, 0, 0, 0);
}

void test_buttonex(HWND hWnd)
{
	HWND hWnd_buttonex = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展按钮", 0, 0, 300, 300, 0, 0);
	HEXDUI hExDui_buttonex = Ex_DUIBindWindowEx(hWnd_buttonex, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_buttonex, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ hObj_btnex1 = Ex_ObjCreate(L"ButtonEx", L"☏直角纯色按钮", -1, 50, 50, 100, 30, hExDui_buttonex);
	EXARGB ThemeColor = RGB(76, 175, 80);
	EX_OBJ_PROPS ButtonExprops = { 0 };
	ButtonExprops.crBkgNormal = ExRGB2ARGB(ThemeColor, 225);
	ButtonExprops.crBkgHover = ExRGB2ARGB(ThemeColor, 250);
	ButtonExprops.crBkgDownOrChecked = ExRGB2ARGB(ThemeColor, 200);
	Ex_ObjSendMessage(hObj_btnex1, WM_EX_PROPS, 0, (size_t)&ButtonExprops);

	HEXOBJ hObj_btnex2 = Ex_ObjCreate(L"ButtonEx", L"点击进入 👉", -1, 50, 100, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops2 = { 0 };
	ButtonExprops2.crBkgNormal = ExRGB2ARGB(7037666, 255);
	ButtonExprops2.crBkgHover = ExRGB2ARGB(6182117, 255);
	ButtonExprops2.crBkgDownOrChecked = ExRGB2ARGB(4865258, 255);
	ButtonExprops2.radius = 15;
	Ex_ObjSendMessage(hObj_btnex2, WM_EX_PROPS, 0, (size_t)&ButtonExprops2);

	HEXOBJ hObj_btnex3 = Ex_ObjCreate(L"ButtonEx", L"Metro按钮☪", -1, 50, 150, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops3 = { 0 };
	ButtonExprops3.crBkgNormal = ExARGB(130, 130, 130, 255);
	ButtonExprops3.crBkgHover = ExARGB(130, 130, 130, 235);
	ButtonExprops3.crBkgDownOrChecked = ExARGB(200, 200, 200, 100);
	ButtonExprops3.crBorderNormal = ExARGB(130, 130, 130, 255);
	ButtonExprops3.crBorderHover = ExARGB(0, 0, 0, 135);
	ButtonExprops3.crBorderDownOrChecked = ExARGB(0, 0, 0, 150);
	ButtonExprops3.strokeWidth = 2;
	Ex_ObjSendMessage(hObj_btnex3, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops3);

	HEXOBJ hObj_btnex4 = Ex_ObjCreate(L"ButtonEx", L"图标在左", -1, 50, 200, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops4 = { 0 };
	ButtonExprops4.crBkgNormal = ExRGB2ARGB(10066176, 255);
	ButtonExprops4.crBkgHover = ExRGB2ARGB(10066176, 220);
	ButtonExprops4.crBkgDownOrChecked = ExRGB2ARGB(10066176, 200);
	Ex_ObjSendMessage(hObj_btnex4, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops4);
	Ex_ObjSetFontFromFamily(hObj_btnex4, L"楷体", 16, -1, TRUE);
	HEXIMAGE hImg;
	_img_createfromfile(L"buttonex\\5.png", &hImg);
	Ex_ObjSendMessage(hObj_btnex4, WM_SETICON, 0, (LPARAM)hImg); /* 设置图标; */

	EX_IMAGEINFO IMG0 = { 0 };
	_img_createfromfile(L"buttonex/4正常.png", &IMG0.imgNormal);
	_img_createfromfile(L"buttonex/4点燃.png", &IMG0.imgHover);
	_img_createfromfile(L"buttonex/4按下.png", &IMG0.imgDownOrChecked);
	HEXOBJ hObj_btnex5 = Ex_ObjCreate(L"ButtonEx", NULL, -1, 50, 250, 100, 30, hExDui_buttonex); /*图片按钮*/
	Ex_ObjSendMessage(hObj_btnex5, BM_SETIMAGE, 0, (LPARAM)&IMG0);

	HEXOBJ hObj_btnex6 = Ex_ObjCreate(L"ButtonEx", L"背景按钮", -1, 180, 50, 100, 30, hExDui_buttonex);
	Ex_ObjSetColor(hObj_btnex6, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(65535, 180), FALSE);
	Ex_ObjSetColor(hObj_btnex6, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(65535, 255), FALSE);
	Ex_ObjSetColor(hObj_btnex6, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(65535, 255), FALSE);
	EX_IMAGEINFO IMG = { 0 };
	_img_createfromfile(L"buttonex/正常.png", &IMG.imgNormal);
	_img_createfromfile(L"buttonex/进入.png", &IMG.imgHover);
	_img_createfromfile(L"buttonex/按下.png", &IMG.imgDownOrChecked);
	Ex_ObjSendMessage(hObj_btnex6, BM_SETIMAGE, 0, (LPARAM)&IMG);

	HEXOBJ hObj_btnex7 = Ex_ObjCreate(L"ButtonEx", L"渐变按钮🔊", -1, 180, 100, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops7 = { 0 };
	ButtonExprops7.crBkgBegin = ExARGB(0, 173, 241, 255);
	ButtonExprops7.crBkgEnd = ExARGB(100, 25, 129, 255);
	ButtonExprops7.strokeWidth = 2;
	Ex_ObjSendMessage(hObj_btnex7, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops7);
	Ex_ObjSetFontFromFamily(hObj_btnex7, L"楷体", 16, -1, TRUE);

	HEXOBJ hObj_btnex8 = Ex_ObjCreate(L"ButtonEx", L"线框按钮", -1, 180, 150, 100, 30, hExDui_buttonex); /*图标按钮*/
	EX_OBJ_PROPS ButtonExprops8 = { 0 };
	ButtonExprops8.crBorderBegin = ExARGB(227, 34, 103, 255);
	ButtonExprops8.crBorderEnd = ExRGB2ARGB(16746496, 255);
	ButtonExprops8.strokeWidth = 2;
	Ex_ObjSendMessage(hObj_btnex8, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops8);
	Ex_ObjSetColor(hObj_btnex8, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(65535, 180), FALSE);
	Ex_ObjSetColor(hObj_btnex8, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(65535, 255), FALSE);
	Ex_ObjSetColor(hObj_btnex8, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(65535, 255), FALSE);
	Ex_ObjSetFontFromFamily(hObj_btnex8, L"楷体", 16, -1, TRUE);

	HEXOBJ hObj_btnex9 = Ex_ObjCreate(L"ButtonEx", L"图标在上", -1, 180, 200, 100, 40, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops9 = { 0 };
	ButtonExprops9.crBkgNormal = ExRGB2ARGB(10061616, 255);
	ButtonExprops9.crBkgHover = ExRGB2ARGB(10061616, 220);
	ButtonExprops9.crBkgDownOrChecked = ExRGB2ARGB(10061616, 200);
	ButtonExprops9.nIconPosition = 2;
	Ex_ObjSendMessage(hObj_btnex9, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops9);
	Ex_ObjSetFontFromFamily(hObj_btnex9, L"楷体", 16, -1, TRUE);
	_img_createfromfile(L"buttonex\\4.png", &hImg);
	Ex_ObjSendMessage(hObj_btnex9, WM_SETICON, 0, (LPARAM)hImg); /* 设置图标; */

	HEXOBJ hObj_btnex10 = Ex_ObjCreate(L"ButtonEx", L"图标在右", -1, 180, 250, 100, 30, hExDui_buttonex);
	EX_OBJ_PROPS ButtonExprops10 = { 0 };
	ButtonExprops10.crBkgNormal = ExARGB(255, 255, 255, 50);
	ButtonExprops10.crBkgHover = ExARGB(255, 255, 255, 80);
	ButtonExprops10.crBkgDownOrChecked = ExARGB(255, 255, 255, 100);
	ButtonExprops10.crBorderNormal = ExARGB(0, 0, 0, 150);
	ButtonExprops10.crBorderHover = ExARGB(0, 0, 0, 180);
	ButtonExprops10.crBorderDownOrChecked = ExARGB(0, 0, 0, 200);
	ButtonExprops10.strokeWidth = 1;
	ButtonExprops10.nIconPosition = 1;
	ButtonExprops10.radius = 7;
	Ex_ObjSendMessage(hObj_btnex10, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops10);
	Ex_ObjSetFontFromFamily(hObj_btnex10, L"楷体", 16, -1, TRUE);
	_img_createfromfile(L"buttonex\\3.png", &hImg);
	Ex_ObjSendMessage(hObj_btnex10, WM_SETICON, 0, (LPARAM)hImg); /* 设置图标; */

	Ex_DUIShowWindow(hExDui_buttonex, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnEditChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == EN_CHANGE)
	{
		OUTPUTW(L"编辑框内容改变", nID);
	}
	else if (nCode == NM_CHAR)
	{
		OUTPUTW(L"编辑框输入字符", nID, wParam);
	}
	return 0;
}

void test_editex(HWND hWnd)
{
	HWND hWnd_editex = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展编辑框", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_editex = Ex_DUIBindWindowEx(hWnd_editex, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_editex, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXIMAGE hImage;
	EX_OBJ_PROPS EditExprops = { 0 };
	HEXOBJ hObj_editex1 = Ex_ObjCreate(L"EditEx", NULL, EOS_VISIBLE | EES_UNDERLINE, 30, 50, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(hObj_editex1, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"请输入账户名称");
	Ex_ObjSetColor(hObj_editex1, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
	_img_createfromfile(L"editex\\icon_contacts_normal.png", &hImage);
	Ex_ObjSendMessage(hObj_editex1, WM_SETICON, 0, (LPARAM)hImage);

	HEXOBJ hObj_editex2 = Ex_ObjCreate(L"EditEx", NULL, EOS_VISIBLE | EES_UNDERLINE | EES_USEPASSWORD, 30, 100, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(hObj_editex2, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"请输入账户密码");
	Ex_ObjSetColor(hObj_editex2, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
	_img_createfromfile(L"editex\\psw_normal.png", &hImage);
	Ex_ObjSendMessage(hObj_editex2, WM_SETICON, 0, (LPARAM)hImage);

	HEXOBJ hObj_editex3 = Ex_ObjCreate(L"EditEx", NULL, -1, 200, 50, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(hObj_editex3, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"搜索一下");
	Ex_ObjSetColor(hObj_editex3, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
	EditExprops.crBkgNormal = ExARGB(20, 20, 120, 150);
	EditExprops.crBorderHover = ExARGB(255, 77, 77, 150);
	EditExprops.crBorderDownOrChecked = ExARGB(255, 77, 77, 200);
	EditExprops.radius = 15;
	EditExprops.strokeWidth = 1;
	EditExprops.nIconPosition = 1;
	Ex_ObjSendMessage(hObj_editex3, WM_EX_PROPS, 0, (LPARAM)&EditExprops);
	_img_createfromfile(L"editex\\search_normal.png", &hImage);
	Ex_ObjSendMessage(hObj_editex3, WM_SETICON, 0, (LPARAM)hImage);

	HEXOBJ hObj_editex4 = Ex_ObjCreate(L"EditEx", NULL, -1, 200, 100, 150, 30, hExDui_editex);
	Ex_ObjSendMessage(hObj_editex4, EM_SETCUEBANNER, ExARGB(255, 255, 255, 100), (LPARAM)L"搜索一下");
	_img_createfromfile(L"editex\\search_normal.png", &hImage);
	Ex_ObjSendMessage(hObj_editex4, WM_SETICON, 0, (LPARAM)hImage); /* 设置图标; */
	Ex_ObjSetColor(hObj_editex4, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);

	Ex_ObjHandleEvent(hObj_editex4, EN_CHANGE, OnEditChangeEvent);
	Ex_ObjHandleEvent(hObj_editex4, NM_CHAR, OnEditChangeEvent);

	Ex_DUIShowWindow(hExDui_editex, SW_SHOWNORMAL, 0, 0, 0);
}

HMENU m_hMenu;

LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT ps{ 0 };
		Ex_ObjBeginPaint(hObj, &ps);
		if ((ps.dwState & STATE_DOWN) != 0)
		{
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 50));
		}
		else if ((ps.dwState & STATE_HOVER) != 0)
		{
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 20));
		}
		else
		{
			_canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 0));
		}
		FLOAT nWidthText = 0;
		FLOAT nHeightText = 0;
		_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, ps.uWidth, ps.uHeight, &nWidthText, &nHeightText);
		HEXIMAGE hImg = Ex_ObjGetLong(hObj, EOL_LPARAM);
		INT nWidthIcon = 0;
		INT nHeightIcon = 0;
		if (hImg != 0)
		{
			_img_getsize(hImg, &nWidthIcon, &nHeightIcon);
		}
		if (hImg != 0)
		{
			_canvas_drawimage(ps.hCanvas, hImg, (ps.uWidth - nWidthIcon) / 2, (ps.uHeight - nHeightIcon - nHeightText - 3) / 2, 255);
		}
		_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat | DT_CENTER | DT_VCENTER, (ps.uWidth - nWidthText) / 2, (ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3, (ps.uWidth + nWidthText) / 2, (ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3 + nHeightText);

		Ex_ObjEndPaint(hObj, &ps);
		*lpResult = 1;
		return 1;
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		auto id = Ex_ObjGetLong(hObj, EOL_ID);
		output(L"菜单按钮点击,id:", id);
		EndMenu();
		*lpResult = 1;
		return 1;
	}

	return 0;
}

LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND)
	{
		if (__get((LPVOID)lParam, 0) == wParam)
		{
			EX_PAINTSTRUCT ps{ 0 };
			RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
			if (ps.uHeight > 10)
			{
				if ((ps.dwState & STATE_HOVER) != 0)
				{
					_canvas_clear(ps.hCanvas, ExRGB2ARGB(16711680, 100));
				}
				else
				{
					_canvas_clear(ps.hCanvas, 0);
				}
				*lpResult = 1;
				return 1;
			}
		}
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		auto id = Ex_ObjGetLong(hObj, EOL_ID);
		output(L"菜单项目点击,id:", id);
	}
	return 0;
}

LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_INITMENUPOPUP)
	{
		RECT rc{ 0 };
		if (wParam == (size_t)m_hMenu) //主菜单
		{
			size_t value = 1;
			SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);
			GetWindowRect(hWnd, &rc);
			SetWindowPos(hWnd, 0, 0, 0, Ex_Scale(rc.right - rc.left + 10), Ex_Scale(rc.bottom - rc.top + 10 + 108), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			rc.right = rc.right - rc.left - Ex_Scale(10);
			rc.bottom = rc.bottom - rc.top - Ex_Scale(10) + Ex_Scale(108);
			rc.left = Ex_Scale(6);
			rc.top = 40;
			//创建顶部按钮
			HEXIMAGE hImg;
			_img_createfromfile(L"custommenu/btn1.png", &hImg);

			Ex_ObjCreateEx(-1, L"button", L"消息", EOS_VISIBLE, rc.left, rc.top, rc.right * 0.333, Ex_Scale(70), hExDUI, 100, -1, hImg, 0, OnMenuBtnMsgProc);

			_img_createfromfile(L"custommenu/btn2.png", &hImg);
			Ex_ObjCreateEx(-1, L"button", L"收藏", EOS_VISIBLE, rc.left + rc.right * 0.333, rc.top, rc.right * 0.333, Ex_Scale(70), hExDUI, 101, -1, hImg, 0, OnMenuBtnMsgProc);

			_img_createfromfile(L"custommenu/btn3.png", &hImg);
			Ex_ObjCreateEx(-1, L"button", L"文件", EOS_VISIBLE, rc.left + rc.right * 0.666, rc.top, rc.right * 0.333, Ex_Scale(70), hExDUI, 102, -1, hImg, 0, OnMenuBtnMsgProc);

			HEXOBJ hObj = Ex_ObjCreateEx(EOS_EX_TRANSPARENT | EOS_EX_TOPMOST, L"Static", 0, EOS_VISIBLE, 0, 0, 45, 38, hExDUI, 0, -1, 0, 0, 0);
			std::vector<CHAR> data;
			Ex_ReadFile(L"custommenu/Icon.png", &data);
			Ex_ObjSetBackgroundImage(hObj, data.data(), data.size(), 0, 0, BIR_NO_REPEAT, 0, 0, 255, TRUE);
			rc.top = rc.top + Ex_Scale(75);
			rc.bottom = rc.bottom - Ex_Scale(75);
		}
		else
		{
			//子菜单
			size_t value = 0;
			SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);
			GetWindowRect(hWnd, &rc);
			SetWindowPos(hWnd, 0, 0, 0, Ex_Scale(rc.right - rc.left + 10), Ex_Scale(rc.bottom - rc.top + 10), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			rc.right = rc.right - rc.left - Ex_Scale(10);
			rc.bottom = rc.bottom - rc.top - Ex_Scale(10);
			rc.left = Ex_Scale(6);
			rc.top = Ex_Scale(8);
		}
		HEXOBJ hObjfind = Ex_ObjFind(hExDUI, 0, L"Item", 0);
		INT t = rc.top;
		RECT rcObj{ 0 };

		while (hObjfind != 0)
		{
			Ex_ObjGetClientRect(hObjfind, &rcObj);
			Ex_ObjMove(hObjfind, rc.left, t, rc.right, rcObj.bottom - rcObj.top, TRUE);
			Ex_ObjSetColor(hObjfind, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);
			Ex_ObjSetLong(hObjfind, EOL_OBJPROC, (size_t)OnMenuItemMsgProc);
			t = t + rcObj.bottom - rcObj.top;
			hObjfind = Ex_ObjGetObj(hObjfind, GW_HWNDNEXT);
		}
	}
	else if (uMsg == WM_ERASEBKGND) //wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
	{
		RECT rc{ 0 };
		HDC dc = GetDC(NULL);
		FLOAT dpix = (FLOAT)GetDeviceCaps(dc, 88) / 96;
		ReleaseDC(0, dc);
		_canvas_clear(wParam, 0);
		HEXIMAGE hImg;
		if (GetPropW(hWnd, L"IsMainMenu") != 0)
		{
			_img_createfromfile(L"custommenu/Main.png", &hImg);
			_canvas_drawimagefromgrid(wParam, hImg, 0, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, 68, 68, 46, 42, 13, 12, 0, 230);
		}
		else
		{
			_img_createfromfile(L"custommenu/Sub.png", &hImg);
			_canvas_drawimagefromgrid(wParam, hImg, 0, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, 24, 24, 8, 9, 10, 10, 0, 230);
		}
		_img_destroy(hImg);
		*lpResult = 1;
		return 1;
	}
	return 0;
}

LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	Ex_TrackPopupMenu(m_hMenu, 0, pt.x, pt.y, 0, hObj, 0, OnMenuWndMsgProc, EMNF_NOSHADOW);
	return 0;
}

void test_custommenu(HWND hWnd)
{
	HWND hWnd_custommenu = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展菜单", 0, 0, 300, 200, 0, 0);
	HEXDUI hExDui_custommenu = Ex_DUIBindWindowEx(hWnd_custommenu, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_custommenu, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ hObj_button = Ex_ObjCreate(L"button", L"弹出菜单", -1, 50, 50, 100, 30, hExDui_custommenu);
	Ex_ObjHandleEvent(hObj_button, NM_CLICK, OnMenuButtonEvent);
	m_hMenu = CreatePopupMenu();
	AppendMenuW(m_hMenu, MF_STRING | MF_ENABLED, 301, L"Item 1");
	CheckMenuItem(m_hMenu, 301, MF_BYCOMMAND | MF_CHECKED);
	AppendMenuW(m_hMenu, MF_STRING | MF_ENABLED, 302, L"Disabled Item");
	AppendMenuW(m_hMenu, MF_SEPARATOR, 666, L"Disabled Item");
	AppendMenuW(m_hMenu, MF_SEPARATOR, 0, L"Disabled Item");
	DeleteMenu(m_hMenu, 666, MF_BYCOMMAND);

	//创建一个新菜单
	HMENU hSubMenu = CreateMenu();
	AppendMenuW(hSubMenu, MF_STRING | MF_ENABLED, 3001, L"SubItem 1"); //添加项目
	AppendMenuW(m_hMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"More");
	EnableMenuItem(m_hMenu, 302, MF_DISABLED);
	Ex_DUIShowWindow(hExDui_custommenu, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnSideButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		if (wParam == 1001) //wParam表示事件对象的ID，1001最里层按钮
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_CLICK)
			{
				std::wstring str;
				str.resize(512);
				Ex_ObjGetText(hObj, str.c_str(), 512);
				auto title = str + L"收到了单击事件";
				Ex_MessageBox(hObj, title.c_str(), 0, 0, 0);
			}
		}
	}
	return 0;
}

LRESULT CALLBACK OnParentButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		if (wParam == 1001) //wParam表示事件对象的ID，1001最里层按钮
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_CLICK)
			{
				if (Ex_MessageBox(hObj, L"按钮直接父控件收到了单击事件，是否拦截？", 0, MB_YESNO, 0) == IDYES) //选择是
				{
					*lpResult = 1; //返回值置1(真)表示拦截
					return 1;      //函数返回值表示是否使用返回值,lpResult才是真正的返回值
				}
			}
		}
	}
	return 0;
}

LRESULT CALLBACK OnEventButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_EX_LCLICK) //左键单击消息,拦截这条则不会触发任何事件
	{
		Ex_MessageBox(hObj, L"按钮收到了单击消息", 0, 0, 0);
	}
	else if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (ni.hObjFrom == hObj)
		{
			if (ni.nCode == NM_CLICK)
			{
				Ex_MessageBox(hObj, L"按钮收到了单击事件", 0, 0, 0);
			}
		}
	}
	return 0;
}

LRESULT CALLBACK OnEventButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CLICK)
	{
		Ex_MessageBox(hObj, L"按钮收到了独立的单击事件", 0, 0, 0);
	}
	return 0;
}

LRESULT CALLBACK OnEventWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		if (wParam == 1001) //wParam表示事件对象的ID，1001最里层按钮
		{
			EX_NMHDR ni{ 0 };
			RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
			if (ni.nCode == NM_CLICK)
			{
				Ex_MessageBox(hExDui, L"按钮所属窗口收到了单击事件", 0, 0, 0);
			}
		}
	}
	return 0;
}

void test_event(HWND hWnd)
{
	HWND hWnd_event = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试事件分发", 0, 0, 400, 300, 0, 0);
	HEXDUI hExDui_event = Ex_DUIBindWindowEx(hWnd_event, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, OnEventWndMsgProc);
	Ex_DUISetLong(hExDui_event, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	/*消息(WM_/XXM_)分发的流程是：
	控件产生消息->控件类回调函数->控件回调函数->默认控件回调函数(在此部分消息会产生事件)
	事件(NM_/XXN_)分发的流程是：
	控件产生事件->JS回调函数->控件自身收到WM_NOTIFY->控件直接父控件收到WM_NOTIFY->控件间接父控件从内而外逐层收到WM_NOTIFY消息(需启用消息冒泡)->所属窗口收到WM_NOTIFY消息
	消息和事件在分发途中均可拦截,HandEvent*/

	HEXOBJ hobj_button_outside = Ex_ObjCreateEx(-1, L"Static", L"按钮外间接父控件", -1, 25, 35, 350, 250, hExDui_event, 0, DT_SINGLELINE, 0, 0, OnSideButtonMsgProc);
	if (hobj_button_outside != 0)
	{
		Ex_ObjSetColor(hobj_button_outside, COLOR_EX_BACKGROUND, ExRGB2ARGB(65535, 100), TRUE);
		HEXOBJ hobj_button_inside = Ex_ObjCreateEx(-1, L"Static", L"按钮内间接父控件", -1, 10, 20, 330, 200, hobj_button_outside, 0, DT_SINGLELINE, 0, 0, OnSideButtonMsgProc);
		if (hobj_button_inside != 0)
		{
			Ex_ObjSetColor(hobj_button_inside, COLOR_EX_BACKGROUND, ExRGBA(100, 100, 100, 100), TRUE);
			HEXOBJ hobj_button_parent = Ex_ObjCreateEx(-1, L"Static", L"按钮直接父控件", -1, 10, 20, 310, 160, hobj_button_inside, 0, DT_SINGLELINE, 0, 0, OnParentButtonMsgProc);
			if (hobj_button_parent != 0)
			{
				Ex_ObjSetColor(hobj_button_parent, COLOR_EX_BACKGROUND, ExRGBA(26, 100, 129, 100), TRUE);
				HEXOBJ hobj_button = Ex_ObjCreateEx(-1, L"button", L"按钮", -1, 10, 20, 200, 100, hobj_button_parent, 1001, -1, 0, 0, OnEventButtonMsgProc);
				if (hobj_button != 0)
				{
					Ex_ObjHandleEvent(hobj_button, NM_CLICK, OnEventButtonEvent);
					Ex_ObjEnableEventBubble(hobj_button, TRUE); // 启用控件的事件冒泡，事件冒泡是指事件将根据父控件逐层传递至窗口
				}
			}
		}
	}
	Ex_DUIShowWindow(hExDui_event, SW_SHOWNORMAL, 0, 0, 0);
}

void test_loading(HWND hWnd)
{
	HWND hWnd_loading = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试加载动画", 0, 0, 300, 250, 0, 0);
	HEXDUI hExDui_loading = Ex_DUIBindWindowEx(hWnd_loading, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_loading, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	//马赛克加载动画
	HEXOBJ hObj_mosaicloading = Ex_ObjCreate(L"MosaicLoading", L"MosaicLoading", -1, 50, 50, 50, 50, hExDui_loading);
	Ex_ObjSetColor(hObj_mosaicloading, COLOR_EX_BACKGROUND, ExRGB(255, 10, 10), TRUE); //动画点颜色，只能提供RGB色

	//WIN10加载动画,默认风格为圆形转圈
	HEXOBJ hObj_win10loading1 = Ex_ObjCreate(L"Win10Loading", L"加载中，请稍后", -1, 150, 30, 100, 80, hExDui_loading);
	HEXOBJ hObj_win10loading2 = Ex_ObjCreate(L"Win10Loading", L"正在读取数据，请稍后", EOS_VISIBLE | ELDS_LINE, 50, 150, 200, 60, hExDui_loading);
	Ex_ObjSetColor(hObj_win10loading2, COLOR_EX_BACKGROUND, ExRGB(100, 236, 255), TRUE); //动画点颜色，只能提供RGB色
	Ex_DUIShowWindow(hExDui_loading, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hSliderBarLabel;

LRESULT CALLBACK OnSliderBarPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	std::wstring t = L"滑块条,当前值是：" + std::to_wstring(lParam);
	Ex_ObjSetText(m_hSliderBarLabel, t.c_str(), TRUE);
	return 0;
}

void test_sliderbar(HWND hWnd)
{
	HWND hWnd_sliderbar = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试滑块条", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_sliderbar = Ex_DUIBindWindowEx(hWnd_sliderbar, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_sliderbar, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	WCHAR ATOM_sliderbarEx[] = L"sliderbar"; /*滑块条*/
	HEXOBJ hObj_sliderbarex1 = Ex_ObjCreate(ATOM_sliderbarEx, NULL, -1, 80, 50, 250, 20, hExDui_sliderbar);
	Ex_ObjSetColor(hObj_sliderbarex1, COLOR_EX_BACKGROUND, ExARGB(100, 36, 255, 250), TRUE);
	Ex_ObjHandleEvent(hObj_sliderbarex1, SBN_VALUE, OnSliderBarPosChangeEvent);

	HEXOBJ hObj_sliderbarex2 = Ex_ObjCreate(ATOM_sliderbarEx, NULL, EOS_VISIBLE | ESBS_VERTICAL, 50, 60, 20, 100, hExDui_sliderbar);
	Ex_ObjSetColor(hObj_sliderbarex2, COLOR_EX_BACKGROUND, ExARGB(100, 236, 255, 250), TRUE);
	Ex_ObjHandleEvent(hObj_sliderbarex2, SBN_VALUE, OnSliderBarPosChangeEvent);
	Ex_ObjSetLong(hObj_sliderbarex2, SBL_BLOCK_POINT, 1);

	HEXOBJ hObj_sliderbarex3 = Ex_ObjCreate(ATOM_sliderbarEx, NULL, EOS_VISIBLE | ESBS_VERTICAL, 350, 60, 20, 100, hExDui_sliderbar);
	Ex_ObjSetColor(hObj_sliderbarex3, COLOR_EX_BACKGROUND, ExARGB(100, 136, 255, 250), TRUE);
	Ex_ObjHandleEvent(hObj_sliderbarex3, SBN_VALUE, OnSliderBarPosChangeEvent);
	Ex_ObjSetColor(hObj_sliderbarex3, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 125), TRUE); /*滑块条底色*/
	Ex_ObjSetColor(hObj_sliderbarex3, COLOR_EX_TEXT_CHECKED, ExARGB(200, 16, 25, 250), TRUE);  /*滑块条走过的颜色*/

	HEXOBJ hObj_sliderbarex4 = Ex_ObjCreate(ATOM_sliderbarEx, NULL, -1, 80, 150, 250, 20, hExDui_sliderbar);
	Ex_ObjSetColor(hObj_sliderbarex4, COLOR_EX_BACKGROUND, ExARGB(0, 136, 255, 250), TRUE);
	Ex_ObjHandleEvent(hObj_sliderbarex4, SBN_VALUE, OnSliderBarPosChangeEvent);
	Ex_ObjSetLong(hObj_sliderbarex4, SBL_BLOCK_POINT, 1);
	m_hSliderBarLabel = Ex_ObjCreate(L"static", L"滑块条,当前值是：", -1, 80, 80, 300, 30, hExDui_sliderbar);
	Ex_DUIShowWindow(hExDui_sliderbar, SW_SHOWNORMAL, 0, 0, 0);
}

void test_rotateimgbox(HWND hWnd)
{
	HWND hWnd_rotateimgbox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试旋转图片框", 0, 0, 200, 200, 0, 0);
	HEXDUI hExDui_rotateimgbox = Ex_DUIBindWindowEx(hWnd_rotateimgbox, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_rotateimgbox, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj_imagebox = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"RotateImageBox", L"旋转背景但文字不变", EOS_VISIBLE, 50, 50, 100, 100, hExDui_rotateimgbox, 0, -1, 0, 0, 0);
	Ex_ObjSetColor(hObj_imagebox, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(255, 255), TRUE);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/rotateimgbox.jpg", &imgdata);
	Ex_ObjSetBackgroundImage(hObj_imagebox, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFAULT, 0, BIF_PLAYIMAGE, 255, TRUE);
	Ex_ObjSetRadius(hObj_imagebox, 50, 50, 50, 50, TRUE);
	Ex_DUIShowWindow(hExDui_rotateimgbox, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnDragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_LBUTTONDOWN)
	{
		Ex_ObjSetLong(hObj, EOL_USERDATA, lParam);
		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, FALSE);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		Ex_ObjSetLong(hObj, EOL_USERDATA, 0);
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, FALSE);
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) == STATE_DOWN)
		{
			auto userdata = Ex_ObjGetLong(hObj, EOL_USERDATA);
			//获取按下位置
			POINT ptOrg;
			ptOrg.x = GET_X_LPARAM(userdata);
			ptOrg.y = GET_Y_LPARAM(userdata);
			//获取当前鼠标位置
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			auto parent = Ex_ObjGetParent(hObj);
			RECT rcParent{ 0 };
			//获取组件矩形
			RECT rcObj{ 0 };
			Ex_ObjGetRect(hObj, &rcObj);
			if (parent != 0)
			{
				Ex_ObjGetRect(parent, &rcParent);
				int x = rcObj.left + pt.x - ptOrg.x;
				int y = rcObj.top + pt.y - ptOrg.y;
				if (x <= 0)
				{
					x = 0;
				}
				if (x > rcParent.right - rcParent.left - (rcObj.right - rcObj.left))
				{
					x = rcParent.right - rcParent.left - (rcObj.right - rcObj.left);
				}
				if (y <= 0)
				{
					y = 0;
				}
				if (y > rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top))
				{
					y = rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top);
				}
				Ex_ObjSetPos(hObj, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			else
			{
				HEXDUI hexdui = Ex_DUIFromWindow(hWnd);
				Ex_DUIGetClientRect(hexdui, &rcParent);
				int x = rcObj.left + pt.x - ptOrg.x;
				int y = rcObj.top + pt.y - ptOrg.y;
				if (x <= 0)
				{
					x = 0;
				}
				if (x > rcParent.right - rcParent.left - (rcObj.right - rcObj.left))
				{
					x = rcParent.right - rcParent.left - (rcObj.right - rcObj.left);
				}
				if (y <= 0)
				{
					y = 0;
				}
				if (y > rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top))
				{
					y = rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top);
				}
				Ex_ObjSetPos(hObj, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}
	}
	return 0;
}

void test_dragobj(HWND hWnd)
{
	HWND hWnd_dragobj = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试拖动组件", 0, 0, 500, 500, 0, 0);
	HEXDUI hExDui_dragobj = Ex_DUIBindWindowEx(hWnd_dragobj, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_dragobj, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj_label1 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"static", L"可拖动组件1", EOS_VISIBLE | EOS_BORDER, 25, 35, 250, 250, hExDui_dragobj, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER, 0, 0, OnDragMsgProc);
	if (hObj_label1 != 0)
	{
		Ex_ObjSetColor(hObj_label1, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
		HEXOBJ hObj_label2 = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"static", L"可拖动组件2", EOS_VISIBLE | EOS_BORDER, 25, 35, 150, 150, hObj_label1, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER, 0, 0, OnDragMsgProc);
		if (hObj_label2 != 0)
		{
			Ex_ObjSetColor(hObj_label2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16722680, 100), TRUE);
		}
	}
	Ex_DUIShowWindow(hExDui_dragobj, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnDropObjDataMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_EX_DROP) //先触发本消息
	{
		EX_DROPINFO di{ 0 };
		RtlMoveMemory(&di, (LPVOID)lParam, sizeof(EX_DROPINFO));
		if (Ex_ObjCheckDropFormat(hObj, di.pDataObject, CF_UNICODETEXT) || Ex_ObjCheckDropFormat(hObj, di.pDataObject, CF_TEXT)) //判断是不是文本
		{
			INT len = Ex_ObjGetDropString(hObj, di.pDataObject, 0, 0);
			if (len > 0)
			{
				std::wstring str;
				str.resize(len * 2 + 2);
				Ex_ObjGetDropString(hObj, di.pDataObject, (LPWSTR)str.c_str(), len + 1);
				OUTPUTW(L"接收到文本拖拽:", str);
				*lpResult = DROPEFFECT_COPY; //按复制模式处理
				return 1;
			}
		}
	}
	else if (uMsg == WM_DROPFILES) //若上面未处理,且控件拥有#EOS_EX_ACCEPTFILES风格,则继续触发本消息
	{
		UINT fileNumber = DragQueryFileW((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
		for (UINT index = 0; index < fileNumber; index++)
		{
			UINT fileNameLength = DragQueryFileW((HDROP)wParam, index, NULL, 0);
			if (fileNameLength)
			{
				std::wstring fileName;
				fileName.resize(fileNameLength);
				DragQueryFileW((HDROP)wParam, index, (LPWSTR)fileName.c_str(), fileNameLength + 2);
				OUTPUTW(L"接收到文件拖拽:", fileName);
			}
		}
		*lpResult = DROPEFFECT_LINK; //按建立快捷方式处理
		return 1;
	}
	return 0;
}

void test_dropobj(HWND hWnd)
{
	HWND hWnd_dropobj = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试接收拖曳信息", 0, 0, 300, 300, 0, 0);
	HEXDUI hExDui_dropobj = Ex_DUIBindWindowEx(hWnd_dropobj, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_dropobj, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_ObjCreateEx(EOS_EX_DRAGDROP | EOS_EX_ACCEPTFILES, L"static", L"拖拽文本、文件到这里", EOS_VISIBLE | EOS_BORDER, 25, 25, 250, 250, hExDui_dropobj, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER, 0, 0, OnDropObjDataMsgProc);
	Ex_DUIShowWindow(hExDui_dropobj, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnProgressBarProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_TIMER)
	{
		size_t nPos = Ex_ObjSendMessage(hObj, PBM_GETPOS, 0, 0);
		size_t nRange = Ex_ObjSendMessage(hObj, PBM_GETRANGE, 0, 0);
		Ex_ObjSendMessage(hObj, PBM_SETPOS, nPos + Random(1, 20), 0);
		Ex_ObjSendMessage(hObj, WM_PAINT, 0, 0);
		if (nRange == nPos)
		{
			Ex_ObjKillTimer(hObj);
			Ex_MessageBox(hObj, L"加载完毕", L"提示", MB_OK, EMBF_CENTEWINDOW);
		}
	}

	return 0;
}

void test_progressbar(HWND hWnd)
{
	HWND hWnd_progressbar = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试进度条", 0, 0, 400, 300, 0, 0);
	HEXDUI hExDui_progressbar = Ex_DUIBindWindowEx(hWnd_progressbar, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_progressbar, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj_progressbar = Ex_ObjCreate(L"ProgressBar", NULL, -1, 50, 100, 300, 20, hExDui_progressbar);
	Ex_ObjSetLong(hObj_progressbar, EOL_OBJPROC, (size_t)OnProgressBarProc);
	Ex_ObjSetColor(hObj_progressbar, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
	Ex_ObjSendMessage(hObj_progressbar, PBM_SETRADIUS, 10, 0);
	Ex_ObjSendMessage(hObj_progressbar, PBM_SETRANGE, 255, 0);
	Ex_ObjInvalidateRect(hObj_progressbar, NULL);
	Ex_ObjSetTimer(hObj_progressbar, 50);
	Ex_DUIShowWindow(hExDui_progressbar, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnNchitTestButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NCHITTEST)
	{
		if (!(GET_X_LPARAM(lParam) >= 20 && GET_X_LPARAM(lParam) <= 60 && GET_Y_LPARAM(lParam) >= 20 && GET_Y_LPARAM(lParam) <= 60))
		{
			return HTTRANSPARENT;
		}
	}
	else if (uMsg == WM_ERASEBKGND) //wParam画布句柄
	{
		RECT rc{ 0 };
		Ex_ObjGetRect(hObj, &rc);
		HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(16746496, 255));
		_canvas_drawrect(wParam, hBrush, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 1, 0);
		_brush_setcolor(hBrush, ExRGB2ARGB(255, 255));
		_canvas_fillrect(wParam, hBrush, 20, 20, 60, 60);
		_brush_destroy(hBrush);
		return 1;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		OUTPUTW(L"被按下");
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		OUTPUTW(L"被放开");
	}
	return 0;
}

void test_nchittest(HWND hWnd)
{
	HWND hWnd_nchittest = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试限制通知区域", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_nchittest = Ex_DUIBindWindowEx(hWnd_nchittest, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_nchittest, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"static", L"鼠标只能在红色区域里响应", -1, 50, 50, 300, 100, hExDui_nchittest, 0, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, 0, OnNchitTestButtonMsgProc);
	Ex_DUIShowWindow(hExDui_nchittest, SW_SHOWNORMAL, 0, 0, 0);
}

HWND m_hWndModal;

INT_PTR CALLBACK OnDialgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG)
	{
		MoveWindow(hWnd, 0, 0, 300, 200, FALSE);
		SetWindowTextW(hWnd, L"我是模态对话框 我在的时候别的窗口不能动");
		HEXDUI hExDui = Ex_DUIBindWindow(hWnd, 0, EWS_TITLE | EWS_SIZEABLE | EWS_MOVEABLE | EWS_BUTTON_CLOSE | EWS_ESCEXIT | EWS_CENTERWINDOW);
		if (hExDui)
		{
			HEXOBJ hEdit = Ex_ObjCreate(L"edit", 0, -1, 20, 50, 250, 30, hExDui);
			if (hEdit)
			{
				Ex_ObjSendMessage(hEdit, EM_SETCUEBANNER, ExRGB2ARGB(8421504, 200), (LPARAM)L"编辑框输入文字正常");
			}
			Ex_DUISetLong(hExDui, EWL_CRBKG, ExRGB2ARGB(16711680, 220));
			Ex_DUIShowWindow(hExDui, SW_SHOW, 0, 0, 0);
		}
	}
	return 0;
}

LRESULT CALLBACK OnModalButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	HRSRC hrsrc = FindResourceW(GetModuleHandleW(0), MAKEINTRESOURCE(IDD_PROPPAGE_SMALL), RT_DIALOG);
	if (hrsrc)
	{
		LPCDLGTEMPLATEW hglobal = (LPCDLGTEMPLATEW)LoadResource(GetModuleHandleW(0), hrsrc);
		if (hglobal)
		{
			DialogBoxIndirectParamW(GetModuleHandleW(0), hglobal, m_hWndModal, OnDialgWndProc, 0);
		}
	}
	return 0;
}

void test_modal(HWND hWnd)
{
	m_hWndModal = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试模态窗口", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui_modal = Ex_DUIBindWindowEx(m_hWndModal, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_modal, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj = Ex_ObjCreate(L"button", L"弹出模态对话框", -1, 50, 50, 300, 100, hExDui_modal);
	Ex_ObjHandleEvent(hObj, NM_CLICK, OnModalButtonEvent);
	Ex_DUIShowWindow(hExDui_modal, SW_SHOWNORMAL, 0, 0, 0);
}


LRESULT CALLBACK OnColorPickerButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == CPN_COLORCHANGE) {
		OUTPUTW(L"颜色已更改", (int)lParam);
	}
	return 0;
}

void test_colorpicker(HWND hParent)
{
	HWND hWnd_colorpicker = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试颜色选择器", 0, 0, 250, 300, 0, 0);
	HEXDUI hExDui_colorpicker = Ex_DUIBindWindowEx(hWnd_colorpicker, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_colorpicker, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj = Ex_ObjCreate(L"ColorPicker", 0, -1, 50, 40, 80, 30, hExDui_colorpicker);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(0, 255), TRUE);
	Ex_ObjHandleEvent(hObj, CPN_COLORCHANGE, OnColorPickerButtonEvent);
	Ex_DUIShowWindow(hExDui_colorpicker, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnDateBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == DBN_DATETIME) {
		EX_DATETIME* dt = (EX_DATETIME*)lParam;
		OUTPUTW(L"日期已更改", dt->Year, dt->Mon, dt->Mday, dt->Wday);
	}
	return 0;
}

void test_datebox(HWND hParent)
{
	HWND hWnd_datebox = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试日期框", 0, 0, 250, 200, 0, 0);
	HEXDUI hExDui_datebox = Ex_DUIBindWindowEx(hWnd_datebox, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_datebox, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ hObj = Ex_ObjCreate(L"DateBox", 0, -1, 50, 80, 150, 30, hExDui_datebox);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, -1, FALSE);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16711680, 255), TRUE);

	Ex_ObjHandleEvent(hObj, DBN_DATETIME, OnDateBoxButtonEvent);

	Ex_DUIShowWindow(hExDui_datebox, SW_SHOWNORMAL, 0, 0, 0);
}

void test_titlebar(HWND hParent)
{
	HWND hWnd = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试标题框", 0, 0, 400, 200, 0, 0);
	HEXDUI hExDui = Ex_DUIBindWindowEx(hWnd, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	HEXOBJ hObj = 0;
	hObj = Ex_ObjCreate(L"Titlebar", L"标题框1", -1, 30, 50, 300, 20, hExDui);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, -1, FALSE);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);

	hObj = Ex_ObjCreate(L"Titlebar", L"标题框2", -1, 30, 80, 300, 20, hExDui);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 255), FALSE);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(255, 255), TRUE);

	hObj = Ex_ObjCreate(L"Titlebar", L"标题框3", -1, 30, 110, 300, 20, hExDui);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 255), FALSE);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16711680, 255), TRUE);

	hObj = Ex_ObjCreate(L"Titlebar", L"标题框4", -1, 30, 140, 300, 20, hExDui);
	Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(0, 255), FALSE);
	Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, -1, TRUE);

	Ex_DUIShowWindow(hExDui, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnCalendarEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == DBN_DATETIME)
	{
		EX_DATETIME* dt = (EX_DATETIME*)lParam;
		OUTPUTW(L"日期已更改", dt->Year, dt->Mon, dt->Mday, dt->Wday);
	}
	return 0;
}

void test_calendar(HWND hParent)
{
	HWND hWnd_calendar = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试月历", 0, 0, 400, 400, 0, 0);
	HEXDUI hExDui_calendar = Ex_DUIBindWindowEx(hWnd_calendar, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_calendar, EWL_CRBKG, ExARGB(200, 200, 200, 255));

	HEXOBJ MonthCal = Ex_ObjCreateEx(-1, L"Calendar", NULL, EOS_VISIBLE | EOS_BORDER, 50, 50, 336, 336, hExDui_calendar, 100, -1, 0, 0, 0);
	Ex_ObjHandleEvent(MonthCal, MCN_DATETIME, OnCalendarEvent);
	Ex_DUIShowWindow(hExDui_calendar, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hObjChromium;
struct MyValObj
{
	LPCWSTR myval;
};


LRESULT CALLBACK OnChromiumWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE)
	{
		HDC dc = GetDC(NULL);
		FLOAT dpiy = (FLOAT)GetDeviceCaps(dc, 90) / 96;
		ReleaseDC(0, dc);
		Ex_ObjMove(m_hObjChromium, 50, 50, (LOWORD(lParam) - 100) / dpiy, (HIWORD(lParam) - 100) / dpiy, FALSE);
	}
	return 0;
}

void CALLBACK OnFunction(LPCWSTR name, HV8VALUE object, std::vector<uintptr_t*> arguments, uintptr_t* retval, LPCWSTR exception, bool* pbHandled, void* lParam) {
	if (std::wstring(name) == L"add_Function")
	{
		//演示接收js传入两个整数变量并运算返回
		int nSum = 0;
		for (INT i = 0; i < arguments.size(); i++) {
			if (!Ck_V8IsInt((HV8VALUE)arguments[i]))
			{
				return;
			}
			nSum = nSum + Ck_V8GetIntValue((HV8VALUE)arguments[i]);
		}
		//创建整数返回值
		*retval = (uintptr_t)Ck_V8CreateInt(nSum);
		*pbHandled = TRUE;
	}
	else if (std::wstring(name) == L"register")
	{
		//演示整数数组传入js并接收返回
		auto v8 = (HV8VALUE)arguments[0];
		std::vector<int> data;
		data.push_back(3);
		data.push_back(8);
		auto ret = Ck_V8ExecuteFunction_Int(v8, data.data(), 2);
		auto retint = Ck_V8GetIntValue(ret);
		*retval = (uintptr_t)Ck_V8CreateInt(retint);
		*pbHandled = TRUE;
	}
	else if (std::wstring(name) == L"addFunction1")
	{
		if (retval)
		{
			//这里设置myval初始化值传给js
			*retval = (uintptr_t)Ck_V8CreateString(L"test.myfunc返回addFunction1的值");
			*pbHandled = TRUE;
		}

	}
}

void CALLBACK OnFunAccessor(int uMsg, LPCWSTR name, HV8VALUE object, uintptr_t* retval, LPCWSTR exception, bool* pbHandled, void* lParam)
{
	if (uMsg == 1)//获取值消息
	{
		if (std::wstring(name) == L"myval")
		{
			//这里设置myval初始化值传给js
			*retval = (uintptr_t)Ck_V8CreateString(L"我是程序初始化的值传给js");
			*pbHandled = TRUE;
		}
	}
	else if (uMsg == 2)//设置值消息
	{
		if (std::wstring(name) == L"myval")
		{
			//这里返回js修改后的myval值
			auto v8 = (HV8VALUE)retval;
			auto retstr = Ck_V8GetStringValue(v8);
			std::wstring str = L"返回js修改后的myval值:" + std::wstring(retstr);
			Ex_MessageBox(0, str.c_str(), L"", 0, 0);
			*pbHandled = TRUE;
		}
	}

}

void CALLBACK OnBeforeCommandLine(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd, void* lParam)
{
	if (uMsg == 1)
	{
		OUTPUTW(L"即将处理命令行");
	}
	else if (uMsg == 2) {
		OUTPUTW(L"即将创建V8环境");
		//绑定js文本值变量示例
		HV8VALUE window = Ck_V8CGetGlobal((HV8CONTEXE)attach1);
		HV8VALUE v8 = Ck_V8CreateString(L"say yes");
		Ck_V8SetValue(window, L"say_yes", v8, 0);

		//绑定js对象变量示例
		HV8VALUE objV8 = Ck_V8CreateObject(OnFunAccessor, NULL);
		Ck_V8SetValueEx(objV8, L"myval", 0, 0);
		Ck_V8SetValue(window, L"MyValObj", objV8, 0);

		//绑定js add_Function方法
		v8 = Ck_V8CreateFunction(L"add_Function", (void*)OnFunction, NULL);
		Ck_V8SetValue(window, L"add_Function", v8, 0);

		//绑定js register方法
		v8 = Ck_V8CreateFunction(L"register", (void*)OnFunction, NULL);
		Ck_V8SetValue(window, L"register", v8, 0);
	}
	else if (uMsg == 3)
	{
		OUTPUTW(L"即将释放V8环境");
	}
	else if (uMsg == 4)
	{
		OUTPUTW(L"即将初始化WebKit");
		//演示赋值全局变量g_value传入js,key固定是v8/mycode,文本值不能是汉字
		std::wstring value = L"var g_value=\"set value\"; ";
		Ck_RegisterExtension(L"v8/mycode", value.c_str());

		//演示赋值全局函数传入js,key是v8/mycode1，注意不能和其它函数同名
		std::wstring js = L"var test;\
			if (!test)\
				test = {};\
		(function() {\
			test.myfunc = function() {\
				native function addFunction1();\
				return addFunction1();\
			};\
		})(); ";
		Ck_RegisterExtensionEx(L"v8/mycode1", js.c_str(), OnFunction, 0);
	}
}

LRESULT CALLBACK OnChromiumEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == CEFN_LOADEND)
	{
		OUTPUTW(L"cef浏览框加载完毕");
	}

	return 0;
}

void test_chromium(HWND hParent)
{
	HWND hWnd_chromium = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试Cef浏览框", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_chromium = Ex_DUIBindWindowEx(hWnd_chromium, 0, EWS_NOINHERITBKG | EWS_CENTERWINDOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON | EWS_SIZEABLE, 0, OnChromiumWndMsgProc);
	Ex_DUISetLong(hExDui_chromium, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_ObjCefBrowserInitialize(0, L"D:/ExDUIR/Release", L"FTBrowser.dll", NULL, 0, 0, OnBeforeCommandLine);
	m_hObjChromium = Ex_ObjCreateEx(-1, L"CefBrowser", NULL, -1, 30, 30, 750, 550, hExDui_chromium, 0, -1, 0, 0, 0);
	Ex_ObjSendMessage(m_hObjChromium, CEFM_LOADURL, 0, (LPARAM)L"https://www.bing.com");
	//Ex_ObjSendMessage(m_hObjChromium, CEFM_LOADURL, 0, (LPARAM)L"d:/test.mp4");
	//Ex_ObjSendMessage(m_hObjChromium, CEFM_LOADURL, 0, (LPARAM)L"d:/ExDUIR/msvc/test/res/cefjs.html");
	Ex_ObjHandleEvent(m_hObjChromium, CEFN_LOADEND, OnChromiumEvent);
	Ex_DUIShowWindow(hExDui_chromium, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<HEXOBJ> m_hScoreBtn(5);

LRESULT CALLBACK OnScoreButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (lParam != 0)
	{
		INT nIndex = Ex_ObjGetLong(hObj, EOL_LPARAM);
		OUTPUTW(L"选择分数", nIndex);
	}
	return 0;
}

void test_scorebtn(HWND hParent)
{
	HWND hWnd_score = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试打分按钮", 0, 0, 300, 100, 0, 0);
	HEXDUI hExDui_score = Ex_DUIBindWindowEx(hWnd_score, 0, EWS_NOINHERITBKG | EWS_CENTERWINDOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON | EWS_SIZEABLE, 0, 0);
	Ex_DUISetLong(hExDui_score, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXIMAGE hImg1 = 0;
	HEXIMAGE hImg2 = 0;
	for (int i = 0; i < 5; i++)
	{
		m_hScoreBtn[i] = Ex_ObjCreate(L"ScoreButton", 0, -1, 20 + i * 45, 40, 40, 40, hExDui_score);
		_img_createfromfile(L"./navbtn/star_normal.png", &hImg1);

		Ex_ObjSendMessage(m_hScoreBtn[i], BM_SETIMAGE, 0, hImg1);

		_img_createfromfile(L"./navbtn/star_hover.png", &hImg2);

		Ex_ObjSendMessage(m_hScoreBtn[i], BM_SETIMAGE, 1, hImg2);
		Ex_ObjSetLong(m_hScoreBtn[i], EOL_LPARAM, i);
		Ex_ObjHandleEvent(m_hScoreBtn[i], NM_CHECK, OnScoreButtonCheckEvent);
	}

	Ex_DUIShowWindow(hExDui_score, SW_SHOWNORMAL, 0, 0, 0);
}

void test_carousel(HWND hParent)
{
	HWND hWnd_carousel = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试轮播", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_carousel = Ex_DUIBindWindowEx(hWnd_carousel, 0, EWS_NOINHERITBKG | EWS_CENTERWINDOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON | EWS_SIZEABLE, 0, 0);
	Ex_DUISetLong(hExDui_carousel, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj = Ex_ObjCreate(L"Carousel", 0, -1, 20, 40, 760, 550, hExDui_carousel);

	Ex_ObjSendMessage(hObj, CM_SIZE, 1600, 1200);
	HEXIMAGE hImg = 0;
	_img_createfromfile(L"res/1.jpeg", &hImg);
	Ex_ObjSendMessage(hObj, CM_ADDIMG, 0, hImg);
	_img_createfromfile(L"res/2.jpeg", &hImg);
	Ex_ObjSendMessage(hObj, CM_ADDIMG, 0, hImg);
	_img_createfromfile(L"res/3.jpeg", &hImg);
	Ex_ObjSendMessage(hObj, CM_ADDIMG, 0, hImg);
	Ex_ObjSetTimer(hObj, 5000);

	// 全部销毁用下面的
	//Ex_ObjSendMessage(hObj, CM_CLEAR, 0, 0);

	Ex_DUIShowWindow(hExDui_carousel, SW_SHOWNORMAL, 0, 0, 0);
}

std::vector<TLISTVIEW_ITEM> m_tlistViewItemInfo;

LRESULT CALLBACK OnTemplateListViewItemBtnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{

	if (Ex_ObjGetLong(hObj, EOL_NODEID) == 3)//点了某项的按钮
	{
		HEXOBJ hObjItem = Ex_ObjGetParent(hObj);// 表项句柄
		INT nIndex = Ex_ObjGetLong(hObjItem, 0);// 获得表项当前代表的索引
		if (nIndex > 0 && nIndex <= (m_tlistViewItemInfo.size()))
		{
			m_tlistViewItemInfo.erase(m_tlistViewItemInfo.begin() + nIndex - 1);
			OUTPUTW(L"TList 按钮点击,删除本行", nIndex - 1, nID, wParam, lParam, m_tlistViewItemInfo.size());
			Ex_ObjSendMessage(Ex_ObjGetParent(hObjItem), WM_PAINT, 0, 1);
		}
	}
	if (nCode == NM_DBLCLK)
	{
		output(L"OnTemplateListViewItemBtnClick NM_DBLCLK", wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK OnTemplateListViewProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	HEXOBJ hObjTmp = 0;
	static int selectedItem[2] = { 0 };
	if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR* ni = (EX_NMHDR*)lParam;
		if (ni->hObjFrom == hObj)
		{
			if (ni->nCode == NM_CALCSIZE)//设置表项尺寸事件 默认为列表框宽度/一行文字的高度
			{
				__set((void*)ni->lParam, 4, 60);//ni->lParam指向一个size结构,偏移0为宽度,4为高度
				__set((void*)ni->lParam, 12, 0);//  ' 纵间距
				*lpResult = 1;//拦截这个事件
				return 1;
			}
			else if (ni->nCode == LVN_ITEMSELECTD)//ni->wParam:当前选中索引   ni->lParam:上次选中索引  索引从1开始
			{
				output(L"TList表项选中改变", ni->wParam, ni->lParam);
			}
			else if (ni->nCode == LVN_ITEMSELECTC)//ni->wParam:当前选中索引   ni->lParam:上次选中索引  索引从1开始
			{
				output(L"TList表项取消选中", ni->wParam, ni->lParam);
			}
			else if (ni->nCode == LVN_ITEMCHANGED)//ni->wParam:当前选中索引   ni->lParam:上次选中索引  索引从1开始
			{
				output(L"TList现行选中项被改变", ni->wParam, ni->lParam);
			}
			else if (ni->nCode == LVN_ITEMRCLICK)//ni->wParam:当前选中索引   ni->lParam:当前选中数目
			{
				output(L"TList表项被右击", ni->wParam, ni->lParam);
			}
			else if (ni->nCode == LVN_ITEMDCLICK)
			{
				output(L"TList NM_DBLCLK", ni->wParam, ni->lParam);
			}
		}
	}
	else if (uMsg == TLVM_ITEM_CREATED)
	{
		//lParam: hObjItem
		hObjTmp = Ex_ObjCreateEx(-1, L"Static", 0, -1, 0, 6, 128, 28, lParam, 0, DT_CENTER | DT_VCENTER, 0, 0, 0);
		Ex_ObjSetLong(hObjTmp, EOL_NODEID, 1);
		hObjTmp = Ex_ObjCreateEx(-1, L"Static", 0, -1, 130, 6, 358, 28, lParam, 0, DT_CENTER | DT_VCENTER, 0, 0, 0);
		Ex_ObjSetLong(hObjTmp, EOL_NODEID, 2);
		hObjTmp = Ex_ObjCreateEx(-1, L"Button", 0, -1, 555, 11, 50, 20, lParam, 0, -1, 0, 0, 0);
		Ex_ObjSetLong(hObjTmp, EOL_NODEID, 3);
		Ex_ObjHandleEvent(hObjTmp, NM_CLICK, OnTemplateListViewItemBtnClick);
		*lpResult = 1;
		return 1;
	}
	else if (uMsg == TLVM_ITEM_FILL)
	{
		if (wParam > 0 && wParam <= (m_tlistViewItemInfo.size()))//索引从1开始
		{
			hObjTmp = Ex_ObjGetFromNodeID(lParam, 1);
			if (hObjTmp)
			{
				Ex_ObjSetText(hObjTmp, L"TEST", true);
			}

			hObjTmp = Ex_ObjGetFromNodeID(lParam, 2);
			if (hObjTmp)
			{
				Ex_ObjSetText(hObjTmp, m_tlistViewItemInfo[wParam - 1].text.c_str(), true);
			}

			hObjTmp = Ex_ObjGetFromNodeID(lParam, 3);
			if (hObjTmp)
			{
				Ex_ObjSetText(hObjTmp, m_tlistViewItemInfo[wParam - 1].btnTitle.c_str(), true);
			}
		}
	}
	return 0;
}

void test_templatelistview(HWND hParent)
{
	HWND hWnd_listview = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试模板列表", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_listview = Ex_DUIBindWindowEx(hWnd_listview, 0, EWS_NOINHERITBKG | EWS_CENTERWINDOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON | EWS_SIZEABLE, 0, 0);
	Ex_DUISetLong(hExDui_listview, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hobj_groupbox = Ex_ObjCreate(L"groupbox", L"分组框", -1, 10, 40, 780, 550, hExDui_listview);
	//ELVS_ITEMTRACKING风格需启用
	HEXOBJ hobj_listview = Ex_ObjCreateEx(-1, L"TListView",
		NULL, EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL | ELVS_ITEMTRACKING, 20, 10, 650, 520,
		hobj_groupbox, 0, -1, 0, 0, OnTemplateListViewProc);
	if (m_tlistViewItemInfo.size() == 0)
	{
		for (int i = 0; i < 20; i++)
		{
			m_tlistViewItemInfo.push_back({ L"标签一" + std::to_wstring(i),L"标签二" + std::to_wstring(i),L"按钮" + std::to_wstring(i) });
		}
	}
	Ex_ObjSendMessage(hobj_listview, LVM_SETITEMCOUNT, m_tlistViewItemInfo.size(), m_tlistViewItemInfo.size());
	Ex_ObjSendMessage(hobj_listview, TLVM_SET_ITEM_HOVERCOLOR, 0, ExRGB2ARGB(15066083, 200));//表项悬浮色
	Ex_ObjSendMessage(hobj_listview, TLVM_SET_ITEM_SELECTCOLOR, 0, ExRGB2ARGB(124123, 250));//表项选中色
	Ex_DUIShowWindow(hExDui_listview, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hObjDrawingBoard;

LRESULT CALLBACK OnDrawingBoardSwitchEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CHECK)
	{
		if (wParam != 0)
		{
			Ex_ObjSendMessage(m_hObjDrawingBoard, DBM_SETPENTYPE, 0, 0);

		}
		else {
			Ex_ObjSendMessage(m_hObjDrawingBoard, DBM_SETPENTYPE, 0, 1);
		}
	}
	return 0;
}

LRESULT CALLBACK OnDrawingBoardButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CLICK)
	{
		if (nID == 100)
		{
			Ex_ObjSendMessage(m_hObjDrawingBoard, DBM_CLEAR, 0, 0);
		}
		else if (nID == 101)
		{
			Ex_ObjSendMessage(m_hObjDrawingBoard, DBM_SETPENWIDTH, 0, 5);
		}
		else if (nID == 102)
		{
			Ex_ObjSendMessage(m_hObjDrawingBoard, DBM_SETPENCOLOR, 0, ExARGB(255, 0, 0, 255));
		}
		else if (nID == 103)
		{
			HEXCANVAS canvas = Ex_ObjGetLong(m_hObjDrawingBoard, EOL_HCANVAS);
			HEXIMAGE img;
			_img_createfromcanvas(canvas, &img);
			_img_savetofile(img, L"d:/111.png");
			_img_destroy(img);
		}
	}
	return 0;
}

void test_drawingboard(HWND hParent)
{
	HWND hWnd_drawingboard = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试鼠标绘制板", 0, 0, 680, 400, 0, 0);
	HEXDUI hExDui_drawingboard = Ex_DUIBindWindowEx(hWnd_drawingboard, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_drawingboard, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	m_hObjDrawingBoard = Ex_ObjCreate(L"drawingboard", 0, -1, 30, 30, 500, 350, hExDui_drawingboard);
	HEXOBJ hObj_switch = Ex_ObjCreate(L"Switch", L"画笔|橡皮擦", -1, 550, 30, 100, 30, hExDui_drawingboard);
	Ex_ObjSendMessage(hObj_switch, BM_SETCHECK, 1, 0); // 设置选中状态
	Ex_ObjHandleEvent(hObj_switch, NM_CHECK, OnDrawingBoardSwitchEvent);

	auto hObj1 = Ex_ObjCreateEx(-1, L"button", L"清空绘制板", -1, 550, 70, 100, 30, hExDui_drawingboard, 100, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj1, NM_CLICK, OnDrawingBoardButtonEvent);
	auto hObj2 = Ex_ObjCreateEx(-1, L"button", L"改变画刷大小", -1, 550, 110, 100, 30, hExDui_drawingboard, 101, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj2, NM_CLICK, OnDrawingBoardButtonEvent);
	auto hObj3 = Ex_ObjCreateEx(-1, L"button", L"改变画刷颜色", -1, 550, 150, 100, 30, hExDui_drawingboard, 102, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj3, NM_CLICK, OnDrawingBoardButtonEvent);
	auto hObj4 = Ex_ObjCreateEx(-1, L"button", L"保存到图片", -1, 550, 190, 100, 30, hExDui_drawingboard, 103, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj4, NM_CLICK, OnDrawingBoardButtonEvent);
	Ex_DUIShowWindow(hExDui_drawingboard, SW_SHOWNORMAL, 0, 0, 0);
}

HEXDUI m_hExDuiPalette;

LRESULT CALLBACK OnPaletteEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == PTN_MOUSEMOVE)
	{
		Ex_DUISetLong(m_hExDuiPalette, EWL_CRBKG, ExRGB2ARGB(wParam, 255));
	}
	return 0;
}

void test_palette(HWND hParent)
{
	HWND hWnd_palette = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试调色板", 0, 0, 400, 250, 0, 0);
	m_hExDuiPalette = Ex_DUIBindWindowEx(hWnd_palette, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(m_hExDuiPalette, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	HEXOBJ hObj = Ex_ObjCreate(L"Palette", 0, -1, 50, 50, 300, 150, m_hExDuiPalette);
	Ex_ObjHandleEvent(hObj, PTN_MOUSEMOVE, OnPaletteEvent);
	Ex_DUIShowWindow(m_hExDuiPalette, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hObjPropertyGrid = 0;

LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_CLICK)
	{
		if (nID == 100)
		{
			LPCWSTR ret = (LPCWSTR)Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_GETITEMVALUE, 0, (LPARAM)L"名称2");
			OUTPUTW(L"名称2 对应值:", ret);
		}
		else if (nID == 101)
		{
			LPCWSTR ret = (LPCWSTR)Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_SETITEMVALUE, (WPARAM)L"新数值123", (LPARAM)L"名称2");
			OUTPUTW(L"置\"名称2\"对应值");
		}
		else if (nID == 102)
		{
			Ex_ObjMove(m_hObjPropertyGrid, 20, 30, 350, 360, TRUE);
		}
	}
	return 0;
}

LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	EX_PROGRID_CHANGEITEMINFO itemInfo{ 0 };
	RtlMoveMemory(&itemInfo, (void*)lParam, sizeof(EX_PROGRID_CHANGEITEMINFO));
	OUTPUTW(L"属性框值改变, 对应行索引:", wParam, L", 改变后值:", itemInfo.text, L", 改变类型:", itemInfo.type);
	return 0;
}

void test_propertygrid(HWND hParent)
{
	HWND hWnd_propertygrid = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试属性框", 0, 0, 500, 400, 0, 0);
	HEXDUI hExDui_propertygrid = Ex_DUIBindWindowEx(hWnd_propertygrid, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_propertygrid, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	m_hObjPropertyGrid = Ex_ObjCreateEx(-1, L"PropertyGrid", L"PropertyGrid", EOS_VISIBLE | EOS_VSCROLL,
		50, 50, 300, 300, hExDui_propertygrid, 0, 0, 0, 0, 0);
	Ex_ObjHandleEvent(m_hObjPropertyGrid, PGN_ITEMVALUECHANGE, OnPropertyGridEvent);

	Ex_ObjSetColor(m_hObjPropertyGrid, COLOR_EX_BACKGROUND, ExRGB2ARGB(14737632, 255), TRUE);

	auto hObj1 = Ex_ObjCreateEx(-1, L"button", L"取表项内容", -1, 380, 70, 100, 30, hExDui_propertygrid, 100, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj1, NM_CLICK, OnPropertyGridButtonEvent);
	hObj1 = Ex_ObjCreateEx(-1, L"button", L"置表项内容", -1, 380, 120, 100, 30, hExDui_propertygrid, 101, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj1, NM_CLICK, OnPropertyGridButtonEvent);
	hObj1 = Ex_ObjCreateEx(-1, L"button", L"修改组件大小", -1, 380, 170, 100, 30, hExDui_propertygrid, 102, -1, 0, 0, 0);
	Ex_ObjHandleEvent(hObj1, NM_CLICK, OnPropertyGridButtonEvent);

	EX_PROGRID_ITEMINFO item;
	item.title = L"小组A";
	Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_GROUP, (LPARAM)&item);
	item.title = L"组合框一";
	EX_PROGRID_ITEMINFO_COMBOBOX a;
	a.text = L"表项1-1";
	EX_PROGRID_ITEMINFO_COMBOBOX b;
	b.text = L"表项1-2";

	item.textComboBox[0] = a;
	item.textComboBox[1] = b;
	item.comboboxNum = 2;


	Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_COMBOBOX, (LPARAM)&item);
	item.title = L"颜色";
	auto color = std::to_wstring(ExRGB2ARGB(167549, 255));
	item.text = color.c_str();
	Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_COLORPICKER, (LPARAM)&item);
	item.title = L"日期";
	item.text = L"2022-7-6";
	Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_DATEBOX, (LPARAM)&item);

	std::vector<std::wstring> titles = { L"普通编辑框",L"数字输入编辑框",L"字母输入编辑框",L"字母数字编辑框",L"只读编辑框" };
	std::vector<std::wstring> texts = { L"中文123abc",L"1234567890",L"aA",L"aA123",L"测试只读" };
	std::vector<int> editStyles = { 0, 1,2,3,4 };
	for (int i = 0; i < 5; i++)
	{
		auto title = titles[i];
		item.title = title.c_str();
		auto text = texts[i];
		item.text = text.c_str();
		item.editStyle = editStyles[i];
		Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_EDIT, (LPARAM)&item);
	}
	item.title = L"小组B";
	Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_GROUP, (LPARAM)&item);
	item.title = L"颜色二";
	color = std::to_wstring(ExRGB2ARGB(3523123, 255));
	item.text = color.c_str();
	Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_COLORPICKER, (LPARAM)&item);
	item.title = L"组合框二";

	EX_PROGRID_ITEMINFO_COMBOBOX c;
	c.text = L"表项2-1";
	EX_PROGRID_ITEMINFO_COMBOBOX d;
	d.text = L"表项2-2";

	item.textComboBox[0] = c;
	item.textComboBox[1] = d;
	item.comboboxNum = 3;

	Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_COMBOBOX, (LPARAM)&item);
	for (int i = 0; i < 4; i++)
	{
		auto title = L"项目" + std::to_wstring(i + 1);
		item.title = title.c_str();
		item.text = L"aA";
		item.editStyle = 0;
		Ex_ObjSendMessage(m_hObjPropertyGrid, PGM_ADDITEM, PGT_OBJ_EDIT, (LPARAM)&item);
	}

	Ex_DUIShowWindow(hExDui_propertygrid, SW_SHOWNORMAL, 0, 0, 0);
}

HWND m_hWndChild;
HWND m_hWndChild2;
HWND m_hWndNativeWindow2;
HEXOBJ m_hObjPage;

LRESULT CALLBACK OnNativeParentWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE)
	{
		INT width = LOWORD(lParam);
		INT height = HIWORD(lParam);
		HDC dc = GetDC(NULL);
		FLOAT dpiy = (FLOAT)GetDeviceCaps(dc, 90) / 96;
		ReleaseDC(0, dc);
		MoveWindow(m_hWndNativeWindow2, 0, 30 * dpiy, width, height - 30 * dpiy, TRUE);
		MoveWindow(m_hWndChild, 0, 0, width - 30, 300 * dpiy, TRUE);
		MoveWindow(m_hWndChild2, 0, 310 * dpiy, width - 30, 300 * dpiy, TRUE);
		Ex_ObjMove(m_hObjPage, 0, 0, width / dpiy, height / dpiy - 30, TRUE);
		Ex_ObjScrollSetInfo(m_hObjPage, SB_VERT, SIF_ALL, 0, (300 + 300 - height / dpiy + 30) * dpiy, 100, 0, TRUE);
	}
	return 0;
}

LRESULT CALLBACK OnNativeWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NCLBUTTONDBLCLK)
	{
		// 禁用标题栏双击最大化消息
		return 1;
	}
	else if (uMsg == WM_NCLBUTTONDOWN)
	{
		// 禁用标题栏鼠标按下拖动消息
		if (wParam == HTCAPTION)
		{
			return 1;
		}
	}
	return 0;
}

INT Ex_ObjScrollDefaultProc(HEXOBJ hObj, DWORD nBar, WPARAM wParam, INT nLine, INT nPage, BOOL fRedraw)
{
	auto hObjScroll = Ex_ObjScrollGetControl(hObj, nBar);
	INT nPos = 0;
	if (hObjScroll != 0)
	{
		DWORD nCode = LOWORD(wParam);
		INT nMin, nMax, oldPos, trackPos;
		Ex_ObjScrollGetInfo(hObj, nBar, &nMin, &nMax, &oldPos, &trackPos);
		if (nCode == SB_PAGEUP)
		{
			nPos = oldPos - nPage;
		}
		else if (nCode == SB_PAGEDOWN)
		{
			nPos = oldPos + nPage;
		}
		else if (nCode == SB_LINEUP)
		{
			nPos = oldPos - nLine;
		}
		else if (nCode == SB_LINEDOWN)
		{
			nPos = oldPos + nLine;
		}
		else if (nCode == SB_TOP)
		{
			nPos = nMin;
		}
		else if (nCode == SB_BOTTOM)
		{
			nPos = nMax;
		}
		else {
			oldPos = nMin - 1;
			nPos = Ex_ObjScrollGetTrackPos(hObj, nBar);
		}
		if (nPos < nMin)
		{
			nPos = nMin;
		}
		if (nPos > nMax)
		{
			nPos = nMax;
		}
		if (nPos != oldPos)
		{
			Ex_ObjScrollSetPos(hObj, nBar, nPos, fRedraw);
		}
	}
	return nPos;
}

LRESULT CALLBACK OnNativeWndScrollMsg(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_VSCROLL)
	{
		auto nPos = Ex_ObjScrollDefaultProc(hObj, SB_VERT, wParam, 1, 10, TRUE);
		RECT rc;
		GetWindowRect(m_hWndChild, &rc);
		HDC dc = GetDC(NULL);
		FLOAT dpiy = (FLOAT)GetDeviceCaps(dc, 90) / 96;
		ReleaseDC(0, dc);
		MoveWindow(m_hWndChild, 0, 0 * dpiy - nPos, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		MoveWindow(m_hWndChild2, 0, 310 * dpiy - nPos, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	}
	return 0;
}

void test_nativewindow(HWND hParent)
{
	HWND hWnd_nativewindow = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试原生子窗口", 0, 0, 400, 450, 0, 0);
	HEXDUI hExDui_nativewindow = Ex_DUIBindWindowEx(hWnd_nativewindow, 0, EWS_NOINHERITBKG | EWS_SIZEABLE | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE, 0, OnNativeParentWndMsgProc);
	Ex_DUISetLong(hExDui_nativewindow, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	m_hWndNativeWindow2 = Ex_WndCreate(hWnd_nativewindow, L"Ex_DirectUI", 0, 0, 30, 400, 420, WS_CHILD | WS_OVERLAPPEDWINDOW, 0);
	HEXDUI hExDui_nativewindow2 = Ex_DUIBindWindowEx(m_hWndNativeWindow2, 0, EWS_NOSHADOW, 0, OnNativeWndMsgProc);
	Ex_DUISetLong(hExDui_nativewindow2, EWL_CRBKG, ExARGB(100, 150, 150, 255));

	m_hObjPage = Ex_ObjCreateEx(-1, L"static", 0, EOS_VISIBLE | EOS_VSCROLL, 0, 0, 400, 420, hExDui_nativewindow2, 0, -1, 0, 0, OnNativeWndScrollMsg);

	Ex_ObjScrollSetInfo(m_hObjPage, SB_VERT, SIF_ALL, 0, (300 + 300 - 420 + 30) * 1, 100, 0, TRUE);//1是文本缩放比例
	Ex_ObjScrollShow(m_hObjPage, SB_VERT, TRUE);

	// 子窗口是原生窗口，父窗口需要删除这个WS_EX_LAYERED风格
	SetWindowLongPtrW(hWnd_nativewindow, GWL_EXSTYLE, GetWindowLongPtrW(hWnd_nativewindow, GWL_EXSTYLE) & ~WS_EX_LAYERED);

	m_hWndChild = Ex_WndCreate(m_hWndNativeWindow2, 0, 0, 0, 0, 300, 300, WS_CHILD | WS_OVERLAPPEDWINDOW, 0);
	// 注意给子窗口添加一个回调，禁用移动和最大化
	auto hExDui_child = Ex_DUIBindWindowEx(m_hWndChild, 0, EWS_NOSHADOW, 0, OnNativeWndMsgProc);
	// 可以在子窗口创建原生win32组件,mfc组件,第三方组件诸如cef,miniblink,webview2,锐浪报表,aplayer
	// 子窗口也可以设置背景色
	Ex_DUISetLong(hExDui_child, EWL_CRBKG, ExARGB(0, 0, 0, 255));

	m_hWndChild2 = Ex_WndCreate(m_hWndNativeWindow2, 0, 0, 0, 0, 300, 300, WS_CHILD | WS_OVERLAPPEDWINDOW, 0);
	auto hExDui_child2 = Ex_DUIBindWindowEx(m_hWndChild2, 0, EWS_NOSHADOW, 0, OnNativeWndMsgProc);
	Ex_DUISetLong(hExDui_child2, EWL_CRBKG, ExARGB(0, 0, 0, 255));

	Ex_DUIShowWindow(hExDui_child, SW_SHOWNORMAL, 0, 0, 0);
	Ex_DUIShowWindow(hExDui_child2, SW_SHOWNORMAL, 0, 0, 0);
	Ex_DUIShowWindow(hExDui_nativewindow, SW_SHOWNORMAL, 0, 0, 0);
	Ex_DUIShowWindow(hExDui_nativewindow2, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnFullScreenWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_NCLBUTTONDBLCLK)
	{
		// 禁用标题栏双击最大化消息
		return 1;
	}
	else if (uMsg == WM_NCLBUTTONDOWN)
	{
		// 禁用标题栏鼠标按下拖动消息
		if (wParam == HTCAPTION)
		{
			return 1;
		}
	}
	return 0;
}

void test_fullscreen(HWND hWnd)
{
	HWND hWnd_fullscreen = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试全屏,最大化,置顶,不可移动改变大小,只能右上角关闭", 0, 0, 200, 200, 0, 0);
	HEXDUI hExDui_fullscreen = Ex_DUIBindWindowEx(hWnd_fullscreen, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_TITLE | EWS_CENTERWINDOW | EWS_FULLSCREEN | EWS_NOSHADOW | EWS_BUTTON_CLOSE, 0, OnFullScreenWndMsgProc);
	Ex_DUISetLong(hExDui_fullscreen, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	// 最大化
	PostMessageW(hWnd_fullscreen, 274, 61488, 0);
	// 置顶
	SetWindowPos(hWnd_fullscreen, (HWND)-1, 0, 0, 0, 0, 3);
	Ex_DUIShowWindow(hExDui_fullscreen, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hObjBrowser;

LRESULT CALLBACK OnMiniblinkWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE)
	{
		Ex_ObjMove(m_hObjBrowser, 50, 50, LOWORD(lParam) - 100, HIWORD(lParam) - 100, FALSE);
	}
	return 0;
}

LRESULT CALLBACK OnMiniblinkBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	//auto js = L"https://media.w3.org/2010/05/sintel/trailer.mp4";
	auto js = L"https://media.w3.org/2010/05/sintel/trailer.mp4";
	Ex_ObjSendMessage(m_hObjBrowser, MBBM_JS, 0, (LPARAM)js);
	return 0;
}

void test_miniblink(HWND hWnd)
{
	HWND hWndminiblink = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试miniblink浏览框", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_miniblink = Ex_DUIBindWindowEx(hWndminiblink, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON | EWS_SIZEABLE, 0, OnMiniblinkWndMsgProc);
	Ex_DUISetLong(hExDui_miniblink, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	//Ex_ObjMiniblinkBrowserInitialize(0, L"miniblink_4975_x64.dll"); // 64位
	Ex_ObjMiniblinkBrowserInitialize(0, L"miniblink_4975_x64.dll");
	m_hObjBrowser = Ex_ObjCreate(L"MbBrowser", NULL, -1, 50, 50, 700, 500, hExDui_miniblink);
	std::wstring path = L"file:///J:/ExduiR/msvc/test/res/MP4.html"; //注意,本地路径带#符号不支持
	Ex_ObjSendMessage(m_hObjBrowser, MBBM_LOAD, MBBL_TYPE_URL, (LPARAM)path.c_str());
	auto btn = Ex_ObjCreate(L"button", NULL, -1, 50, 550, 100, 30, hExDui_miniblink);
	Ex_ObjHandleEvent(btn, NM_CLICK, OnMiniblinkBtnEnevt);
	Ex_DUIShowWindow(hExDui_miniblink, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hObjMedia;
HEXOBJ m_hObjMediaButton1;
HEXOBJ m_hObjMediaButton2;
HEXOBJ m_hObjMediaButton3;
HEXOBJ m_hObjMediaButton4;
HEXOBJ m_hObjMediaButton5;

LRESULT CALLBACK OnMediaBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (hObj == m_hObjMediaButton1)
	{
		Ex_ObjSendMessage(m_hObjMedia, MFM_STATE_PLAY, 0, (LPARAM)L"./res/test.mp4");//可不带file:///前缀
	}
	else if (hObj == m_hObjMediaButton2)
	{
		Ex_ObjSendMessage(m_hObjMedia, MFM_STATE_PAUSE, 0, 0);
	}
	else if (hObj == m_hObjMediaButton3)
	{
		Ex_ObjSendMessage(m_hObjMedia, MFM_STATE_CONTINUE, 0, 0);
	}
	else if (hObj == m_hObjMediaButton4)
	{
		Ex_ObjSendMessage(m_hObjMedia, MFM_STATE_STOP, 0, 0);
	}
	else if (hObj == m_hObjMediaButton5)
	{
		Ex_ObjSendMessage(m_hObjMedia, MFM_SET_POSITION, 0, 20);
	}
	return 0;
}

void test_mediaPlay(HWND hWnd)
{
	HWND hWndmedia = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试媒体播放器", 0, 0, 800, 600, 0, 0);
	HEXDUI hExDui_media = Ex_DUIBindWindowEx(hWndmedia, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON | EWS_SIZEABLE, 0, OnMiniblinkWndMsgProc);
	Ex_DUISetLong(hExDui_media, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	m_hObjMedia = Ex_ObjCreate(L"MediaFoundation", NULL, -1, 50, 50, 700, 500, hExDui_media);

	m_hObjMediaButton1 = Ex_ObjCreate(L"button", L"播放", -1, 50, 560, 100, 30, hExDui_media);
	m_hObjMediaButton2 = Ex_ObjCreate(L"button", L"暂停", -1, 160, 560, 100, 30, hExDui_media);
	m_hObjMediaButton3 = Ex_ObjCreate(L"button", L"继续", -1, 270, 560, 100, 30, hExDui_media);
	m_hObjMediaButton4 = Ex_ObjCreate(L"button", L"停止", -1, 380, 560, 100, 30, hExDui_media);
	m_hObjMediaButton5 = Ex_ObjCreate(L"button", L"置位置", -1, 490, 560, 100, 30, hExDui_media);
	Ex_ObjHandleEvent(m_hObjMediaButton1, NM_CLICK, OnMediaBtnEnevt);
	Ex_ObjHandleEvent(m_hObjMediaButton2, NM_CLICK, OnMediaBtnEnevt);
	Ex_ObjHandleEvent(m_hObjMediaButton3, NM_CLICK, OnMediaBtnEnevt);
	Ex_ObjHandleEvent(m_hObjMediaButton4, NM_CLICK, OnMediaBtnEnevt);
	Ex_ObjHandleEvent(m_hObjMediaButton5, NM_CLICK, OnMediaBtnEnevt);
	Ex_DUIShowWindow(hExDui_media, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnsvgAndfontProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND) //wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
	{
		_canvas_clear(wParam, ExRGBA(150, 150, 150, 255));
		HEXFONT hFont = _font_createfromfile(L"res/文道灵飞小楷.ttf", 64);
		_canvas_drawtext(wParam, hFont, ExRGBA(200, 0, 200, 200), L"我是测试文本", -1, -1, 20, 450, 450, 530);
		_font_destroy(hFont);

		_canvas_drawsvgfromfile(wParam, L"./res/niu1.svg", 0, 50, 50, 200, 200);
		std::vector<CHAR> data;
		Ex_ReadFile(L"./res/niu1.svg", &data);
		_canvas_drawsvg(wParam, data.data(), ExRGBA(55, 250, 20, 255), 250, 50, 400, 200);
		_canvas_drawsvgfromfile(wParam, L"./res/niu1.svg", ExRGBA(55, 0, 250, 255), 50, 250, 200, 400);
		_canvas_drawsvgfromfile(wParam, L"./res/niu.svg", 0, 250, 250, 400, 450);

		*lpResult = 1;
		return 1;
	}
	return 0;
}

void test_svgAndfont(HWND hWnd)
{
	HWND hWnd_svgAndfont = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"自定义字体和SVG测试", 0, 0, 500, 600, 0, 0);
	HEXDUI hExDui_svgAndfont = Ex_DUIBindWindowEx(hWnd_svgAndfont, 0, EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE, 0, OnsvgAndfontProc);
	Ex_DUISetLong(hExDui_svgAndfont, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	Ex_DUIShowWindow(hExDui_svgAndfont, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ m_hObjRM = 0;
HEXIMAGELIST m_hImageListRollMenu;

LRESULT CALLBACK OnRollMenuWndBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 101)//删除分组
	{
		Ex_ObjSendMessage(m_hObjRM, RM_DELGROUP, 3, 0);
	}
	else if (nID == 102)//删除子项
	{
		Ex_ObjSendMessage(m_hObjRM, RM_DELITEM, 4, 2);
	}
	else if (nID == 103)//设置分组状态(展开/收缩)  wParam :分组索引(从1开始)  lParam: 状态(BOOL)
	{
		Ex_ObjSendMessage(m_hObjRM, RM_SETEXPAND, 2, 1);
	}
	else if (nID == 104)//设置分组状态(展开/收缩)  wParam :分组索引(从1开始)  lParam: 状态(BOOL)
	{
		Ex_ObjSendMessage(m_hObjRM, RM_SETEXPAND, 2, 0);
	}
	else if (nID == 105)//取 
	{
		int group = 0, item = 0;
		Ex_ObjSendMessage(m_hObjRM, RM_GETSEL, (WPARAM)&group, (LPARAM)&item);
		output(L"RM_GETSEL 所在分组:", group, L"选中子项:", item);
	}
	else if (nID == 106)//设置 
	{
		Ex_ObjSendMessage(m_hObjRM, RM_SETSEL, 4, 2);
	}
	return 0;
}

LRESULT CALLBACK OnRollMenuBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == RMN_CLICK)
	{
		output(L"RollMenu单击子项  子项:", wParam, L"所在分组: ", lParam);
	}
	return 0;
}

LRESULT CALLBACK OnRollMenuWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_CLOSE)
	{
		_imglist_destroy(m_hImageListRollMenu);
	}
	return 0;
}

void test_rollmenu(HWND hWnd)
{
	HWND hWndRM = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试卷帘菜单", 0, 0, 500, 500, 0, 0);
	HEXDUI hExDui_RM = Ex_DUIBindWindowEx(hWndRM, 0, EWS_NOINHERITBKG | EWS_MOVEABLE | EWS_SIZEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE | EWS_HASICON | EWS_SIZEABLE, 0, OnRollMenuWndMsgProc);
	Ex_DUISetLong(hExDui_RM, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	m_hObjRM = Ex_ObjCreateEx(-1, L"RollMenu", NULL, -1, 30, 50, 220, 400, hExDui_RM, 0, DT_VCENTER | DT_CENTER, 0, 0, NULL);//DT_LEFT
	Ex_ObjHandleEvent(m_hObjRM, RMN_CLICK, OnRollMenuBtnEvent);

	auto btn = Ex_ObjCreateEx(-1, L"button", L"删除分组", -1, 280, 150, 100, 30, hExDui_RM, 101, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
	btn = Ex_ObjCreateEx(-1, L"button", L"删除子项", -1, 280, 200, 100, 30, hExDui_RM, 102, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
	btn = Ex_ObjCreateEx(-1, L"button", L"展开分组", -1, 280, 250, 100, 30, hExDui_RM, 103, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
	btn = Ex_ObjCreateEx(-1, L"button", L"收缩分组", -1, 280, 300, 100, 30, hExDui_RM, 104, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
	btn = Ex_ObjCreateEx(-1, L"button", L"取现行选中子项", -1, 280, 350, 100, 30, hExDui_RM, 105, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
	btn = Ex_ObjCreateEx(-1, L"button", L"置现行选中子项", -1, 280, 400, 100, 30, hExDui_RM, 106, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);

	m_hImageListRollMenu = _imglist_create(32, 32);
	std::vector<CHAR> imgdata;
	Ex_ReadFile(L"res/account.png", &imgdata);
	size_t nImageAccountIndex = _imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0);
	Ex_ReadFile(L"res/video.png", &imgdata);
	size_t nImageVideoIndex = _imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0);
	Ex_ReadFile(L"res/info.png", &imgdata);
	size_t nImageInfoIndex = _imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0);
	Ex_ReadFile(L"res/fans.png", &imgdata);
	size_t nImageFansIndex = _imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0);
	Ex_ReadFile(L"res/authorization.png", &imgdata);
	size_t nImageAuthIndex = _imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0);

	std::vector<HEXIMAGE> emoji;
	Ex_ReadFile(L"navbtn/大图标1.png", &imgdata);
	emoji.push_back(_imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0));
	Ex_ReadFile(L"navbtn/大图标2.png", &imgdata);
	emoji.push_back(_imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0));
	Ex_ReadFile(L"navbtn/大图标3.png", &imgdata);
	emoji.push_back(_imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0));
	Ex_ReadFile(L"navbtn/大图标4.png", &imgdata);
	emoji.push_back(_imglist_add(m_hImageListRollMenu, imgdata.data(), imgdata.size(), 0));

	EX_ROLLMENU_DATA rollmenu = { 0 };
	rollmenu.title = L"账号信息";
	rollmenu.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageAccountIndex);
	rollmenu.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageAccountIndex);
	rollmenu.stateico.rc = { 40, 4, 72, 36 };
	Ex_ObjSendMessage(m_hObjRM, RM_ADDGROUP, 0, (LPARAM)&rollmenu);

	rollmenu.title = L"视频管理";
	rollmenu.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageVideoIndex);
	rollmenu.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageVideoIndex);
	rollmenu.stateico.rc = { 40, 4, 72, 36 };
	size_t groupVideoIndex = Ex_ObjSendMessage(m_hObjRM, RM_ADDGROUP, 0, (LPARAM)&rollmenu);
	EX_ROLLMENU_ITEM rollitemAuth = { 0 };
	rollitemAuth.title = L"视频权限";
	rollitemAuth.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageAuthIndex);
	rollitemAuth.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageAuthIndex);
	rollitemAuth.stateico.rc = { 55, 0, 83, 28 };
	Ex_ObjSendMessage(m_hObjRM, RM_ADDITEM, groupVideoIndex, (LPARAM)&rollitemAuth);
	EX_ROLLMENU_ITEM rollitem = { 0 };
	rollitem.title = L"视频列表";
	Ex_ObjSendMessage(m_hObjRM, RM_ADDITEM, groupVideoIndex, (LPARAM)&rollitem);

	rollmenu.title = L"数据分析";
	rollmenu.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageInfoIndex);
	rollmenu.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageInfoIndex);
	rollmenu.stateico.rc = { 40, 4, 72, 36 };
	Ex_ObjSendMessage(m_hObjRM, RM_ADDGROUP, 0, (LPARAM)&rollmenu);

	rollmenu.title = L"粉丝画像分析";
	rollmenu.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageFansIndex);
	rollmenu.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageFansIndex);
	rollmenu.stateico.rc = { 40, 4, 72, 36 };
	size_t groupFansIndex = Ex_ObjSendMessage(m_hObjRM, RM_ADDGROUP, 0, (LPARAM)&rollmenu);

	rollitem.extraico.rc = { 160, 2, 188 ,30 };
	std::wstring title;
	for (int i = 0; i < 20; i++)
	{
		title = L"粉丝_" + std::to_wstring(i + 1);
		rollitem.title = title.c_str();
		rollitem.extraico.icon = _imglist_get(m_hImageListRollMenu, emoji[Random(0, 3)]);
		Ex_ObjSendMessage(m_hObjRM, RM_ADDITEM, groupFansIndex, (LPARAM)&rollitem);
	}

	Ex_DUIShowWindow(hExDui_RM, SW_SHOWNORMAL, 0, 0, 0);
}

HEXDUI m_hExDui_tray;
HICON m_icon = 0;

LRESULT CALLBACK OnTrayButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	std::vector<CHAR> data;
	Ex_ReadFile(L"res\\icon.ico", &data);
	m_icon = (HICON)Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_ICON, 0);
	Ex_DUITrayIconSet(m_hExDui_tray, m_icon, L"ExDuiR");
	return 0;
}

LRESULT CALLBACK OnTrayWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_CLOSE)
	{
		if (m_icon != 0)
		{
			//销毁图标
			DeleteObject(m_icon);
			m_icon = 0;
		}
	}
	else if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
		if (ni.nCode == NM_TRAYICON)
		{
			INT16 nType = LOWORD(ni.lParam);
			if (nType == WM_RBUTTONDOWN)//右键按下托盘图标
			{
				Ex_DUITrayIconPopup(m_hExDui_tray, L"弹出托盘内容", L"弹出托盘标题", NIIF_INFO);
			}
		}
	}
	return 0;
}

void test_tray(HWND hWnd)
{
	HWND hWnd_tray = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试托盘图标", 0, 0, 250, 150, 0, 0);
	m_hExDui_tray = Ex_DUIBindWindowEx(hWnd_tray, 0, EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE, 0, OnTrayWndMsgProc);
	Ex_DUISetLong(m_hExDui_tray, EWL_CRBKG, ExARGB(150, 150, 150, 255));
	auto button1 = Ex_ObjCreateEx(-1, L"button", L"设置托盘", -1, 50, 30, 120, 30, m_hExDui_tray, 0, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(button1, NM_CLICK, OnTrayButtonEvent);
	Ex_ObjCreateEx(-1, L"static", L"右击托盘图标可以弹出托盘", -1, 10, 70, 200, 30, m_hExDui_tray, 0, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_DUIShowWindow(m_hExDui_tray, SW_SHOWNORMAL, 0, 0, 0);
}

LRESULT CALLBACK OnMaskObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_ERASEBKGND)
	{
		HEXIMAGE hImg1 = 0;
		HEXIMAGE hImg2 = 0;
		_img_createfromfile(L"mask/mask4.jpg", &hImg1);
		_img_createfromfile(L"mask/mask1.png", &hImg2);
		HEXIMAGE hImg3;
		_img_mask(hImg1, hImg2, 0, FALSE, &hImg3);
		_canvas_drawimagerect(wParam, hImg3, 0, 0, 200, 200, 255);
		_img_destroy(hImg1);
		_img_destroy(hImg2);
		_img_destroy(hImg3);
		*lpResult = 1;
		return 1;
	}
	return 0;
}

void test_mask(HWND hWnd)
{
	HWND hWnd_mask = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试蒙板", 0, 0, 300, 300, 0, 0);
	auto hExDui_mask = Ex_DUIBindWindowEx(hWnd_mask, 0, EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE, 0, 0);
	Ex_DUISetLong(hExDui_mask, EWL_CRBKG, ExRGB2ARGB(16711680, 255));

	Ex_ObjCreateEx(-1, L"static", L"", -1, 50, 50, 200, 200, hExDui_mask, 0, DT_VCENTER | DT_CENTER, 0, 0, OnMaskObjMsgProc);
	Ex_DUIShowWindow(hExDui_mask, SW_SHOWNORMAL, 0, 0, 0);
}

HEXOBJ hObj_taggingBoard;

LRESULT CALLBACK OnTaggingBoardEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	output(L"命中闭合路径", (size_t)lParam);
	return 0;
}

LRESULT CALLBACK OnTaggingButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 1010)
	{
		Ex_ObjSendMessage(hObj_taggingBoard, TBM_START, 0, 0);
	}
	else if (nID == 1020)
	{
		Ex_ObjSendMessage(hObj_taggingBoard, TBM_STOP, 0, 0);
	}
	else if (nID == 1030)
	{
		Ex_ObjSendMessage(hObj_taggingBoard, TBM_CLEAR, 0, 0);
		Ex_ObjSendMessage(hObj_taggingBoard, TBM_SET_BKG, 0, 0);
	}
	else if (nID == 1040)
	{
		auto arr = (EX_POLYGON_ARRAY*)Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_DATA, 0, 0);
		auto offsetLeft = (INT)Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_IMG_LEFT_OFFSET, 0, 0);
		auto offsetTop = (INT)Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_IMG_TOP_OFFSET, 0, 0);
		float scale;
		auto scalePtr = (LPVOID)Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_IMG_SCALE, 0, 0);
		RtlMoveMemory(&scale, scalePtr, 4);
		//最后一个是不闭合路径,不需要
		for (int i = 0; i < arr->count - 1; i++)
		{
			size_t ptrValue = 0;
			RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)), sizeof(size_t));
			EX_POlYGON* ptr = (EX_POlYGON*)ptrValue;
			if (ptr->count > 0)
			{
				for (int j = 0; j < ptr->count; j++)
				{
					FLOAT x = 0, y = 0;
					RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
					RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
					//缩放图坐标转原图坐标
					output(L"原图坐标 arr", i, L"index", j, L"x", (x - offsetLeft) * scale, L"y", (y - offsetTop) * scale);
				}
			}
		}
	}
	else if (nID == 1050)
	{
		HEXIMAGE img;
		_img_createfromfile(L"res//3.jpeg", &img);
		Ex_ObjSendMessage(hObj_taggingBoard, TBM_SET_BKG, 0, img);
		float scale;
		auto scalePtr = (LPVOID)Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_IMG_SCALE, 0, 0);
		RtlMoveMemory(&scale, scalePtr, 4);
		auto offsetLeft = (INT)Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_IMG_LEFT_OFFSET, 0, 0);
		auto offsetTop = (INT)Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_IMG_TOP_OFFSET, 0, 0);

		auto ptr = (EX_POlYGON*)malloc(sizeof(EX_POlYGON));
		const int size = 5;
		ptr->points = malloc(size * sizeof(POINT));
		ptr->count = size;
		int pointArrX[size] = { 356, 329, 331, 388, 388 };
		int pointArrY[size] = { 377, 398, 419, 419, 392 };


		for (int i = 0; i < size; i++)
		{
			//原图坐标转缩放图坐标
			FLOAT x = pointArrX[i] / scale + offsetLeft;
			FLOAT y = pointArrY[i] / scale + offsetTop;
			RtlMoveMemory((LPVOID)((size_t)ptr->points + i * 8), &x, 4);
			RtlMoveMemory((LPVOID)((size_t)ptr->points + i * 8 + 4), &y, 4);
		}

		auto arr = (EX_POLYGON_ARRAY*)malloc(sizeof(EX_POLYGON_ARRAY));
		arr->polygons = malloc(sizeof(size_t));
		arr->count = 0;
		size_t ptrValue = (size_t)ptr;
		RtlMoveMemory((LPVOID)((size_t)arr->polygons + arr->count * sizeof(size_t)), &ptrValue, sizeof(size_t));
		arr->count = arr->count + 1;
		Ex_ObjSendMessage(hObj_taggingBoard, TBM_SET_DATA, 0, (size_t)arr);
	}
	else if (nID == 1060)
	{
		INT hitPath = Ex_ObjSendMessage(hObj_taggingBoard, TBM_GET_HIT_PATH, 0, 0);
		if (hitPath > 0)
		{
			Ex_ObjSendMessage(hObj_taggingBoard, TBM_DELETE_PATH, 0, hitPath);
		}
	}
	return 0;
}

LRESULT CALLBACK OnTaggingWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE)
	{
		HDC dc = GetDC(NULL);
		FLOAT dpiy = (FLOAT)GetDeviceCaps(dc, 90) / 96;
		ReleaseDC(0, dc);
		//Ex_ObjMove(hObj_taggingBoard, 30, 30, (LOWORD(lParam) - 200) / dpiy, (HIWORD(lParam) - 50) / dpiy, TRUE);
	}
	return 0;
}

HEXOBJ hObj_taggingLabel1;
HEXOBJ hObj_taggingLabel2;

LRESULT CALLBACK OnTaggingBoradMouseMove(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	INT x = (INT)wParam;
	INT y = (INT)lParam;
	std::wstring xstr = L"x:" + std::to_wstring(x);
	std::wstring ystr = L"y:" + std::to_wstring(y);
	Ex_ObjSetText(hObj_taggingLabel1, xstr.c_str(), TRUE);
	Ex_ObjSetText(hObj_taggingLabel2, ystr.c_str(), TRUE);
	return 0;
}

void test_tagging(HWND hWnd)
{
	HWND hWnd_tagging = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试标注画板", 0, 0, 1200, 900, 0, 0);
	auto hExDui_tagging = Ex_DUIBindWindowEx(hWnd_tagging, 0, EWS_MOVEABLE | EWS_SIZEABLE | EWS_CENTERWINDOW | EWS_NOSHADOW | EWS_BUTTON_CLOSE | EWS_TITLE, 0, OnTaggingWndMsgProc);
	Ex_DUISetLong(hExDui_tagging, EWL_CRBKG, ExARGB(150, 150, 150, 255));

	hObj_taggingBoard = Ex_ObjCreateEx(-1, L"TaggingBoard", L"", -1, 30, 30, 1000, 800, hExDui_tagging, 0, DT_VCENTER | DT_CENTER, 0, 0, 0);
	auto hObj1 = Ex_ObjCreateEx(-1, L"button", L"开始绘图", -1, 1050, 30, 100, 30, hExDui_tagging, 1010, -1, 0, 0, 0);
	auto hObj2 = Ex_ObjCreateEx(-1, L"button", L"结束绘图", -1, 1050, 70, 100, 30, hExDui_tagging, 1020, -1, 0, 0, 0);
	auto hObj3 = Ex_ObjCreateEx(-1, L"button", L"清空绘图", -1, 1050, 110, 100, 30, hExDui_tagging, 1030, -1, 0, 0, 0);
	auto hObj4 = Ex_ObjCreateEx(-1, L"button", L"取出数据", -1, 1050, 150, 100, 30, hExDui_tagging, 1040, -1, 0, 0, 0);
	auto hObj5 = Ex_ObjCreateEx(-1, L"button", L"设置数据", -1, 1050, 190, 100, 30, hExDui_tagging, 1050, -1, 0, 0, 0);
	auto hObj6 = Ex_ObjCreateEx(-1, L"button", L"删除选中路径", -1, 1050, 230, 100, 30, hExDui_tagging, 1060, -1, 0, 0, 0);

	hObj_taggingLabel1 = Ex_ObjCreateEx(-1, L"static", L"x:", -1, 1050, 270, 60, 30, hExDui_tagging, 1070, -1, 0, 0, 0);
	hObj_taggingLabel2 = Ex_ObjCreateEx(-1, L"static", L"y:", -1, 1120, 270, 60, 30, hExDui_tagging, 1080, -1, 0, 0, 0);
	auto hObj7 = Ex_ObjCreateEx(-1, L"static", L"操作提示：\r\n1.点击【开始绘图】，鼠标在画板左键单击，开始绘制路径点，右键可以撤销点，达到3个点及以上可以闭合路径。 闭合路径后会自动调用【结束绘图】。此时再次点击【开始绘图】继续绘制下一条路径。\r\n2.绘制过程中点击【结束绘图】清空临时点。变为选中模式，可以选择画板上闭合的路径。\r\n3.点击【清空绘图】清空画板全部临时点和闭合路径。\r\n4.点击【取出数据】演示打印原图点坐标。\r\n5.按住CTRL键+鼠标滚轮,可以放大缩小", -1, 1050, 310, 130, 580, hExDui_tagging, 1090, DT_WORDBREAK, 0, 0, 0);
	Ex_ObjSetFontFromFamily(hObj7, L"微软雅黑", 15, EFS_BOLD, FALSE);
	Ex_ObjSetColor(hObj7, COLOR_EX_TEXT_NORMAL, ExARGB(133, 33, 53, 255), TRUE);

	Ex_ObjHandleEvent(hObj1, NM_CLICK, OnTaggingButtonEvent);
	Ex_ObjHandleEvent(hObj2, NM_CLICK, OnTaggingButtonEvent);
	Ex_ObjHandleEvent(hObj3, NM_CLICK, OnTaggingButtonEvent);
	Ex_ObjHandleEvent(hObj4, NM_CLICK, OnTaggingButtonEvent);
	Ex_ObjHandleEvent(hObj5, NM_CLICK, OnTaggingButtonEvent);
	Ex_ObjHandleEvent(hObj6, NM_CLICK, OnTaggingButtonEvent);

	Ex_ObjHandleEvent(hObj_taggingBoard, TBN_HIT_PATH, OnTaggingBoardEvent);
	Ex_ObjSendMessage(hObj_taggingBoard, TBM_SET_PEN_COLOR, 0, ExARGB(0, 255, 0, 255));

	HEXIMAGE img;
	_img_createfromfile(L"res//3.jpeg", &img);
	Ex_ObjSendMessage(hObj_taggingBoard, TBM_SET_BKG, 0, img);
	Ex_ObjHandleEvent(hObj_taggingBoard, TBN_MOUSE_MOVE, OnTaggingBoradMouseMove);

	Ex_DUIShowWindow(hExDui_tagging, SW_SHOWNORMAL, 0, 0, 0);
}