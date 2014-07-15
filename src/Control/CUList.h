/**  
*  @file    CUlist.h
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __CULIST_H__
#define __CULIST_H__

#pragma once

namespace CULib {

	class CListHeaderUI;

#define UILIST_MAX_COLUMNS 32

typedef struct tagTListInfoUI
{
	int nColumns;
	RECT rcColumn[UILIST_MAX_COLUMNS];
	int nFont;
	UINT uTextStyle;
	RECT rcTextPadding;
	DWORD dwTextColor;
	DWORD dwBkColor;
	CStdString sBkImage;
	bool bAlternateBk;
	DWORD dwSelectedTextColor;
	DWORD dwSelectedBkColor;
	CStdString sSelectedImage;
	DWORD dwHotTextColor;
	DWORD dwHotBkColor;
	CStdString sHotImage;
	DWORD dwDisabledTextColor;
	DWORD dwDisabledBkColor;
	CStdString sDisabledImage;
	DWORD dwLineColor;
	bool bShowHtml;
	bool bMultiExpandable;
} TListInfoUI;


/////////////////////////////////////////////////////////////////////////////////////
//

class IListCallbackUI
{
public:
	virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem) = 0;
};

class IListOwnerUI
{
public:
	virtual TListInfoUI* GetListInfo() = 0;
	virtual int GetCurSel() const = 0;
	virtual bool SelectItem(int iIndex, bool bTakeFocus = false) = 0;
	virtual void DoEvent(TEventUI& event) = 0;
};

class IListUI : public IListOwnerUI
{
public:
	virtual CListHeaderUI* GetHeader() const = 0;
	virtual CContainerUI* GetList() const = 0;
	virtual IListCallbackUI* GetTextCallback() const = 0;
	virtual void SetTextCallback(IListCallbackUI* pCallback) = 0;
	virtual bool ExpandItem(int iIndex, bool bExpand = true) = 0;
	virtual int GetExpandedItem() const = 0;
};

class IListItemUI
{
public:
	/** 得到和设置项序号 */
	virtual int GetIndex() const = 0;
	virtual void SetIndex(int iIndex) = 0;
	/** 获取和设置列表拥有者 */
	virtual IListOwnerUI* GetOwner() = 0;
	virtual void SetOwner(CControlUI* pOwner) = 0;
	/** 获取和设置被选中 */
	virtual bool IsSelected() const = 0;
	virtual bool Select(bool bSelect = true) = 0;
	/** 获取和设置是否展开项 */
	virtual bool IsExpanded() const = 0;
	virtual bool Expand(bool bExpand = true) = 0;
	/** 绘制项的文本 */
	virtual void DrawItemText(HDC hDC, const RECT& rcItem) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListBodyUI;
class CListHeaderUI;

class CULIB_API CListUI : public CVerticalLayoutUI, public IListUI
{
public:
	CListUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 获取和设置滚动条是否被选中 */
	bool GetScrollSelect();
	void SetScrollSelect(bool bScrollSelect);
	/** 得到当前选中项 */
	int GetCurSel() const;
	/** 设置当前选中项 可设置是否获得焦点 */
	bool SelectItem(int iIndex, bool bTakeFocus = false);

	/** 得到列表头指针 */
	CListHeaderUI* GetHeader() const;  
	/** 得到列表体指针 */
	CContainerUI* GetList() const;
	/** 得到列表属性 */
	TListInfoUI* GetListInfo();

