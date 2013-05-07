#include "CULib.h"
#pragma warning(disable : 4541)
namespace CULib {

/////////////////////////////////////////////////////////////////////////////////////
//
ContextMenuObserver s_context_menu_observer;

// MenuUI
const TCHAR* const kMenuUIClassName = _T("MenuUI");
const TCHAR* const kMenuUIInterfaceName = _T("Menu");

CMenuUI::CMenuUI()
{
	if (GetHeader() != NULL)
		GetHeader()->SetVisible(false);
}

CMenuUI::~CMenuUI()
{}

LPCTSTR CMenuUI::GetClass() const
{
    return kMenuUIClassName;
}

LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, kMenuUIInterfaceName) == 0 ) return static_cast<CMenuUI*>(this);
    return CListUI::GetInterface(pstrName);
}

/**
 * @fn          DoEvent
 * @brief       事件处理函数
 * @param       TEventUI & event
 * @retval    	void
 *              调用父类的事件处理函数
 */
void CMenuUI::DoEvent(TEventUI& event)
{
	return __super::DoEvent(event);
}

/**
 * @fn          Add
 * @brief       向菜单中加入菜单项
 * @param       CControlUI * pControl   要加入的菜单项
 * @param       CControlUI * pParent    一般为NULL
 * @return      成功返回true，失败返回false
 * @retval    	bool
 */
bool CMenuUI::Add(CControlUI* pControl,CControlUI* pParent)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	for (int i = 0; i < pMenuItem->GetCount(); ++i){
		if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL){
			(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
		}
	}
	return CListUI::Add(pControl);
}

/**
 * @fn          AddAt
 * @brief       在指定位置向菜单中加入菜单项
 * @param       CControlUI * pControl    要加入的菜单项
 * @param       int iIndex   加入位置
 * @return      成功返回true，失败返回false
 * @retval    	bool
 */
bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	for (int i = 0; i < pMenuItem->GetCount(); ++i){
		if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL){
			(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
		}
	}
	return CListUI::AddAt(pControl, iIndex);
}

int CMenuUI::GetItemIndex(CControlUI* pControl) const
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return -1;

	return __super::GetItemIndex(pControl);
}

bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	return __super::SetItemIndex(pControl, iIndex);
}

/**
 * @fn          Remove
 * @brief       移除指定菜单项
 * @param       CControlUI * pControl    被移除的菜单项
 * @return      成功返回true，失败返回false
 * @retval    	bool
 */
bool CMenuUI::Remove(CControlUI* pControl)
{
	CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
	if (pMenuItem == NULL)
		return false;

	return __super::Remove(pControl);
}

/**
 * @fn          EstimateSize
 * @brief       计算菜单窗口的大小
 * @param       SIZE szAvailable
 * @return      返回窗口大小
 * @retval    	SIZE
 */
SIZE CMenuUI::EstimateSize(SIZE szAvailable)
{
	int cxFixed = 0;
    int cyFixed = 0;
    for( int it = 0; it < GetCount(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
        if( !pControl->IsVisible() ) continue;
        SIZE sz = pControl->EstimateSize(szAvailable);
        cyFixed += sz.cy;
		if( cxFixed < sz.cx )
			cxFixed = sz.cx;
    }
    return CSize(cxFixed, cyFixed);
}

/**
 * @fn          SetAttribute
 * @brief       设置菜单属性
 * @param       LPCTSTR pstrName    属性名称
 * @param       LPCTSTR pstrValue   属性值
 * @retval    	void
 */
void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CListUI::SetAttribute(pstrName, pstrValue);
}

/////////////////////////////////////////////////////////////////////////////////////
//

/** 生成菜单控件和菜单项控件的函数 */
class CMenuBuilderCallback: public IDialogBuilderCallback
{
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, kMenuUIInterfaceName) == 0){
			return new CMenuUI();
		}
		else if (_tcsicmp(pstrClass, kMenuElementUIInterfaceName) == 0){
			return new CMenuElementUI();
		}
		return NULL;
	}
};

