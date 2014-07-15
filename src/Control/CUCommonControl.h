/**  
*  @file	CUCommonControl.h   
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __CUCOMMONCONTROLS_H__
#define __CUCOMMONCONTROLS_H__

#pragma once

namespace CULib {

class CULIB_API CPictureBox:public CControlUI
{
public:
	CPictureBox();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
};

/** 标签控件 */
class CULIB_API CLabelUI : public CControlUI
{
public:
	CLabelUI();

	/** 返回控件名 */
	LPCTSTR GetClass() const;

	/** 得到对象指针 */
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 获取 OR 设置 文本样式 */
	void SetTextStyle(UINT uStyle);
	UINT GetTextStyle() const;

	/** 获取 OR 设置 可用状态文字颜色 */
	void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;

	/** 获取 OR 设置 不可用状态文字颜色 */
	void SetDisabledTextColor(DWORD dwTextColor);
	DWORD GetDisabledTextColor() const;

	/** 获取 OR 设置 控件使用的字体, 对应在字体列表中的序号 */
	void SetFont(int index);
	int GetFont() const;
	 /** 获取 OR 设置 文字与边框之间填充距离 */
	RECT GetTextPadding() const;
	void SetTextPadding(RECT rc);

	/** 获取 OR 设置 是否使用内嵌html标记格式 */
	bool IsShowHtml();
	void SetShowHtml(bool bShowHtml = true);

	/** 预估控件大小 */
	SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintText(HDC hDC);

protected:
	DWORD m_dwTextColor;
	DWORD m_dwDisabledTextColor;
	int m_iFont;
	UINT m_uTextStyle;
	RECT m_rcTextPadding;
	bool m_bShowHtml;
	TOOLINFO m_ToolTip;
	HWND m_hwndTooltip;
	bool m_bMouseTracking;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CButtonUI : public CLabelUI
{
public:
	CButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	/** 
	*功能 激活按钮控件
	*实现 判断是否活跃，如活跃发送通知
	*/
	bool Activate();

	/** 设置按钮控件是否可用 */
	void SetEnabled(bool bEnable = true);
	void DoEvent(TEventUI& event);

	/** 获取 OR 设置 正常状态下按钮背景图片 */
	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 鼠标悬浮时按钮背景图片 */
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 按钮被按下时背景图片 */
	LPCTSTR GetPushedImage();
	void SetPushedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 按钮获得焦点时背景图片 */
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 非可用状态下按钮背景图片 */
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);

	/** 设置/获取 鼠标悬时文本的颜色 */
	void SetHotTextColor(DWORD dwColor);
	DWORD GetHotTextColor() const;

	/** 设置/获取 按钮按下时文本颜色 */
	void SetPushedTextColor(DWORD dwColor);
	DWORD GetPushedTextColor() const;

	/** 设置/获取 按钮获得焦点时文本颜色 */
	void SetFocusedTextColor(DWORD dwColor);
	DWORD GetFocusedTextColor() const;
	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintText(HDC hDC);
	void PaintStatusImage(HDC hDC);

protected:
	UINT m_uButtonState;

	DWORD m_dwHotTextColor;
	DWORD m_dwPushedTextColor;
	DWORD m_dwFocusedTextColor;

	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sPushedImage;
	CStdString m_sFocusedImage;
	CStdString m_sDisabledImage;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API COptionUI : public CButtonUI
{
public:
	COptionUI();
	~COptionUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	bool Activate();
	void SetEnabled(bool bEnable = true);

	/** 获取 OR 设置 选中状态背景图片 */
	LPCTSTR GetSelectedImage();
	void SetSelectedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置选中文本颜色 */
	void SetSelectedTextColor(DWORD dwTextColor);
	DWORD GetSelectedTextColor();

	/** 获取 OR 设置 前景图片 */
	LPCTSTR GetForeImage();
	void SetForeImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 选项所属组 */
	LPCTSTR GetGroup() const;
	void SetGroup(LPCTSTR pStrGroupName = NULL);

	/** 获取 OR 设置 选项是否处于选中状态 */
	bool IsSelected() const;
	void Selected(bool bSelected);

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);

protected:
	bool m_bSelected;
	CStdString m_sGroupName;

	DWORD m_dwSelectedTextColor;

	CStdString m_sSelectedImage;
	CStdString m_sForeImage;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CTextUI : public CLabelUI
{
public:
	CTextUI();
	~CTextUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 得到当前控件中指定序号的超文本的内容 */
	CStdString* GetLinkContent(int iIndex);

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);

	void PaintText(HDC hDC);

protected:
	enum { MAX_LINK = 8 };

	/** 超文本数量 */
	int m_nLinks;

	/** 超文本数量 */
	RECT m_rcLinks[MAX_LINK];