	/** 按序号获取列表项 */
	CControlUI* GetItemAt(int iIndex) const;
	/** 按控件指针获取序号 */
	int GetItemIndex(CControlUI* pControl) const;
	/** 设置列表项 */
	bool SetItemIndex(CControlUI* pControl, int iIndex);
	/** 得到列表项数 */
	int GetCount() const;
	/** 加入一个控件 */
	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);
	/** 移除控件 */
	bool Remove(CControlUI* pControl);
	bool RemoveAt(int iIndex);
	void RemoveAll();

	/** 确保显示第iIndex项 */
	void EnsureVisible(int iIndex);
	/** 滚动 */
	void Scroll(int dx, int dy);

	/** 获取和设置子控件间距 */
	int GetChildPadding() const;
	void SetChildPadding(int iPadding);

	/** 设置字体 */
	void SetItemFont(int index);
	/** 设置文本类型 */
	void SetItemTextStyle(UINT uStyle);
	/** 设置文本间距 */
	void SetItemTextPadding(RECT rc);
	/** 设置文本色 背景色 背景图 */
	void SetItemTextColor(DWORD dwTextColor);
	void SetItemBkColor(DWORD dwBkColor);
	void SetItemBkImage(LPCTSTR pStrImage);

	/** 设置备用背景属性 */
	void SetAlternateBk(bool bAlternateBk);
	/** 设置选中项文本色 背景色 背景图*/
	void SetSelectedItemTextColor(DWORD dwTextColor);
	void SetSelectedItemBkColor(DWORD dwBkColor);
	void SetSelectedItemImage(LPCTSTR pStrImage); 

	/** 设置活跃项文本色 背景色 背景图*/
	void SetHotItemTextColor(DWORD dwTextColor);
	void SetHotItemBkColor(DWORD dwBkColor);
	void SetHotItemImage(LPCTSTR pStrImage);

	/** 设置非可用项文本色 背景色 背景图 */
	void SetDisabledItemTextColor(DWORD dwTextColor);
	void SetDisabledItemBkColor(DWORD dwBkColor);
	void SetDisabledItemImage(LPCTSTR pStrImage);

	/** 设置和获取项边框色 */
	void SetItemLineColor(DWORD dwLineColor);
	DWORD GetItemLineColor() const;

	/** 获取和设置项是否使用内嵌html标识 */
	bool IsItemShowHtml();
	void SetItemShowHtml(bool bShowHtml = true);
	/** 获取文本间距 */
	RECT GetItemTextPadding() const;
	/** 获取文本颜色 背景色 背景图 */
	DWORD GetItemTextColor() const;
	DWORD GetItemBkColor() const;
	LPCTSTR GetItemBkImage() const;

	/** 获取备用背景属性 */
	bool IsAlternateBk() const;
	/** 获取选中项文本色 背景色 背景图 */
	DWORD GetSelectedItemTextColor() const;
	DWORD GetSelectedItemBkColor() const;
	LPCTSTR GetSelectedItemImage() const;

	/** 获取激活状态项文本色 背景色 背景图 */
	DWORD GetHotItemTextColor() const;
	DWORD GetHotItemBkColor() const;
	LPCTSTR GetHotItemImage() const;

	/** 获取非可用项文本色 背景色 背景图 */
	DWORD GetDisabledItemTextColor() const;
	DWORD GetDisabledItemBkColor() const;
	LPCTSTR GetDisabledItemImage() const;

	/** 设置多项展开属性 */
	void SetMultiExpanding(bool bMultiExpandable); 
	/** 获取展开项序号 */
	int GetExpandedItem() const;
	/** 展开项 */
	bool ExpandItem(int iIndex, bool bExpand = true);

	void SetPos(RECT rc);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	/** 获取和设置文本回调 */
	IListCallbackUI* GetTextCallback() const;
	void SetTextCallback(IListCallbackUI* pCallback);

	/** 获取滚动条位置 */
	SIZE GetScrollPos() const;
	/** 获取和设置滚动条位置 */
	SIZE GetScrollRange() const;
	void SetScrollPos(SIZE szPos);
	/** 向各个方向操纵滚动条 */
	void LineUp();
	void LineDown();
	void PageUp();
	void PageDown();
	void HomeUp();
	void EndDown();
	void LineLeft();
	void LineRight();
	void PageLeft();
	void PageRight();
	void HomeLeft();
	void EndRight();
	/** 设置是否使用滚动条 */
	void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
	/** 获取横向和纵向滚动条 */
	virtual CScrollBarUI* GetVerticalScrollBar() const;
	virtual CScrollBarUI* GetHorizontalScrollBar() const;

