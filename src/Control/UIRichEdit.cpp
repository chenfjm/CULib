#include "UIRichEdit.h"


//#ifdef SUPPORT_RICHEDIT//CE不支持richedit
namespace CULib {

class CRichEditWnd:public CWindowWnd
{
public:
	CRichEditWnd();
	void Init(CRichEditUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName()const;
	LPCTSTR GetSuperClassName()const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	CRichEditUI* m_pOwner;
	HBRUSH m_hBkBrush;
	bool m_bInit;
};

CRichEditWnd::CRichEditWnd():m_pOwner(NULL),m_hBkBrush(NULL),m_bInit(false)
{

}

void CRichEditWnd::Init(CRichEditUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcPos = CalPos();
	UINT uStyle = WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_AUTOVSCROLL;
	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
	SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetDefaultFontInfo()->hFont, TRUE);
	Edit_SetText(m_hWnd, m_pOwner->GetText());
	Edit_SetModify(m_hWnd, FALSE);
	SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
	Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
	Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);
	m_bInit = true;    

}

RECT CRichEditWnd::CalPos()
{
	CRect rcPos = m_pOwner->GetPos();
	RECT rcInset = m_pOwner->m_rcInset;
	rcPos.left += rcInset.left;
	rcPos.top += rcInset.top;
	rcPos.right -= rcInset.right;
	rcPos.bottom -= rcInset.bottom;

	//if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
	//if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

	/*if (m_pOwner->m_pVerticalScrollBar&&m_pOwner->m_pVerticalScrollBar->IsVisible())
	{
		rcPos.right-=m_pOwner->m_pVerticalScrollBar->GetFixedWidth();
	}

	if (m_pOwner->m_pHorizontalScrollBar&&m_pOwner->m_pHorizontalScrollBar->IsVisible())
	{
		rcPos.bottom-=m_pOwner->m_pHorizontalScrollBar->GetFixedHeight();
	}*/

	
	return rcPos;
}

LPCTSTR CRichEditWnd::GetWindowClassName()const
{
	return _T("RichEditWnd");
}

LPCTSTR CRichEditWnd::GetSuperClassName() const
{
	return WC_EDIT;
}

void CRichEditWnd::OnFinalMessage(HWND /*hWnd*/)
{
	// Clear reference and die
	if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
	m_pOwner->m_pWindow = NULL;
	delete this;
}


LRESULT CRichEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	PostMessage(WM_CLOSE);
	return lRes;
}

LRESULT CRichEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
	else if( uMsg == OCM_COMMAND ) {
		if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE )	lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
		else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		}
	}
	else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) {
		m_pOwner->GetManager()->SendNotify(m_pOwner, _T("return"));
	}
	else if (uMsg==OCM__BASE+WM_CTLCOLORSTATIC||uMsg==OCM__BASE+WM_CTLCOLOREDIT)
	{
		if( m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;

		DWORD clrColor=m_pOwner->GetNativeEditBkColor();
		::SetBkColor((HDC)wParam,RGB(GetBValue(clrColor),GetGValue(clrColor),GetRValue(clrColor)));
		//::SetBkMode((HDC)wParam,TRANSPARENT);
		DWORD dwTextColor = m_pOwner->GetTextColor();
		::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
		if( m_hBkBrush == NULL ) {
			DWORD clrColor = m_pOwner->GetNativeEditBkColor();
			m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
		}

		return (LRESULT)m_hBkBrush;
	}
	
	else bHandled = FALSE;
	if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
	return CWindowWnd::HandleMessage(uMsg,wParam,lParam);
}


LRESULT CRichEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( !m_bInit ) return 0;
	if( m_pOwner == NULL ) return 0;
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if( pstr == NULL ) return 0;
	::GetWindowText(m_hWnd, pstr, cchLen);
	m_pOwner->m_sText = pstr;
	m_pOwner->GetManager()->SendNotify(m_pOwner, _T("textchanged"));
	return 0;
}





//////////////////////////////////////////////////////////////////////////////////
//

CRichEditUI::CRichEditUI() : m_uTextStyle(DT_LEFT|DT_TOP), m_dwTextColor(0), 
m_dwDisabledTextColor(0), m_iFont(-1), m_bShowHtml(false), m_pWindow(NULL), m_bReadOnly(false)
{
	::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
	SetTextPadding(CRect(4, 3, 4, 3));
	SetBkColor(0xFFFFFFFF);
}

LPCTSTR CRichEditUI::GetClass() const
{
	return _T("RichEditUI");
}

LPVOID CRichEditUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("RichEdit")) == 0 ) return static_cast<CRichEditUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

bool CRichEditUI::IsLeft()const		//判断当前插入符是否最左
{
	POINT pt;
	::GetCaretPos(&pt);
	if (pt.x<=0) return true;		//判断插入符位置
	return false;
}

