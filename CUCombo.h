/**  
*  @file    CUCombo.h
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __CUCOMBO_H__
#define __CUCOMBO_H__

#pragma once

namespace CULib {
//////////////////////////////////////////////////////////////////////////

class CComboWnd;

/** 组合控件 */
class CULIB_API CComboUI : public CContainerUI, public IListOwnerUI
{
	friend CComboWnd;
public:
	CComboUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void DoInit();
	/** 返回tab切换停留属性 */
	UINT GetControlFlags() const;
	/** 得到文本 */
	CStdString GetText() const;
	void SetEnabled(bool bEnable = true);

	/** 获取或设置下拉框属性列表字符串 */
	CStdString GetDropBoxAttributeList();
	void SetDropBoxAttributeList(LPCTSTR pstrList);
	
	/** 得到和设置下拉框大小 */
	SIZE GetDropBoxSize() const;
	void SetDropBoxSize(SIZE szDropBox);

	/** 获取当前选中项 */
	int GetCurSel() const;  
	/** 设置当前选中项 */
	bool SelectItem(int iIndex, bool bTakeFocus = false);
	/** 按项的控件指针获取项序号 */
	bool SetItemIndex(CControlUI* pControl, int iIndex);

	/** 加入一项 */
	bool Add(CControlUI* pControl,CControlUI* pParent=NULL);
	/** 加入一项到列表指定位置 */
	bool AddAt(CControlUI* pControl, int iIndex);
	/** 按项控件的指针移除一项 */
	bool Remove(CControlUI* pControl);
	/** 移除指定序号的一项 */
	bool RemoveAt(int iIndex);
	/** 移除所有项 */
	void RemoveAll();

	/** 设置控件活跃 */
	bool Activate();

	/** 获取和设置文本与控件边缘的间距 */
	RECT GetTextPadding() const;
	void SetTextPadding(RECT rc);

	/** 获取和设置正常状态图 */
	LPCTSTR GetNormalImage() const;
	void SetNormalImage(LPCTSTR pStrImage);

	/** 获取和设置活跃状态图 */
	LPCTSTR GetHotImage() const;
	void SetHotImage(LPCTSTR pStrImage);

	/** 获取和设置按下状态图 */
	LPCTSTR GetPushedImage() const;
	void SetPushedImage(LPCTSTR pStrImage);

	/** 获取和设置焦点状态图 */
	LPCTSTR GetFocusedImage() const;
	void SetFocusedImage(LPCTSTR pStrImage);

	/** 获取和设置非可用状态图 */
	LPCTSTR GetDisabledImage() const;
	void SetDisabledImage(LPCTSTR pStrImage);

	/** 获取列表信息 */
	TListInfoUI* GetListInfo();
	/** 设置项字体 */
	void SetItemFont(int index);
	/** 设置项字体大小 */
	void SetItemTextStyle(UINT uStyle);
	/** 获取和设置项文本间距 */
	RECT GetItemTextPadding() const;
	void SetItemTextPadding(RECT rc);
	/** 得到和设置项目文本色 */
	DWORD GetItemTextColor() const;
	void SetItemTextColor(DWORD dwTextColor);
	/** 获取和设置项背景色 */
	DWORD GetItemBkColor() const;
	void SetItemBkColor(DWORD dwBkColor);
	/** 获取和设置项图 */
	LPCTSTR GetItemBkImage() const;
	void SetItemBkImage(LPCTSTR pStrImage);
	bool IsAlternateBk() const;
	void SetAlternateBk(bool bAlternateBk);
	
	/** 得到和设置选中状态文本色 */
	DWORD GetSelectedItemTextColor() const;
	void SetSelectedItemTextColor(DWORD dwTextColor);
	/** 得到和设置项选中状态背景图 */
	DWORD GetSelectedItemBkColor() const;
	void SetSelectedItemBkColor(DWORD dwBkColor);
	/** 得到和设置项选中状态背景图 */
	LPCTSTR GetSelectedItemImage() const;
	void SetSelectedItemImage(LPCTSTR pStrImage);
	/** 得到和设置项活跃状态文本色 */
	DWORD GetHotItemTextColor() const;
	void SetHotItemTextColor(DWORD dwTextColor);
	/** 得到和设置项活跃状态背景色 */
	DWORD GetHotItemBkColor() const;
	void SetHotItemBkColor(DWORD dwBkColor);
	/** 设置项活跃状态图 */
	LPCTSTR GetHotItemImage() const;
	void SetHotItemImage(LPCTSTR pStrImage);
	/** 得到和设置非可用项文本色 */
	DWORD GetDisabledItemTextColor() const;
	void SetDisabledItemTextColor(DWORD dwTextColor);
	/** 得到和设置项非可用状态背景色 */
	DWORD GetDisabledItemBkColor() const;
	void SetDisabledItemBkColor(DWORD dwBkColor);
	/** 获取和设置项非可用状态图 */
	LPCTSTR GetDisabledItemImage() const;
	void SetDisabledItemImage(LPCTSTR pStrImage);
	/** 获取和设置项边框色 */
	DWORD GetItemLineColor() const;
	void SetItemLineColor(DWORD dwLineColor);
	/** 获取和设置项是否使用内嵌html标识语言 */
	bool IsItemShowHtml();
	void SetItemShowHtml(bool bShowHtml = true);

	SIZE EstimateSize(SIZE szAvailable);
	void SetPos(RECT rc);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void DoPaint(HDC hDC, const RECT& rcPaint);
	void PaintText(HDC hDC);
	void PaintStatusImage(HDC hDC);

protected:
	/** 下拉框窗口 */
	CComboWnd* m_pWindow;
	/** 当前选中项 */
	int m_iCurSel;
	/** 文本与控件边缘间距 */
	RECT m_rcTextPadding;
	/** 下拉框属性 */
	CStdString m_sDropBoxAttributes;
	/** 下拉框窗体大小 */
	SIZE m_szDropBox;
	/** 控件状态 */
	UINT m_uButtonState;

	/** 各种状态图 */
	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sPushedImage;
	CStdString m_sFocusedImage;
	CStdString m_sDisabledImage;

	/** 下拉框项的列表 */
	TListInfoUI m_ListInfo;
};

} // namespace DuiLib

#endif // __UICOMBO_H__