protected:
	/** 滚动条被选中 */
	bool m_bScrollSelect;
	/** 当前选中项 */
	int m_iCurSel;
	/** 展开项 */
	int m_iExpandedItem;
	/** 回调 */
	IListCallbackUI* m_pCallback;
	/** 列体指针 */
	CListBodyUI* m_pList;
	/** 表头 */
	CListHeaderUI* m_pHeader;
	/** 表属性 */
	TListInfoUI m_ListInfo;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CListBodyUI : public CVerticalLayoutUI
{
public:
	CListBodyUI(CListUI* pOwner);

	/** 设置滚动条位置 */
	void SetScrollPos(SIZE szPos);
	void SetPos(RECT rc);
	void DoEvent(TEventUI& event);

protected:
	/** 拥有者指针 */
	CListUI* m_pOwner;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CListHeaderUI : public CHorizontalLayoutUI
{
public:
	CListHeaderUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 计算大小*/
	SIZE EstimateSize(SIZE szAvailable);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CListHeaderItemUI : public CControlUI
{
public:
	CListHeaderItemUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void SetEnabled(bool bEnable = true);

	/** 获取和设置是否支持拖拽 */
	bool IsDragable() const;
	void SetDragable(bool bDragable);
	/** 获取和设置分隔条宽度 */
	DWORD GetSepWidth() const;
	void SetSepWidth(int iWidth);
	/** 获取和设置文本类型 */
	DWORD GetTextStyle() const;
	void SetTextStyle(UINT uStyle);
	/** 获取和设置文本颜色 */
	DWORD GetTextColor() const;
	void SetTextColor(DWORD dwTextColor);
	/** 设置和获取文本间距 */
	void SetTextPadding(RECT rc);
	RECT GetTextPadding() const;
	/** 设置字体 */
	void SetFont(int index);
	/** 获取和设置是否使用内嵌html标记 */
	bool IsShowHtml();
	void SetShowHtml(bool bShowHtml = true);
	/** 获取和设置正常状态图 */
	LPCTSTR GetNormalImage() const;
	void SetNormalImage(LPCTSTR pStrImage);
	/** 获取和设置激活状态图  */
	LPCTSTR GetHotImage() const;
	void SetHotImage(LPCTSTR pStrImage);
	/** 获取和设置按下状态图 */
	LPCTSTR GetPushedImage() const;
	void SetPushedImage(LPCTSTR pStrImage);
	/** 获取和设置焦点状态图 */
	LPCTSTR GetFocusedImage() const;
	void SetFocusedImage(LPCTSTR pStrImage);
	/** 获取和设置分隔条背景图 */
	LPCTSTR GetSepImage() const;
	void SetSepImage(LPCTSTR pStrImage);

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	/** 获取宽度 */
	RECT GetThumbRect() const;

	void PaintText(HDC hDC);
	void PaintStatusImage(HDC hDC);

protected:
	/** 鼠标位置 */
	POINT ptLastMouse;
	/** 可拖拽属性 */
	bool m_bDragable;
	/** 控件状态 */
	UINT m_uButtonState;
	/** 分隔条宽度 */
	int m_iSepWidth;
	/** 文本颜色 */
	DWORD m_dwTextColor;
	/** 字体 */
	int m_iFont;
	/** 文本类型 */
	UINT m_uTextStyle;
	/** 是否使用内嵌html */
	bool m_bShowHtml;
	/** 文本间距 */
	RECT m_rcTextPadding;

	/** 各种状态图 */
	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sPushedImage;
	CStdString m_sFocusedImage;
	/** 分隔条背景图 */
	CStdString m_sSepImage;
	CStdString m_sSepImageModify;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CListElementUI : public CControlUI, public IListItemUI
{
public:
	CListElementUI();

	LPCTSTR GetClass() const;
	/** 获取控件标志 */
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetEnabled(bool bEnable = true);

	/** 得到和设置列表元素序号 */
	int GetIndex() const;
	void SetIndex(int iIndex);

	/** 获取和设置列表元素拥有者 */
	IListOwnerUI* GetOwner();
	void SetOwner(CControlUI* pOwner);
	/** 设置列表元素可见性 */
	void SetVisible(bool bVisible = true);

	/** 获取和设置被选中 */
	bool IsSelected() const;
	bool Select(bool bSelect = true);
	bool IsExpanded() const;
	bool Expand(bool bExpand = true);

	/** 使区域失效,直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域 */
	void Invalidate(); 
	/** 激活列表元素状态 */
	bool Activate();

	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	/** 绘制列表元素背景色 */
	void DrawItemBk(HDC hDC, const RECT& rcItem);

protected:
	/** 列表元素序列号 */
	int m_iIndex;
	/** 是否被选中 */
	bool m_bSelected;
	/** 列表元素状态 */
	UINT m_uButtonState;
	/** 列表元素拥有者 */
	IListOwnerUI* m_pOwner;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CListLabelElementUI : public CListElementUI
{
public:
	CListLabelElementUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	/** 绘制列表项文本 */
	void DrawItemText(HDC hDC, const RECT& rcItem);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CListTextElementUI : public CListLabelElementUI
{
public:
	CListTextElementUI();
	~CListTextElementUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	/** 得到和设置文本 */
	LPCTSTR GetText(int iIndex) const;
	void SetText(int iIndex, LPCTSTR pstrText);

	/** 设置拥有者 */
	void SetOwner(CControlUI* pOwner);
	/** 得到超连接内容 */
	CStdString* GetLinkContent(int iIndex);

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);
	/** 绘制文本 */
	void DrawItemText(HDC hDC, const RECT& rcItem);

protected:
	/** 最大超连接数 */
	enum { MAX_LINK = 8 };
	/** 当前超链接数目 */
	int m_nLinks;
	/** 链接文本区域 */
	RECT m_rcLinks[MAX_LINK];
	/** 链接文本文字 */
	CStdString m_sLinks[MAX_LINK];
	int m_nHoverLink;
	/** 拥有者 */
	IListUI* m_pOwner;
	/** 列项的文本 */
	CStdPtrArray m_aTexts;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CULIB_API CListContainerElementUI : public CContainerUI, public IListItemUI
{
public:
	CListContainerElementUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	int GetIndex() const;
	void SetIndex(int iIndex);

	IListOwnerUI* GetOwner();
	void SetOwner(CControlUI* pOwner);
	void SetVisible(bool bVisible = true);
	/** 设置是否可用 */
	void SetEnabled(bool bEnable = true);

	/** 获取和设置是否被选中 */
	bool IsSelected() const;
	bool Select(bool bSelect = true);

	/** 获取和设置是否被展开 */
	bool IsExpanded() const;
	bool Expand(bool bExpand = true);

	/**  直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域 */
	void Invalidate(); 
	bool Activate();

	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoPaint(HDC hDC, const RECT& rcPaint);

	void DrawItemText(HDC hDC, const RECT& rcItem);    
	void DrawItemBk(HDC hDC, const RECT& rcItem);

protected:
	int m_iIndex;
	bool m_bSelected;
	UINT m_uButtonState;
	IListOwnerUI* m_pOwner;
};

} 

#endif // __UILIST_H__