CMenuWnd::CMenuWnd(HWND hParent):
m_hParent(hParent),
m_pOwner(NULL),
m_pLayout(),
m_xml(_T(""))
{}

/**
 * @fn          Receive
 * @brief       菜单窗口作为观察者对通知的处理
 * @param       ContextMenuParam param
 * @retval    	BOOL
 * 参数为1，关闭所有菜单，参数为2，关闭当前菜单项所在菜单的子菜单
 */
BOOL CMenuWnd::Receive(ContextMenuParam param)
{
	switch (param.wParam){
	case 1:
		Close();
		break;
	case 2:{
			HWND hParent = GetParent(m_hWnd);
			while (hParent != NULL){
				if (hParent == param.hWnd){
					Close();
					break;
				}
				hParent = GetParent(hParent);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

/**
 * @fn          Init
 * @brief       菜单窗口是初始化
 * @param       CControlUI * pOwner   菜单窗口的拥有者
 * @param       STRINGorID xml        菜单描述文件
 * @param       LPCTSTR pSkinType     资源类型
 * @param       POINT point           激活菜单时鼠标坐标
 * @retval    	void
 *              在该函数中完成菜单窗口的创建，并把菜单窗口加入菜单观察对象中，
 *              此观察对象全局唯一，负责通知各个菜单窗口关闭自身
 */
void CMenuWnd::Init(CControlUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point)
{
	m_BasedPoint = point;
    m_pControl = pOwner;
	m_pOwner = dynamic_cast<CMenuElementUI*>(m_pControl);
    m_pLayout = NULL;

	if (pSkinType != NULL)
		m_sType = pSkinType;

	m_xml = xml;

	s_context_menu_observer.AddReceiver(this);

	Create((m_pOwner == NULL) ? m_hParent : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP , WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CRect());
    // HACK: Don't deselect the parent's caption
    HWND hWndParent = m_hWnd;
    while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
    ::ShowWindow(m_hWnd, SW_SHOW);
#if defined(WIN32) && !defined(UNDER_CE)
    ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
#endif	
}

LPCTSTR CMenuWnd::GetWindowClassName() const
{
    return _T("MenuWnd");
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	if( m_pOwner != NULL ) {
		for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
			if( static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)) != NULL ) {
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pOwner->GetParent());
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
			}
		}
		m_pOwner->m_pWindow = NULL;
		m_pOwner->m_uButtonState &= ~ UISTATE_PUSHED;
		m_pOwner->Invalidate();
	}
    delete this;
}


/**
 * @fn          OnCreate
 * @brief       生成菜单创库的消息处理函数
 * @retval    	LRESULT
 *              该函数实现了从xml配置文件读取数据并创建菜单界面，若m_pOwner不为空
 *              则所创建的菜单为子菜单，负责为主菜单。
 */
LRESULT CMenuWnd::OnCreate( UINT uMag, WPARAM wParam, LPARAM lParam,BOOL& bHandled )
{
	if( m_pOwner != NULL) {
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

		m_pm.Init(m_hWnd);
		// The trick is to add the items to the new container. Their owner gets
		// reassigned by this operation - which is why it is important to reassign
		// the items back to the righfull owner/manager when the window closes.
		m_pLayout = new CMenuUI();
		m_pm.UseParentResource(m_pOwner->GetManager());
		m_pLayout->SetManager(&m_pm, NULL, true);
		LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(kMenuUIInterfaceName);
		if( pDefaultAttributes ) {
			m_pLayout->ApplyAttributeList(pDefaultAttributes);
		}
		m_pLayout->SetBkColor(0xFFFFFFFF);
		m_pLayout->SetBorderColor(0xFF85E4FF);
		m_pLayout->SetBorderSize(0);
		m_pLayout->SetAutoDestroy(false);
		m_pLayout->EnableScrollBar();
		for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
			if(m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL ){
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pLayout);
				m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
			}
		}
		m_pm.AttachDialog(m_pLayout);

		// Position the popup window in absolute space
		RECT rcOwner = m_pOwner->GetPos();
		RECT rc = rcOwner;

		int cxFixed = 0;
		int cyFixed = 0;

