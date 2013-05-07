#ifndef __CUACTIVEX_H__
#define __CUACTIVEX_H__

#pragma once

struct IOleObject;

namespace CULib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CActiveXCtrl;

/** 安全释放对象t */
template< class T >
class CSafeRelease
{
public:
	CSafeRelease(T* p) : m_p(p) { };
	~CSafeRelease() { if( m_p != NULL ) m_p->Release(); };
	T* Detach() { T* t = m_p; m_p = NULL; return t; };
	T* m_p;
};

/////////////////////////////////////////////////////////////////////////////////////
//

/** 为主要类包装了CActiveXCtrl和CActiveXWnd作为控件供用户使用 */
class CULIB_API CActiveXUI : public CControlUI, public IMessageFilterUI
{
	friend CActiveXCtrl;
public:
	CActiveXUI();
	virtual ~CActiveXUI();

	/** 得到控件类名 */
	LPCTSTR GetClass() const;
	/** 得到控件指针 */
	LPVOID GetInterface(LPCTSTR pstrName);

	/** 得到宿主窗口句柄 */
	HWND GetHostWindow() const;

	/** 获取 OR 设置延时创建activeX控件 */
	bool IsDelayCreate() const;
	void SetDelayCreate(bool bDelayCreate = true);

	/** 按clsid创建控件 */
	bool CreateControl(const CLSID clsid);
	/** 按activeX名创建控件 */
	bool CreateControl(LPCTSTR pstrCLSID);
	/** 获取控件接口 */
	HRESULT GetControl(const IID iid, LPVOID* ppRet);
	/** 得到ActiveX控件id */
	CLSID GetClisd() const;
	/** 得到控件名称 */
	CStdString GetModuleName() const;
	/** 设置控件名称 */
	void SetModuleName(LPCTSTR pstrText);

	/** 设置是否可见 */
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	/** 设置控件位置 */
	void SetPos(RECT rc);
	/** 调用CActiveXCtrl::IViewObject::Draw() 实现绘制 */
	void DoPaint(HDC hDC, const RECT& rcPaint);

	/** 设置控件属性 */
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	/** 消息处理 */
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

protected:
	/** 释放控件 */
	void ReleaseControl();
	/** 创建控件 */
	bool DoCreateControl();

protected:
	CLSID m_clsid;
	CStdString m_sModuleName;
	bool m_bCreated;
	bool m_bDelayCreate;
	IOleObject* m_pUnk;
	CActiveXCtrl* m_pControl;
	HWND m_hwndHost;
};

} // namespace DuiLib

#endif