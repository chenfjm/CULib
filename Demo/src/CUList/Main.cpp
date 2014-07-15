
#include <objbase.h>
//#include <zmouse.h>
//#include <exdisp.h>
#include <comdef.h>
#include <vector>
#include <sstream>

#include "CULib.h"

using namespace CULib;

#ifdef _DEBUG
#       pragma comment(lib, "CULib_d.lib")
#else
#       pragma comment(lib, "CULib.lib")
#endif

#define WM_ADDLISTITEM WM_USER + 50
/*
* 存放第二列数据
*/
std::vector<std::string> domain;
/*
* 存放第三列数据
*/
std::vector<std::string> desc;
/*
*  线程函数中传入的结构体变量，使用线程为了使界面线程立即返回，防止卡住，你们懂得。
*/
struct Prama
{
    HWND hWnd;
    CListUI* pList;
    CButtonUI* pSearch;
    CStdString tDomain;
};

/*
class CMenuwnd : public CWindowWnd, public INotifyUI
{
public:
    CMenuwnd() : m_pOwner(NULL) { };
    void Init(CControlUI* pOwner, POINT pt) {
        if( pOwner == NULL ) return;
        m_pOwner = pOwner;
        m_ptPos = pt;
        Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW);
        HWND hWndParent = m_hWnd;
        while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
        ::ShowWindow(m_hWnd, SW_SHOW);
        ::SendMessage(hWndParent, WM_ACTIVATE, TRUE, 0L);
    }

    void AdjustPostion() {
        CRect rcWnd;
        GetWindowRect(m_hWnd, &rcWnd);
        int nWidth = rcWnd.GetWidth();
        int nHeight = rcWnd.GetHeight();
        rcWnd.left = m_ptPos.x;
        rcWnd.top = m_ptPos.y;
        rcWnd.right = rcWnd.left + nWidth;
        rcWnd.bottom = rcWnd.top + nHeight;
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CRect rcWork = oMonitor.rcWork;

        if( rcWnd.bottom > rcWork.bottom ) {
            if( nHeight >= rcWork.GetHeight() ) {
                rcWnd.top = 0;
                rcWnd.bottom = nHeight;
            }
            else {
                rcWnd.bottom = rcWork.bottom;
                rcWnd.top = rcWnd.bottom - nHeight;
            }
        }
        if( rcWnd.right > rcWork.right ) {
            if( nWidth >= rcWork.GetWidth() ) {
                rcWnd.left = 0;
                rcWnd.right = nWidth;
            }
            else {
                rcWnd.right = rcWork.right;
                rcWnd.left = rcWnd.right - nWidth;
            }
        }
        ::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    LPCTSTR GetWindowClassName() const { return _T("MenuWnd"); };
    void OnFinalMessage(HWND /*hWnd) { delete this; };

    void Notify(TNotifyUI& msg)
    {
        if( msg.sType == _T("itemselect") ) {
            Close();
        }
        else if( msg.sType == _T("itemclick") ) {
            if( msg.pSender->GetName() == _T("menu_Delete") ) {
                if( m_pOwner ) m_pOwner->GetManager()->SendNotify(m_pOwner, _T("menu_Delete"), 0, 0, true);
            }
        }
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("menu.xml"), NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        m_pm.SetRoundCorner(3, 3);

        CListUI* pList = static_cast<CListUI*>(m_pOwner);
        int nSel = pList->GetCurSel();
        if( nSel < 0 ) {
            CControlUI* pControl = m_pm.FindControl(_T("menu_Delete"));
            if( pControl ) pControl->SetEnabled(false);
        }

        AdjustPostion();
        return 0;
    }

    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( m_hWnd != (HWND) wParam ) PostMessage(WM_CLOSE);
        return 0;
    }

    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( wParam == VK_ESCAPE ) Close();
        return 0;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch( uMsg ) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
        case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
        case WM_MOUSEWHEEL:    break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
        }
        if( bHandled ) return lRes;
        if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

public:
    CPaintManagerUI m_pm;
    CControlUI* m_pOwner;
    POINT m_ptPos;
};
*/
class ListMainForm : public CWindowWnd, public INotifyUI, public IListCallbackUI
{
public:
    ListMainForm() {
    };

    LPCTSTR GetWindowClassName() const 
    { 
        return _T("ScanMainForm"); 
    };

    UINT GetClassStyle() const
    { 
        return CS_DBLCLKS; 
    };

    void OnFinalMessage(HWND /*hWnd*/) 
    { 
        delete this;
    };

