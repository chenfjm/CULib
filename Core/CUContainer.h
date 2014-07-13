/**  
*  @file	CUContainer.h   
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __CUCONTAINER_H__
#define __CUCONTAINER_H__

namespace CULib{

class IContainerUI
{
public:
	virtual CControlUI* GetItemAt(int iIndex) const = 0;
	virtual int GetItemIndex(CControlUI* pControl) const  = 0;
	virtual bool SetItemIndex(CControlUI* pControl, int iIndex)  = 0;
	virtual int GetCount() const = 0;
	virtual bool Add(CControlUI* pControl,CControlUI* pParent=NULL) = 0;
	virtual bool AddAt(CControlUI* pControl, int iIndex)  = 0;
	virtual bool Remove(CControlUI* pControl) = 0;
	virtual bool RemoveAt(int iIndex)  = 0;
	virtual void RemoveAll() = 0;
};

class CScrollBarUI;

class CULIB_API CContainerUI : public CControlUI, public IContainerUI
{
public:

	CContainerUI();
	virtual ~CContainerUI();

public:
	/** 得到控件类名*/
	LPCTSTR GetClass() const;
	/** 得到控件指针 */
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 得到容器中控件项 */
	CControlUI* GetItemAt(int iIndex) const;
	/** 按序号获取和设置容器中控件项 */
	int GetItemIndex(CControlUI* pControl) const;
	bool SetItemIndex(CControlUI* pControl, int iIndex);
	/** 获取容器控件项数 */
	int GetCount() const;
	/** 增加一个控件到容器中 */
	bool Add(CControlUI* pControl,CControlUI* pParent=NULL);
	bool AddAt(CControlUI* pControl, int iIndex);
	/** 删除容器中控件项 */
	bool Remove(CControlUI* pControl);
	/** 删除指定序号的容器控件项 */
	bool RemoveAt(int iIndex);
	void RemoveAll();

	void DoEvent(TEventUI& event);
	/** 设置容器是否可见 */
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	/** 设置是否处理鼠标事件 */
	void SetMouseEnabled(bool bEnable = true);

	/** 获取 OR 设置内边距，相当于设置客户区 */
	virtual RECT GetInset() const;
	virtual void SetInset(RECT rcInset); 
	/** 获取和设置子控件间距 */
	virtual int GetChildPadding() const;
	virtual void SetChildPadding(int iPadding);
	/** 获取 OR 设置容器内控件项是否自动摧毁 */
	virtual bool IsAutoDestroy() const;
	virtual void SetAutoDestroy(bool bAuto);
	/** 获取 OR 设置容器本身是否延迟摧毁 */
	virtual bool IsDelayedDestroy() const;
	virtual void SetDelayedDestroy(bool bDelayed);
	/** 获取 OR 设置容器中控件是否响应鼠标事件 */
	virtual bool IsMouseChildEnabled() const;
	virtual void SetMouseChildEnabled(bool bEnable = true);
	
	/** 查找容器中下一个可选项 */
	virtual int FindSelectable(int iIndex, bool bForward = true) const;

	void SetPos(RECT rc);
	void DoPaint(HDC hDC, const RECT& rcPaint);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
	CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	/** 获取和设置滚动块位置 */
	virtual SIZE GetScrollPos() const;
	virtual void SetScrollPos(SIZE szPos);
	/** 获取滚动条范围 */
	virtual SIZE GetScrollRange() const;
	/** 向不同方向操纵滚动条 */
	virtual void LineUp();
	virtual void LineDown();
	virtual void PageUp();
	virtual void PageDown();
	virtual void HomeUp();
	virtual void EndDown();
	virtual void LineLeft();
	virtual void LineRight();
	virtual void PageLeft();
	virtual void PageRight();
	virtual void HomeLeft();
	virtual void EndRight();
	/** 滚动条可用 */
	virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
	/** 得到纵向和横向滚动条 */
	virtual CScrollBarUI* GetVerticalScrollBar() const;
	virtual CScrollBarUI* GetHorizontalScrollBar() const;

protected:
	/** 设置悬浮位置 */
	virtual void SetFloatPos(int iIndex);
	/** 处理滚动条 */
	virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);