bool CRichEditUI::IsRight()const	//判断当前插入符是否最右(由于Caret只是在当前显示区坐标，故不可用，用字符数来判断)
{
	TCHAR str[1024];		//防止内存写异常
	int linecount,now;		//行数、字符数
	int pre=0;				//起始到前一行字符数
	now=SendMessage(*m_pWindow, EM_GETSEL,0,0);
	now=now/pow(2.0,16.0);							   //获取插入符前的字符个数
									
	linecount=SendMessage(*m_pWindow,EM_LINEFROMCHAR,now,0);//插入符所在行数

	for (int i=0;i<linecount;i++)					   //统计插入符前n-1行字符数
	{
	   pre+=Edit_GetLine(*m_pWindow,i,str,sizeof(TCHAR)*1024);
	   pre+=2;		//"\r\t"
	}
	linecount=Edit_GetLine(*m_pWindow,linecount,str,sizeof(TCHAR)*1024);//插入符所在行字符数
	if ((now-pre)>=linecount) return true;					//插入符溢出判断
	return false;
	
}

bool CRichEditUI::IsUp()const	//判断当前插入符是否最上
{

	int nPos;
	nPos=SendMessage(*m_pWindow, EM_GETSEL, 0, 0);  
	nPos=nPos/pow(2.0,16.0);
	nPos=SendMessage(*m_pWindow,EM_LINEFROMCHAR,nPos,0);//获取当前行数
	if (nPos<=0) return true;

	return false;
}

bool CRichEditUI::IsDown()const		//判断当前插入符是否最下
{
	int nPos;
	nPos=SendMessage(*m_pWindow, EM_GETSEL, 0, 0);
	nPos=nPos/pow(2.0,16.0);
	nPos=SendMessage(*m_pWindow,EM_LINEFROMCHAR,nPos,0);	//插入符所在行数
	if ((nPos+1)==SendMessage(*m_pWindow, EM_GETLINECOUNT, 0, 0 )) return true;//与总行数判断
	return false;
}


void CRichEditUI::SetTextStyle(UINT uStyle)
{
	m_uTextStyle = uStyle;
	Invalidate();
}

UINT CRichEditUI::GetTextStyle() const
{
	return m_uTextStyle;
}

void CRichEditUI::SetTextColor(DWORD dwTextColor)
{
	m_dwTextColor = dwTextColor;
}

DWORD CRichEditUI::GetTextColor() const
{
	return m_dwTextColor;
}

void CRichEditUI::SetDisabledTextColor(DWORD dwTextColor)
{
	m_dwDisabledTextColor = dwTextColor;
}

DWORD CRichEditUI::GetDisabledTextColor() const
{
	return m_dwDisabledTextColor;
}

void CRichEditUI::SetFont(int index)
{
	m_iFont = index;
}

int CRichEditUI::GetFont() const
{
	return m_iFont;
}

RECT CRichEditUI::GetTextPadding() const
{
	return m_rcTextPadding;
}

void CRichEditUI::SetTextPadding(RECT rc)
{
	m_rcTextPadding = rc;
	Invalidate();
}

bool CRichEditUI::IsShowHtml()
{
	return m_bShowHtml;
}

void CRichEditUI::SetShowHtml(bool bShowHtml)
{
	if( m_bShowHtml == bShowHtml ) return;

	m_bShowHtml = bShowHtml;
	Invalidate();
}


void CRichEditUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	RECT rcTemp = { 0 };
	if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

	CRenderClip clip;
	CRenderClip::GenerateClip(hDC, rcTemp, clip);
	CControlUI::DoPaint(hDC, rcPaint);

	if( m_items.GetSize() > 0 ) {
		RECT rc = m_rcItem;
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

		if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
			for( int it = 0; it < m_items.GetSize(); it++ ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
				if( !pControl->IsVisible() ) continue;
				if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
				if( pControl ->IsFloat() ) {
					if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
					pControl->DoPaint(hDC, rcPaint);
				}
			}
		}
		else {
			CRenderClip childClip;
			CRenderClip::GenerateClip(hDC, rcTemp, childClip);
			for( int it = 0; it < m_items.GetSize(); it++ ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
				if( !pControl->IsVisible() ) continue;
				if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
				if( pControl ->IsFloat() ) {
					if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
					CRenderClip::UseOldClipBegin(hDC, childClip);
					pControl->DoPaint(hDC, rcPaint);
					CRenderClip::UseOldClipEnd(hDC, childClip);
				}
				else {
					if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
					pControl->DoPaint(hDC, rcPaint);
				}
			}
		}
	}

	/*if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
		if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
			m_pVerticalScrollBar->DoPaint(hDC, rcPaint);
		}
	}

	if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
		if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
			m_pHorizontalScrollBar->DoPaint(hDC, rcPaint);
		}
	}*/
}