#if defined(WIN32) && !defined(UNDER_CE)
		MONITORINFO oMonitor = {}; 
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CRect rcWork = oMonitor.rcWork;
#else
		CRect rcWork;
		GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
		SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };

		for( int it = 0; it < m_pOwner->GetCount(); it++ ) {
			if(m_pOwner->GetItemAt(it)->GetInterface(kMenuElementUIInterfaceName) != NULL ){
				CControlUI* pControl = static_cast<CControlUI*>(m_pOwner->GetItemAt(it));
				SIZE sz = pControl->EstimateSize(szAvailable);
				cyFixed += sz.cy;

				if( cxFixed < sz.cx )
					cxFixed = sz.cx;
			}
		}
		cyFixed += 4;
		cxFixed += 4;

		RECT rcWindow;
		GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWindow);

		rc.top = rcOwner.top;
		rc.bottom = rc.top + cyFixed;
		::MapWindowRect(m_pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
		rc.left = rcWindow.right;
		rc.right = rc.left + cxFixed;
		rc.right += 2;

		bool bReachBottom = false;
		bool bReachRight = false;
		LONG chRightAlgin = 0;
		LONG chBottomAlgin = 0;

		RECT rcPreWindow = {0};
		ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(s_context_menu_observer);
		ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
		while( pReceiver != NULL ) {
			CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
			if( pContextMenu != NULL ) {
				GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

				bReachRight = rcPreWindow.left >= rcWindow.right;
				bReachBottom = rcPreWindow.top >= rcWindow.bottom;
				if( pContextMenu->GetHWND() == m_pOwner->GetManager()->GetPaintWindow() 
					||  bReachBottom || bReachRight )
					break;
			}
			pReceiver = iterator.next();
		}

		if (bReachBottom){
			rc.bottom = rcWindow.top;
			rc.top = rc.bottom - cyFixed;
		}

		if (bReachRight){
			rc.right = rcWindow.left;
			rc.left = rc.right - cxFixed;
		}

		if( rc.bottom > rcWork.bottom ){
			rc.bottom = rc.top;
			rc.top = rc.bottom - cyFixed;
		}

		if (rc.right > rcWork.right){
			rc.right = rcWindow.left;
			rc.left = rc.right - cxFixed;

			rc.top = rcWindow.bottom;
			rc.bottom = rc.top + cyFixed;
		}

		if( rc.top < rcWork.top ){
			rc.top = rcOwner.top;
			rc.bottom = rc.top + cyFixed;
		}

		if (rc.left < rcWork.left){
			rc.left = rcWindow.right;
			rc.right = rc.left + cxFixed;
		}

		MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
	}
	else {
		m_pm.Init(m_hWnd);

		CDialogBuilder builder;
		CMenuBuilderCallback menuCallback;

		CControlUI* pRoot = builder.Create(m_xml, &menuCallback, &m_pm , NULL , m_sType.GetData());
		m_pm.AttachDialog(pRoot);

#if defined(WIN32) && !defined(UNDER_CE)
		MONITORINFO oMonitor = {}; 
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CRect rcWork = oMonitor.rcWork;
#else
		CRect rcWork;
		/** 原来有bug m_pOwner为NULL，不能用m_pOwner->GetManager()，应为m_pm */
		//GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
		GetWindowRect(m_pm.GetPaintWindow(), &rcWork);
#endif
		SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
		szAvailable = pRoot->EstimateSize(szAvailable);
		m_pm.SetInitSize(szAvailable.cx, szAvailable.cy);

		DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top;

		SIZE szInit = m_pm.GetInitSize();
		CRect rc;
		CPoint point = m_BasedPoint;
		rc.left = point.x;
		rc.top = point.y;
		rc.right = rc.left + szInit.cx;
		rc.bottom = rc.top + szInit.cy;

		int nWidth = rc.GetWidth();
		int nHeight = rc.GetHeight();

		if (dwAlignment & eMenuAlignment_Right){
			rc.right = point.x;
			rc.left = rc.right - nWidth;
		}

		if (dwAlignment & eMenuAlignment_Bottom){
			rc.bottom = point.y;
			rc.top = rc.bottom - nHeight;
		}

		SetForegroundWindow(m_hWnd);
		MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
		SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), SWP_SHOWWINDOW);
		m_pLayout=static_cast<CMenuUI*>(m_pm.GetRoot());
	}
		m_pm.SetFocus(m_pLayout->GetItemAt(0));
	return 0;
}