protected:
	/** 容器中的空间项 */
	CStdPtrArray m_items;
	/** 设置内边距 客户区 */
	RECT m_rcInset;
	/** 控件项间距 */
	int m_iChildPadding;
	/** 自动摧毁子控件 */
	bool m_bAutoDestroy;
	bool m_bDelayedDestroy;
	/** 鼠标可用 */
	bool m_bMouseChildEnabled;
	/** 是否处理滚动 */
	bool m_bScrollProcess; 
	/** 横向和纵向滚动条 */
	CScrollBarUI* m_pVerticalScrollBar;
	CScrollBarUI* m_pHorizontalScrollBar;

	/** 指向当前容器的前一个和后一个容器 */
	CContainerUI* m_prevContainer;
	CContainerUI* m_nextContainer;
	
};

/** 纵向布局的容器控件 */
class CULIB_API CVerticalLayoutUI : public CContainerUI
{
public:
	CVerticalLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	/** 设置和获取分割栏高度 */
	void SetSepHeight(int iHeight);
	int GetSepHeight() const;
	/** 设置和获取刷新模式,立即刷新或延时刷新 */
	void SetSepImmMode(bool bImmediately);
	bool IsSepImmMode() const;
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);

	void SetPos(RECT rc);
	void DoPostPaint(HDC hDC, const RECT& rcPaint);
	/** 获取欲刷新的区域 */
	RECT GetThumbRect(bool bUseNew = false) const;

protected:
	/** 分隔栏高度 */
	int m_iSepHeight;
	/** 容器控件状态 */
	UINT m_uButtonState;
	/** 鼠标位置 */
	POINT ptLastMouse;
	/** 刷新区域 */
	RECT m_rcNewPos;
	/** 立即刷新模式 */
	bool m_bImmMode;
};

class CULIB_API CHorizontalLayoutUI : public CContainerUI
{
public:
	CHorizontalLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;
	/** 获取 OR 设置分隔栏宽度 */
	void SetSepWidth(int iWidth);
	int GetSepWidth() const;
	/** 获取 OR 设置刷新模式 */
	void SetSepImmMode(bool bImmediately);
	bool IsSepImmMode() const;
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);

	void SetPos(RECT rc);
	void DoPostPaint(HDC hDC, const RECT& rcPaint);

	/** 获取刷新区域 */
	RECT GetThumbRect(bool bUseNew = false) const;

protected:
	/** 分隔栏宽度*/
	int m_iSepWidth;
	/** 控件状态 */
	UINT m_uButtonState;
	/** 鼠标位置 */
	POINT ptLastMouse;
	/** 设置位置 */
	RECT m_rcNewPos;
	/** 立即刷新模式 */
	bool m_bImmMode;
};
/////////////////////////////////////////////////////////////////////////////////////
//
class CULIB_API CDialogLayoutUI : public CContainerUI
{
public:
	CDialogLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 设置拉伸模式 */
	void SetStretchMode(CControlUI* pControl, UINT uMode);

	void SetPos(RECT rc);
	SIZE EstimateSize(SIZE szAvailable);
	/** 重新计算区域 */
	RECT RecalcArea();   

protected:  
	/** 拉伸模式结构 */
	typedef struct 
	{
		CControlUI* pControl;
		UINT uMode;
		RECT rcItem;
	} STRETCHMODE;

	/** 拉伸模式列表 */
	CStdValArray m_aModes;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CTileLayoutUI : public CContainerUI
{
public:
	CTileLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 设置容器及其容器内控件项位置 */
	void SetPos(RECT rc);

	/** 获取 OR 设置子控件大小 */
	SIZE GetItemSize() const;
	void SetItemSize(SIZE szItem);
	/** 获取 OR 设置 布局列数 */
	int GetColumns() const;
	void SetColumns(int nCols);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	SIZE m_szItem;
	int m_nColumns;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CTabLayoutUI : public CContainerUI
{
public:
	CTabLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 向容器中添加控件，如果不是选中项则设置为非可见 */
	bool Add(CControlUI* pControl,CControlUI* pParent=NULL);
	bool AddAt(CControlUI* pControl, int iIndex);
	/** 移除控件 */
	bool Remove(CControlUI* pControl);
	void RemoveAll();
	/** 得到当前选中的控件 */
	int GetCurSel() const;
	/** 设置控件项为选中项 */
	bool SelectItem(int iIndex);

	/** 设置容器及其控件位置 */
	void SetPos(RECT rc);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	int m_iCurSel;
};

}

#endif