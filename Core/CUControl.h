/**  
*  @file	CUControl.h   
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __CUCONTROL_H__
#define __CUCONTROL_H__

#pragma once
#include "CUDelegate.h"
namespace CULib{

typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

/** 控件基类 */
class CULIB_API CControlUI
{
public:
	CControlUI();
	virtual ~CControlUI();

public:
	/** 得到和设置控件名称，名称为控件的唯一ID */
	virtual CStdString GetName() const;
	virtual void SetName(LPCTSTR pstrName);

	/** 得到控件类名 */
	virtual LPCTSTR GetClass() const;

	/** 得到控件指针 */
	virtual LPVOID GetInterface(LPCTSTR pstrName);

	/** 得到控件的类型标志 */
	virtual UINT GetControlFlags() const;

	/** 控件是否活跃 */
	virtual bool Activate();

	/** 得到和设置绘制管理者 */
	virtual CPaintManagerUI* GetManager() const;
	virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
	
	/** 得到父控件 */
	virtual CControlUI* GetParent() const;

	/**得到和设置控件显示内容 */
	virtual CStdString GetText() const;
	virtual void SetText(LPCTSTR pstrText);

	/** 得到和设置背景 */
	DWORD GetBkColor() const;
	void SetBkColor(DWORD dwBackColor);
	DWORD GetBkColor2() const;
	void SetBkColor2(DWORD dwBackColor);
	DWORD GetBkColor3() const;
	void SetBkColor3(DWORD dwBackColor);

	/** 得到和设置背景图片 */ 
	LPCTSTR GetBkImage();
	void SetBkImage(LPCTSTR pStrImage);

	/** 得到和设置边的颜色 */
	DWORD GetBorderColor() const;
	void SetBorderColor(DWORD dwBorderColor);

	/** 得到和设置获得焦点时边的颜色 */
	DWORD GetFocusBorderColor() const;
	void SetFocusBorderColor(DWORD dwBorderColor);
	bool IsColorHSL() const;
	void SetColorHSL(bool bColorHSL);

	/** 得到和设置边的宽度 */
	int GetBorderSize() const;
	void SetBorderSize(int nSize);

	/** 得到和设置圆角 */
	SIZE GetBorderRound() const;
	void SetBorderRound(SIZE cxyRound);

	/** 绘制图片 */
	bool DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

	/** 得到和设置控件位置 */
	virtual const RECT& GetPos() const;
	virtual void SetPos(RECT rc);

	/** 得到控件宽度和高度 */
	virtual int GetWidth() const;
	virtual int GetHeight() const;

	/** 得到控件横坐标和纵坐标位置 */
	virtual int GetX() const;
	virtual int GetY() const;

	/** 获得和设置外边距,由上层窗口绘制 */
	virtual RECT GetPadding() const;
	virtual void SetPadding(RECT rcPadding); 

	/** 实际大小位置使用GetPos获取，这里得到的是预设的参考值 */
	virtual SIZE GetFixedXY() const;   

	/** 仅float为true时有效 */
	virtual void SetFixedXY(SIZE szXY);  

	/** 实际大小位置使用GetPos获取，这里得到的是预设的参考值 */
	virtual int GetFixedWidth() const;       

	/** 预设的参考值 */
	virtual void SetFixedWidth(int cx);  

	/** 实际大小位置使用GetPos获取，这里得到的是预设的参考值 */
	virtual int GetFixedHeight() const;     

	/**  预设的参考值 */
	virtual void SetFixedHeight(int cy);  

	/** 获取或设置大小的限制值 */
	virtual int GetMinWidth() const;
	virtual void SetMinWidth(int cx);
	virtual int GetMaxWidth() const;
	virtual void SetMaxWidth(int cx);
	virtual int GetMinHeight() const;
	virtual void SetMinHeight(int cy);
	virtual int GetMaxHeight() const;
	virtual void SetMaxHeight(int cy);

	/** 设置相对位置比例 */
	virtual void SetRelativePos(SIZE szMove,SIZE szZoom);

	/** 设置父控件大小 */
	virtual void SetRelativeParentSize(SIZE sz);

	/** 得到相对位置比例 */
	virtual TRelativePosUI GetRelativePos() const;

	/** 是否使用相对位置 */
	virtual bool IsRelativePos() const;

	/** 鼠标提示 */
	virtual CStdString GetToolTip() const;
	virtual void SetToolTip(LPCTSTR pstrText);