	/** 超文本内容数组 */
	CStdString m_sLinks[MAX_LINK];
	int m_nHoverLink;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CProgressUI : public CLabelUI
{
public:
	CProgressUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 获取 OR 设置 进度条方向 */
	bool IsHorizontal();
	void SetHorizontal(bool bHorizontal = true);

	/** 获取 OR 设置 最小值 */
	int GetMinValue() const;
	void SetMinValue(int nMin);

	/** 获取 OR 设置 最大值 */
	int GetMaxValue() const;
	void SetMaxValue(int nMax);

	/** 获取 OR 设置 当前值 */
	int GetValue() const;
	void SetValue(int nValue);

	/** 获取 OR 设置 前景图 */
	LPCTSTR GetForeImage() const;
	void SetForeImage(LPCTSTR pStrImage);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);

protected:
	/** 进度条控件方向 */
	bool m_bHorizontal;
	int m_nMax;
	int m_nMin;
	int m_nValue;

	CStdString m_sForeImage;
	CStdString m_sForeImageModify;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CSliderUI : public CProgressUI
{
public:
	CSliderUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 设置滑块控件是否可用 */
	void SetEnabled(bool bEnable = true);

	/** 获取 OR 设置滑块步长 */
	int GetChangeStep();
	void SetChangeStep(int step);

	/** 获取 OR 设置滑块大小 */
	void SetThumbSize(SIZE szXY);
	RECT GetThumbRect() const;

	/** 获取 OR 设置滑块图 */
	LPCTSTR GetThumbImage() const;
	void SetThumbImage(LPCTSTR pStrImage);
	
	/** 获取 OR 设置鼠标悬浮状态滑块图 */
	LPCTSTR GetThumbHotImage() const;
	void SetThumbHotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置滑块按下状态滑块图 */
	LPCTSTR GetThumbPushedImage() const;
	void SetThumbPushedImage(LPCTSTR pStrImage);

	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);

protected:
	SIZE m_szThumb;
	
	/** 控件状态 */
	UINT m_uButtonState;

	/** 步长 */
	int m_nStep;

	/** 正常状态滑块块图 */
	CStdString m_sThumbImage;

	/** 鼠标悬浮状态滑块图 */
	CStdString m_sThumbHotImage;

	/** 按下状态滑块图 */
	CStdString m_sThumbPushedImage;

	CStdString m_sImageModify;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CEditWnd;

class CULIB_API CEditUI : public CLabelUI
{
	friend CEditWnd;
public:
	CEditUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	
	/** 得到控件的类型标志 */
	UINT GetControlFlags() const;

	/** 设置编辑框控件是否可用 */
	void SetEnabled(bool bEnable = true);

	/** 设置文本 */
	void SetText(LPCTSTR pstrText);

	/** 设置输入文本长度限制 */
	void SetMaxChar(UINT uMax);

	/** 获取最大输入文本长度 */
	UINT GetMaxChar();

	/** 设置和获取只读模式 */
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;

	/** 获取和设置密码编辑框模式 */
	void SetPasswordMode(bool bPasswordMode);
	bool IsPasswordMode() const;

	/** 设置和获取密码模式显示字符 */
	void SetPasswordChar(TCHAR cPasswordChar);
	TCHAR GetPasswordChar() const;

	/** 获取 OR 设置正常状态图 */
	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);

	/** 获取 OR 设置悬浮状态图 */
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置获得焦点状态图 */
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置非可用状态图 */
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);

	/** 获取 OR 设置原生编辑框背景色 */
	void SetNativeEditBkColor(DWORD dwBkColor);
	DWORD GetNativeEditBkColor() const;

	/** 得到编辑框位置 */
	void SetPos(RECT rc);

	/** 设置可见状态 */
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);

protected:
	/** 原生编辑框窗口 */
	CEditWnd* m_pWindow;

	/** 文本长度限制 */
	UINT m_uMaxChar;

	/** 只读方式 */
	bool m_bReadOnly;

	/** 显示密码模式 */
	bool m_bPasswordMode;

	/** 密码模式显示的字符 */
	TCHAR m_cPasswordChar;

	/** 编辑框状态 */
	UINT m_uButtonState;

	/** 不同状态的背景图 */
	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sFocusedImage;
	CStdString m_sDisabledImage;

	/** 编辑框背景色 */
	DWORD m_dwEditbkColor;
};

/////////////////////////////////////////////////////////////////////////////////////
//

/** 滚动条控件 */
class CULIB_API CScrollBarUI : public CControlUI
{
public:
	CScrollBarUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 得到和设置滚动条拥有者 */
	CContainerUI* GetOwner() const;
	void SetOwner(CContainerUI* pOwner);

	void SetVisible(bool bVisible = true);
	
	/** 设置滚动条是否可用 */
	void SetEnabled(bool bEnable = true);
	void SetFocus();

	/** 获取和设置滚动条方向 */
	bool IsHorizontal();
	void SetHorizontal(bool bHorizontal = true);

	/** 得到和设置滚动条范围 */ 
	int GetScrollRange() const;
	void SetScrollRange(int nRange);

	/** 得到和设置滚动块位置 */
	int GetScrollPos() const;
	void SetScrollPos(int nPos);

	/** 得到和设置步长大小 */
	int GetLineSize() const;
	void SetLineSize(int nSize);

	/** 获取 OR 设置 上或左位置的按钮显示状态 */
	bool GetShowButton1();
	void SetShowButton1(bool bShow);