void CRichEditUI::DoEvent(TEventUI& event)
{

	
	
//处理滑轮
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		return;
	}
	if( event.Type == UIEVENT_WINDOWSIZE )
	{
		if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
	}
	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		if( m_pWindow != NULL ) return;
	}
	if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
	{
		
		if( m_pWindow )	return;		
		m_pWindow = new CRichEditWnd();
		ASSERT(m_pWindow);
		m_pWindow->Init(this);
		Invalidate();
	}
	if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
	{
//		Invalidate();
	}
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
	{
		if( IsEnabled() ) {
			GetManager()->ReleaseCapture();
			if( IsFocused() && m_pWindow == NULL )
			{
				m_pWindow = new CRichEditWnd();
				ASSERT(m_pWindow);
				m_pWindow->Init(this);

				if( PtInRect(&m_rcItem, event.ptMouse) )
				{
					int nSize = GetWindowTextLength(*m_pWindow);
					if( nSize == 0 )
						nSize = 1;

					Edit_SetSel(*m_pWindow, 0, nSize);
				}
			}
			else if( m_pWindow != NULL )
			{
#if 1
				int nSize = GetWindowTextLength(*m_pWindow);
				if( nSize == 0 )
					nSize = 1;

				Edit_SetSel(*m_pWindow, 0, nSize);
#else
				POINT pt = event.ptMouse;
				pt.x -= m_rcItem.left + m_rcTextPadding.left;
				pt.y -= m_rcItem.top + m_rcTextPadding.top;
				::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
			}
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEMOVE ) 
	{
		return;
	}
	if( event.Type == UIEVENT_BUTTONUP ) 
	{
		return;
	}
	if( event.Type == UIEVENT_CONTEXTMENU )
	{
		return;
	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
//			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
//			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	CContainerUI::DoEvent(event);
	
	
}



UINT CRichEditUI::GetControlFlags()const
{
	return 1;
}



void CRichEditUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	
}

void CRichEditUI::SetText(LPCTSTR pstrText)
{
	m_sText = pstrText;
	if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
	Invalidate();
}



void CRichEditUI::SetReadOnly(bool bReadOnly)
{
	if( m_bReadOnly == bReadOnly ) return;

	m_bReadOnly = bReadOnly;
	if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
	Invalidate();
}

bool CRichEditUI::IsReadOnly() const
{
	return m_bReadOnly;
}



LPCTSTR CRichEditUI::GetNormalImage()
{
	return m_sNormalImage;
}

void CRichEditUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CRichEditUI::GetHotImage()
{
	return m_sHotImage;
}

void CRichEditUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CRichEditUI::GetFocusedImage()
{
	return m_sFocusedImage;
}

void CRichEditUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CRichEditUI::GetDisabledImage()
{
	return m_sDisabledImage;
}

void CRichEditUI::SetDisabledImage(LPCTSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

void CRichEditUI::SetNativeEditBkColor(DWORD dwBkColor)
{
	m_dwEditbkColor = dwBkColor;
}

DWORD CRichEditUI::GetNativeEditBkColor() const
{
	return m_dwEditbkColor;
}

void CRichEditUI::SetPos(RECT rc)
{
	CContainerUI::SetPos(rc);
	if( m_pWindow != NULL ) {
		RECT rcPos = m_pWindow->CalPos();
		::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
			rcPos.bottom - rcPos.top,1);        
	}
}

void CRichEditUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
}

void CRichEditUI::PaintText(HDC hDC)
{
	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	if( m_sText.IsEmpty() ) return;

	CStdString sText = m_sText;

	RECT rc = m_rcItem;
	rc.left +=m_rcInset.left;
	rc.right -= m_rcInset.right;
	rc.top += m_rcInset.top;
	rc.bottom -= m_rcInset.bottom;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
	if( IsEnabled() ) {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
			m_iFont,m_uTextStyle|DT_TOP);
	}
	else {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
			m_iFont,m_uTextStyle);
	}
}

void CRichEditUI::SetInternVisible(bool bVisible)
{
	if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	
}


//SIZE CRichEditUI::EstimateSize(SIZE szAvailable)
//{
//	if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo()->tm.tmHeight + 6);
//	return CControlUI::EstimateSize(szAvailable);
//}

void CRichEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	switch(pstrName[0])
	{
	case 'h':
		if( _tcscmp(pstrName, _T("hotimage")) == 0 ) {SetHotImage(pstrValue);return ;}
		break;
	case 'f':
		if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) {SetFocusedImage(pstrValue);return ;}
		break;
	case 'd':
		if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) {SetDisabledImage(pstrValue);return ;}
		break;
	case 'n':
		if( _tcscmp(pstrName, _T("normalimage")) == 0 ) {SetNormalImage(pstrValue);return ;}
		else if( _tcscmp(pstrName, _T("nativebkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeEditBkColor(clrColor);
			return ;
		}
		break;
	case 'r':
		if( _tcscmp(pstrName, _T("readonly")) == 0 ) {SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);return ;}
		break;
	}
	CContainerUI::SetAttribute(pstrName, pstrValue);
}



} // namespace DuiLib