	/** 快捷键 */
	virtual TCHAR GetShortcut() const;
	virtual void SetShortcut(TCHAR ch);

	/** 菜单 */
	virtual bool IsContextMenuUsed() const;
	virtual void SetContextMenuUsed(bool bMenuUsed);

	/** 用户属性 */
	virtual const CStdString& GetUserData(); 
	virtual void SetUserData(LPCTSTR pstrText); 
	virtual UINT_PTR GetTag() const; 
	virtual void SetTag(UINT_PTR pTag); 

	/** 一些重要的属性 */
	/** 获取 OR 设置 控件是否可见 */
	virtual bool IsVisible() const;
	virtual void SetVisible(bool bVisible = true);
	virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
	
	/** 获取 OR 设置 控件可用 */
	virtual bool IsEnabled() const;
	virtual void SetEnabled(bool bEnable = true);

	/** 获取 OR 设置 是否处理鼠标事件 */
	virtual bool IsMouseEnabled() const;
	virtual void SetMouseEnabled(bool bEnable = true);
	virtual bool IsKeyboardEnabled() const;
	virtual void SetKeyboardEnabled(bool bEnable = true);
	virtual bool IsFocused() const;
	virtual void SetFocus();
	virtual bool IsFloat() const;
	virtual void SetFloat(bool bFloat = true);

	//焦点控制
	virtual bool IsLeft()const;
	virtual bool IsRight()const;
	virtual bool IsUp()const;
	virtual bool IsDown()const;

	/** 查找控件 */
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	/** 设置无效 */
	void Invalidate();

	/** 获取 OR 设置 需更新状态 */
	bool IsUpdateNeeded() const;
	void NeedUpdate();
	void NeedParentUpdate();
	DWORD GetAdjustColor(DWORD dwColor);

	/** 初始化控件事件响应, 在SetManager()中被调用 */
	virtual void Init();
	virtual void DoInit();

	/** 事件响应函数 */
	virtual void Event(TEventUI& event);
	virtual void DoEvent(TEventUI& event);

	/** 设置属性, 根据解析后的名称对所关心的属性设置状态成员变量的值 */
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	CControlUI* ApplyAttributeList(LPCTSTR pstrList);

	/** 预估控件大小 */
	virtual SIZE EstimateSize(SIZE szAvailable);

	/** 
	*控件绘制
	*绘制循序：背景颜色->背景图->状态图->文本->边框
	*/
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);

	/** 绘制背景色 */
	virtual void PaintBkColor(HDC hDC);

	/** 绘制背景图片 */
	virtual void PaintBkImage(HDC hDC);

	/** 根据状态绘制背景图片 */
	virtual void PaintStatusImage(HDC hDC);

	/** 绘制文字 */
	virtual void PaintText(HDC hDC);

	/** 绘制边框 */
	virtual void PaintBorder(HDC hDC);

	/** 延时绘制 */
	virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

public:
	CEventSource OnInit;
	CEventSource OnDestroy;
	CEventSource OnSize;
	CEventSource OnEvent;
	CEventSource OnNotify;

protected:
	CPaintManagerUI* m_pManager;
	CControlUI* m_pParent;
	CStdString m_sName;
	bool m_bUpdateNeeded;
	bool m_bMenuUsed;
	RECT m_rcItem;
	RECT m_rcPadding;
	SIZE m_cXY;
	SIZE m_cxyFixed;
	SIZE m_cxyMin;
	SIZE m_cxyMax;
	bool m_bVisible;
	bool m_bInternVisible;
	bool m_bEnabled;
	bool m_bMouseEnabled;
	bool m_bKeyboardEnabled ;
	bool m_bFocused;
	bool m_bFloat;
	bool m_bSetPos; // 防止SetPos循环调用
	TRelativePosUI m_tRelativePos;

	CStdString m_sText;
	CStdString m_sToolTip;
	TOOLINFO m_ToolTip;
	HWND m_hwndTooltip;
	bool m_bMouseTracking;
	TCHAR m_chShortcut;
	CStdString m_sUserData;
	UINT_PTR m_pTag;

	DWORD m_dwBackColor;
	DWORD m_dwBackColor2;
	DWORD m_dwBackColor3;
	CStdString m_sBkImage;
	DWORD m_dwBorderColor;
	DWORD m_dwFocusBorderColor;
	bool m_bColorHSL;
	int m_nBorderSize;
	SIZE m_cxyBorderRound;
	RECT m_rcPaint;
};

}

#endif