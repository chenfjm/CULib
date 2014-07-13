#ifndef __UIRICHEDIT_H__
#define __UIRICHEDIT_H__
#include "CULib.h"
#include <math.h>

#pragma once

namespace CULib {

class CRichEditWnd;

class CULIB_API CRichEditUI : public CContainerUI
{
	friend CRichEditWnd;
public:
	CRichEditUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT CRichEditUI::GetControlFlags()const;

///////////////////////////////////////////////////////////label
	void SetTextStyle(UINT uStyle);
	UINT GetTextStyle() const;
	void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;
	void SetDisabledTextColor(DWORD dwTextColor);
	DWORD GetDisabledTextColor() const;
	void SetFont(int index);
	int GetFont() const;
	RECT GetTextPadding() const;
	void SetTextPadding(RECT rc);
	bool IsShowHtml();
	void SetShowHtml(bool bShowHtml = true);

//	void DoEvent(TEventUI& event);
//	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

//	void PaintText(HDC hDC);
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////label	
protected:
	DWORD m_dwTextColor;
	DWORD m_dwDisabledTextColor;
	int m_iFont;
	UINT m_uTextStyle;
	RECT m_rcTextPadding;
	bool m_bShowHtml;
///////////////////////////////////////////////////////label

///////////////////////////////////////////////////////edit
public:
	void SetEnabled(bool bEnable = true);
	void SetText(LPCTSTR pstrText);
//	void SetMaxChar(UINT uMax);
//	UINT GetMaxChar();
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	void SetNativeEditBkColor(DWORD dwBkColor);
	DWORD GetNativeEditBkColor() const;

	void SetPos(RECT rc);
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
//	SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
//½¹µã¿ØÖÆ
	bool IsLeft()const;
	bool IsRight()const;
	bool IsUp()const;
	bool IsDown()const;


	void PaintText(HDC hDC);
	void DoPaint(HDC hDC, const RECT& rcPaint);
protected:
	CRichEditWnd* m_pWindow;

	
	bool m_bReadOnly;
	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sFocusedImage;
	CStdString m_sDisabledImage;
	DWORD m_dwEditbkColor;	
};

}

#endif // __UIRICHEDIT_H__