    void Init() 
    {
        m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
        m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
        m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
        m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
        m_pSearch = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn")));
    }

	bool OnDelete(void* param)
	{
	   TNotifyUI* pMsg = (TNotifyUI*)param;
	   if( pMsg->sType == _T("itemactivate") ) 
		  if( m_pMenu->m_pControl ) 
		  {
			  int nSel = m_pList->GetCurSel();
			  if( nSel < 0 ) 
				  return false;
			  m_pList->RemoveAt(nSel);
			  domain.erase(domain.begin() + nSel);
			  desc.erase(desc.begin() + nSel);   
		  }
		return true;
	}
    void OnPrepare(TNotifyUI& msg) 
    {

    }
    static DWORD WINAPI Search(LPVOID lpParameter)
    {
        try
        {
            struct Prama* prama = (struct Prama *)lpParameter;
            CListUI* pList = prama->pList;
            CButtonUI* pSearch = prama->pSearch;
            CStdString tDomain = prama->tDomain;
            //-------------------------------------
            /*
            * 添加数据循环
            */
            for(int i=0; i<100; i++)
            {
                std::stringstream ss;
                ss << "www." << i << ".com";
                domain.push_back(ss.str());
                ss.clear();
                ss << "it's " << i;
                desc.push_back(ss.str());
                CListTextElementUI* pListElement = new CListTextElementUI;
                pListElement->SetTag(i);
                if (pListElement != NULL)
                {
                    ::PostMessage(prama->hWnd, WM_ADDLISTITEM, 0L, (LPARAM)pListElement);
                }
                /*
                *	Sleep 为了展示添加的动态效果，故放慢了添加速度，同时可以看到添加过程中界面仍然可以响应
                */
                ::Sleep(100);
            }
            //------------------------------------------
            delete prama;

            pSearch->SetEnabled(true);
            return 0;
        }
        catch(...)
        {
            return 0;
        }
    }
    void OnSearch()
    {
        struct Prama *prama = new Prama;

        m_pList = static_cast<CListUI*>(m_pm.FindControl(_T("domainlist")));
        CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("input")));
        pEdit->SetEnabled(false);
        CStdString input = pEdit->GetText();
        m_pSearch->SetEnabled(false);
        m_pList->RemoveAll();
        domain.empty();
        domain.resize(0);
        desc.empty();
        desc.resize(0);
        DWORD dwThreadID = 0;
        m_pList->SetTextCallback(this);//[1]

        prama->hWnd = GetHWND();
        prama->pList = m_pList;
        prama->pSearch = m_pSearch;
        prama->tDomain = input;

        HANDLE hThread = CreateThread(NULL,0,&ListMainForm::Search, (LPVOID)prama,  0,&dwThreadID);
    }
    /*
    * 关键的回调函数，IListCallbackUI 中的一个虚函数，渲染时候会调用,在[1]中设置了回调对象
    */
    LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
    {
        TCHAR szBuf[MAX_PATH] = {0};
        switch (iSubItem)
        {
        case 0:
            _stprintf(szBuf, _T("%d"), iIndex);
            break;
        case 1:
            {
#ifdef _UNICODE		
            int iLen = domain[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
            _stprintf(szBuf, lpText);
            delete[] lpText;
#else
            _stprintf(szBuf, domain[iIndex].c_str());
#endif
            }
            break;
        case 2:
            {
#ifdef _UNICODE		
            int iLen = desc[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, desc[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
            _stprintf(szBuf, lpText);
            delete[] lpText;
#else
            _stprintf(szBuf, desc[iIndex].c_str());
#endif
            }
            break;
        }
        pControl->SetUserData(szBuf);
        return pControl->GetUserData();
    }

    void Notify(TNotifyUI& msg)
    {
        if( msg.sType == _T("windowinit") ) 
            OnPrepare(msg);
        else if( msg.sType == _T("click") ) 
        {
            if( msg.pSender == m_pCloseBtn ) 
            {
                PostQuitMessage(0);
                return; 
            }
            else if( msg.pSender == m_pMinBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND, 0, 0);
                return; 
            }
            else if( msg.pSender == m_pMaxBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND,0, 0); return; 
            }
            else if( msg.pSender == m_pRestoreBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND, 0, 0); return; 
            }
            else if(msg.pSender == m_pSearch)
            {
                OnSearch();
            }
        }
        else if(msg.sType==_T("setfocus"))
        {
        }
        else if( msg.sType == _T("itemclick") ) 
        {
        }
        else if( msg.sType == _T("itemactivate") ) 
        {
            int iIndex = msg.pSender->GetTag();
            CStdString sMessage = _T("Click: ");;
#ifdef _UNICODE		
            int iLen = domain[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
            sMessage += lpText;
            delete[] lpText;
#else
            sMessage += domain[iIndex].c_str();

#endif
            ::MessageBox(NULL, sMessage.GetData(), _T("提示(by tojen)"), MB_OK);
        }
        else if(msg.sType == _T("menu")) 
        {
            if( msg.pSender->GetName() != _T("domainlist") ) return;
            m_pMenu = new CMenuWnd(m_hWnd);
            if( m_pMenu == NULL ) { return; }
            POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
            ::ClientToScreen(*this, &pt);
			STRINGorID xml(_T("menu.xml"));
            m_pMenu->Init(msg.pSender,xml,_T("xml") ,pt);
			CMenuElementUI* pElement=(CMenuElementUI*)m_pMenu->m_pm.FindControl(_T("menu_Delete"));
			if(pElement)
				pElement->OnNotify += MakeDelegate(this,&ListMainForm::OnDelete);

        }
        else if( msg.sType == _T("menu_Delete") ) {
            CListUI* pList = static_cast<CListUI*>(msg.pSender);
            int nSel = pList->GetCurSel();
            if( nSel < 0 ) return;
            pList->RemoveAt(nSel);
            domain.erase(domain.begin() + nSel);
            desc.erase(desc.begin() + nSel);   
        }
    }

    LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
        CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("domainlist")));
        if( pList ) pList->Add(pListElement);
        return 0;
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS );
        m_pm.Init(m_hWnd);
        //m_pm.SetTransparent(100);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("skin.xml"),  NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        Init();
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        ::PostQuitMessage(0L);
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
//        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

 /*     LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
      POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        if( !::IsZoomed(*this) ) {
            RECT rcSizeBox = m_pm.GetSizeBox();
            if( pt.y < rcClient.top + rcSizeBox.top ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
                return HTTOP;
            }
            else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
        }

        RECT rcCaption = m_pm.GetCaptionRect();
        if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
                if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                    _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                    _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
                    return HTCAPTION;
        }

        return HTCLIENT;
    }
*/
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
/*  
		SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
            HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
            HRGN hRgn2 = ::CreateRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx);
            ::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
            ::SetWindowRgn(*this, hRgn1, TRUE);
            ::DeleteObject(hRgn1);
            ::DeleteObject(hRgn2);
       }
*/
        bHandled = FALSE;
        return 0;
    }

 /*   LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CRect rcWork = oMonitor.rcWork;
        rcWork.Offset(-rcWork.left, -rcWork.top);

        LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
        lpMMI->ptMaxPosition.x	= rcWork.left;
        lpMMI->ptMaxPosition.y	= rcWork.top;
        lpMMI->ptMaxSize.x		= rcWork.right;
        lpMMI->ptMaxSize.y		= rcWork.bottom;

        bHandled = FALSE;
        return 0;
    }
*/
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if( wParam == SC_CLOSE ) {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
 //    BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
  /*         if( ::IsZoomed(*this) != bZoomed ) {
            if( !bZoomed ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if( pControl ) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if( pControl ) pControl->SetVisible(true);
            }
            else {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if( pControl ) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if( pControl ) pControl->SetVisible(false);
            }
        }*/
        return lRes;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch( uMsg ) {
            case WM_ADDLISTITEM:   lRes = OnAddListItem(uMsg, wParam, lParam, bHandled); break;
            case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
            case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
            case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
            case WM_ACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
//            case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
//            case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
//            case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
            case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
//            case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
            case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
            default:
                bHandled = FALSE;
        }
        if( bHandled ) return lRes;
        if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }
public:
    CPaintManagerUI m_pm;

private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
    CButtonUI* m_pSearch;
	CMenuWnd* m_pMenu;
	CListUI* m_pList;

    //...
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
    CPaintManagerUI::SetResourceZip(_T("ListRes.zip"));

	HRESULT Hr = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
	if( FAILED(Hr) ) return 0;

    ListMainForm* pFrame = new ListMainForm();
    if( pFrame == NULL ) return 0;
    pFrame->Create(NULL, _T("ListDemo"), 0, 0L , 0, 0, 600, 320);
    pFrame->CenterWindow();
    ::ShowWindow(*pFrame, SW_SHOW);

    CPaintManagerUI::MessageLoop();

	return 0;
}