	/** 获取 OR 设置 上或左位置的正常状态图 */
	LPCTSTR GetButton1NormalImage();
	void SetButton1NormalImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 上或左位置的悬浮状态图 */
	LPCTSTR GetButton1HotImage();
	void SetButton1HotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 上或左位置的按下状态图  */
	LPCTSTR GetButton1PushedImage();
	void SetButton1PushedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 上或左位置的非可用状态图 */
	LPCTSTR GetButton1DisabledImage();
	void SetButton1DisabledImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 下或右位置的按钮显示状态 */
	bool GetShowButton2();
	void SetShowButton2(bool bShow);

	/** 获取 OR 设置 下或右位置的正常状态图 */
	LPCTSTR GetButton2NormalImage();
	void SetButton2NormalImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 下或右位置的悬浮状态图 */
	LPCTSTR GetButton2HotImage();
	void SetButton2HotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 下或右位置的按下状态图  */
	LPCTSTR GetButton2PushedImage();
	void SetButton2PushedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 下或右位置的非可用状态图 */
	LPCTSTR GetButton2DisabledImage();
	void SetButton2DisabledImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动块 正常状态图 */
	LPCTSTR GetThumbNormalImage();
	void SetThumbNormalImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动块 悬浮状态图 */
	LPCTSTR GetThumbHotImage();
	void SetThumbHotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动块 按下状态图 */
	LPCTSTR GetThumbPushedImage();
	void SetThumbPushedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动块 非可用状态图 */
	LPCTSTR GetThumbDisabledImage();
	void SetThumbDisabledImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动轨迹 正常状态图 */
	LPCTSTR GetRailNormalImage();
	void SetRailNormalImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动轨迹 悬浮状态图 */
	LPCTSTR GetRailHotImage();
	void SetRailHotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动轨迹 按下状态图 */
	LPCTSTR GetRailPushedImage();
	void SetRailPushedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动轨迹 非可用状态图 */
	LPCTSTR GetRailDisabledImage();
	void SetRailDisabledImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动条 背景图 */
	LPCTSTR GetBkNormalImage();
	void SetBkNormalImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动条 悬浮状态图 */
	LPCTSTR GetBkHotImage();
	void SetBkHotImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动条 按下状态图 */
	LPCTSTR GetBkPushedImage();
	void SetBkPushedImage(LPCTSTR pStrImage);

	/** 获取 OR 设置 滚动条 非可用状态图 */
	LPCTSTR GetBkDisabledImage();
	void SetBkDisabledImage(LPCTSTR pStrImage);

	void SetPos(RECT rc);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void DoPaint(HDC hDC, const RECT& rcPaint);

	void PaintBk(HDC hDC);

	/** 绘制上或左向的按钮 */
	void PaintButton1(HDC hDC);
	/** 绘制下或右位置的按钮 */
	void PaintButton2(HDC hDC);

	/** 绘制滚动块 */
	void PaintThumb(HDC hDC);
	/** 绘制滚动轨迹 */
	void PaintRail(HDC hDC);

protected:

	enum { 
		/** 滚动条默认大小 */
		DEFAULT_SCROLLBAR_SIZE = 16,
		/** 默认计时器id */
		DEFAULT_TIMERID = 10,
	};
	/** 滚动条横纵方向 */
	bool m_bHorizontal;
	/** 范围 */
	int m_nRange;
	/** 滚动块位置 */
	int m_nScrollPos;
	/** 步长 */
	int m_nLineSize;
	/** 滚动条拥有者 */
	CContainerUI* m_pOwner;
	/** 鼠标位置 */
	POINT ptLastMouse;
	/** 滚动块位置 */
	int m_nLastScrollPos;
	/** 滚动块偏移量 未使用 */
	int m_nLastScrollOffset;
	int m_nScrollRepeatDelay;

	/** 各种状态的图形 */
	CStdString m_sBkNormalImage;
	CStdString m_sBkHotImage;
	CStdString m_sBkPushedImage;
	CStdString m_sBkDisabledImage;

	bool m_bShowButton1;
	RECT m_rcButton1;
	UINT m_uButton1State;
	CStdString m_sButton1NormalImage;
	CStdString m_sButton1HotImage;
	CStdString m_sButton1PushedImage;
	CStdString m_sButton1DisabledImage;

	bool m_bShowButton2;
	RECT m_rcButton2;
	UINT m_uButton2State;
	CStdString m_sButton2NormalImage;
	CStdString m_sButton2HotImage;
	CStdString m_sButton2PushedImage;
	CStdString m_sButton2DisabledImage;

	RECT m_rcThumb;
	UINT m_uThumbState;
	CStdString m_sThumbNormalImage;
	CStdString m_sThumbHotImage;
	CStdString m_sThumbPushedImage;
	CStdString m_sThumbDisabledImage;

	CStdString m_sRailNormalImage;
	CStdString m_sRailHotImage;
	CStdString m_sRailPushedImage;
	CStdString m_sRailDisabledImage;

	CStdString m_sImageModify;
};

} 

#endif 