LRESULT CMenuWnd::OnKillFocus( UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled )
{
	HWND hFocusWnd = (HWND)wParam;

	BOOL bInMenuWindowList = FALSE;
	ContextMenuParam param;
	param.hWnd = GetHWND();

	ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(s_context_menu_observer);
	ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
	while( pReceiver != NULL ) {
		CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
		if( pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd ) {
			bInMenuWindowList = TRUE;
			break;
		}
		pReceiver = iterator.next();
	}

	if( !bInMenuWindowList ) {
		param.wParam = 1;
		s_context_menu_observer.RBroadcast(param);
	}
	bHandled=false;
	return 0;
}

LRESULT CMenuWnd::OnKeyDown( UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled )
{
	if( wParam == VK_ESCAPE){
		Close();
	}
	bHandled=false;
	return 0;
}

LRESULT CMenuWnd::OnClose( UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled )
{
	if( m_pOwner != NULL ){
		m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
		m_pOwner->SetPos(m_pOwner->GetPos());
		m_pOwner->SetFocus();
	}
	bHandled=false;
	return 0;
}

/** 菜单窗口的消息预处理 */
LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////////
//

// MenuElementUI
const TCHAR* const kMenuElementUIClassName = _T("MenuElementUI");
const TCHAR* const kMenuElementUIInterfaceName = _T("MenuElement");

CMenuElementUI::CMenuElementUI():
m_pWindow(NULL)
{
	m_cxyFixed.cy = 25;
	m_bMouseChildEnabled = true;

	SetMouseChildEnabled(false);
}

CMenuElementUI::~CMenuElementUI()
{}

LPCTSTR CMenuElementUI::GetClass() const
{
	return kMenuElementUIClassName;
}

LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, kMenuElementUIInterfaceName) == 0 ) return static_cast<CMenuElementUI*>(this);    
    return CListContainerElementUI::GetInterface(pstrName);
}

/** 菜单项的绘制函数 */
void CMenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
	CMenuElementUI::DrawItemBk(hDC, m_rcItem);
	DrawItemText(hDC, m_rcItem);
	for (int i = 0; i < GetCount(); ++i){
		if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) == NULL)
			GetItemAt(i)->DoPaint(hDC, rcPaint);
	}
}

void CMenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
    if( m_sText.IsEmpty() ) return;

    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iTextColor = pInfo->dwHotTextColor;
    }
    if( IsSelected() ) {
        iTextColor = pInfo->dwSelectedTextColor;
    }
    if( !IsEnabled() ) {
        iTextColor = pInfo->dwDisabledTextColor;
    }
    int nLinks = 0;
    RECT rcText = rcItem;
    rcText.left += pInfo->rcTextPadding.left;
    rcText.right -= pInfo->rcTextPadding.right;
    rcText.top += pInfo->rcTextPadding.top;
    rcText.bottom -= pInfo->rcTextPadding.bottom;

    if( pInfo->bShowHtml )
        CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        NULL, NULL, nLinks, DT_SINGLELINE | pInfo->uTextStyle);
    else
        CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, iTextColor, \
        pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
}

/** 计算菜单项大小 */
SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
{
	SIZE cXY = {0};
	for( int it = 0; it < GetCount(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
		if( !pControl->IsVisible() ) continue;
		SIZE sz = pControl->EstimateSize(szAvailable);
		cXY.cy += sz.cy;
		if( cXY.cx < sz.cx )
			cXY.cx = sz.cx;
	}
	if(cXY.cy == 0) {
		TListInfoUI* pInfo = m_pOwner->GetListInfo();

		DWORD iTextColor = pInfo->dwTextColor;
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			iTextColor = pInfo->dwHotTextColor;
		}
		if( IsSelected() ) {
			iTextColor = pInfo->dwSelectedTextColor;
		}
		if( !IsEnabled() ) {
			iTextColor = pInfo->dwDisabledTextColor;
		}

		RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
		rcText.left += pInfo->rcTextPadding.left;
		rcText.right -= pInfo->rcTextPadding.right;
		if( pInfo->bShowHtml ) {   
			int nLinks = 0;
			CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, NULL, NULL, nLinks, DT_CALCRECT | pInfo->uTextStyle);
		}
		else {
			CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
		}
		cXY.cx = rcText.right - rcText.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right + 20;
		cXY.cy = rcText.bottom - rcText.top + pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
	}

	if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
	return cXY;
}

/**
 * @fn          DoEvent
 * @brief       菜单项时间处理函数
 * @param       TEventUI & event
 * @retval    	void
 *              该函数只处理鼠标进入和鼠标按下消息，鼠标进入时显示菜单项对应的子菜单。
 *              并关闭菜单项所在菜单窗口的其他子菜单。鼠标按下时，菜单项属于激活状态，
 *              发送itemactivate通知
 */
void CMenuElementUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_MOUSEENTER ){
		CListContainerElementUI::DoEvent(event);
		if( m_pWindow ) return;
		bool hasSubMenu = false;
		for( int i = 0; i < GetCount(); ++i ){
			if( GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL ){
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);

				hasSubMenu = true;
			}
		}
		if( hasSubMenu ){			
			m_pOwner->SelectItem(GetIndex(), true);
			CreateMenuWnd();
		}
		else{
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 2;
			s_context_menu_observer.RBroadcast(param);
			m_pOwner->SelectItem(GetIndex(), true);
		}
	}

	if( event.Type == UIEVENT_BUTTONDOWN ){
		if( IsEnabled() ){
			CListContainerElementUI::DoEvent(event);

			if( m_pWindow ) return;

			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i ) {
				if( GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL ) {
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);

					hasSubMenu = true;
				}
			}
			if( hasSubMenu ){
				CreateMenuWnd();
			}
			else{
				Activate();
			}
        }
        return;
    }

    CListContainerElementUI::DoEvent(event);
}

/**
 * @fn          Activate
 * @brief       菜单项激活时执行的函数
 * @retval    	bool
 *              调用父类的激活函数，并向观察对象发送通知
 */
bool CMenuElementUI::Activate()
{
	CListContainerElementUI::Activate();
	ContextMenuParam param;
	param.hWnd = m_pManager->GetPaintWindow();
	param.wParam = 1;
	s_context_menu_observer.RBroadcast(param);
	return true;
}

CMenuWnd* CMenuElementUI::GetMenuWnd()
{
	return m_pWindow;
}

/**
 * @fn          CreateMenuWnd
 * @brief       生成菜单窗口
 * @retval    	void
 *              该菜单项即为生成的菜单窗口的拥有者
 */
void CMenuElementUI::CreateMenuWnd()
{
	if( m_pWindow ) return;

	m_pWindow = new CMenuWnd(m_pManager->GetPaintWindow());
	ASSERT(m_pWindow);

	ContextMenuParam param;
	param.hWnd = m_pManager->GetPaintWindow();
	param.wParam = 2;
	s_context_menu_observer.RBroadcast(param);

	m_pWindow->Init(static_cast<CControlUI*>(this), _T(""), _T(""), CPoint());
}

}
