/**  
*  @file   CUDlgBuilder.cpp
*  @brief  此文件用来实现菜单控件 
*  
*		   文件的前半部分为观察者模式的实现，后半部分共定义三个类，分别为
*	       菜单控件抽象CMenuUI，菜单窗口CMenuWnd，菜单选项CMenuElementUI。
* @author  chencheng
* @data    2013-04-13
*/
#ifndef __UIMENU_H__
#define __UIMENU_H__

#ifdef _MSC_VER
#pragma once
#endif
#include "CULib.h"
#include <map>
#include <vector>

namespace CULib {

template <typename ReturnT, typename ParamT>
class ReceiverImplBase;

/** 观察对象基类 */
template <typename ReturnT, typename ParamT>
class ObserverImplBase
{
public:
	virtual void AddReceiver(ReceiverImplBase<ReturnT, ParamT>* receiver) = 0;
	virtual void RemoveReceiver(ReceiverImplBase<ReturnT, ParamT>* receiver) = 0;
	virtual ReturnT Broadcast(ParamT param) = 0;
	virtual ReturnT RBroadcast(ParamT param) = 0;
	virtual ReturnT Notify(ParamT param) = 0;
};

/** 观察者基类 */
template <typename ReturnT, typename ParamT>
class ReceiverImplBase
{
public:
	virtual void AddObserver(ObserverImplBase<ReturnT, ParamT>* observer) = 0;
	virtual void RemoveObserver() = 0;
	virtual ReturnT Receive(ParamT param) = 0;
	virtual ReturnT Respond(ParamT param, ObserverImplBase<ReturnT, ParamT>* observer) = 0;
};

template <typename ReturnT, typename ParamT>
class ReceiverImpl;

/** 观察对象 */
template <typename ReturnT, typename ParamT>
class ObserverImpl : public ObserverImplBase<ReturnT, ParamT>
{
	template <typename ReturnT, typename ParamT>
	friend class Iterator;
public:
	ObserverImpl()
	{}

	virtual ~ObserverImpl()	{}

	/** 增加观察者 */
	virtual void AddReceiver(ReceiverImplBase<ReturnT, ParamT>* receiver)
	{
		if (receiver == NULL)
			return;

		receivers_.push_back(receiver);
		receiver->AddObserver(this);
	}

	/** 移除所有观察者 */
	virtual void RemoveReceiver(ReceiverImplBase<ReturnT, ParamT>* receiver)
	{
		if (receiver == NULL)
			return;

		ReceiversVector::iterator it = receivers_.begin();
		for (; it != receivers_.end(); ++it)
		{
			if (*it == receiver)
			{
				receivers_.erase(it);
				break;
			}
		}
	}

	/** 向观察者发送通知 */
	virtual ReturnT Broadcast(ParamT param)
	{
		ReceiversVector::iterator it = receivers_.begin();
		for (; it != receivers_.end(); ++it)
		{
			(*it)->Receive(param);
		}

		return ReturnT();
	}

	/** 按逆向顺序向观察者发送通知 */
	virtual ReturnT RBroadcast(ParamT param)
	{
		ReceiversVector::reverse_iterator it = receivers_.rbegin();
		for (; it != receivers_.rend(); ++it)
		{
			(*it)->Receive(param);
		}

		return ReturnT();
	}

	/** 向观察者发送通知，并把自身作为参数传递 */
	virtual ReturnT Notify(ParamT param)
	{
		ReceiversVector::iterator it = receivers_.begin();
		for (; it != receivers_.end(); ++it)
		{
			(*it)->Respond(param, this);
		}

		return ReturnT();
	}

	/** 观察对象的迭代器 */
	template <typename ReturnT, typename ParamT>
	class Iterator
	{
		ObserverImpl<ReturnT, ParamT> & _tbl;
		int index;
		ReceiverImplBase<ReturnT, ParamT>* ptr;
	public:
		Iterator( ObserverImpl & table )
			: _tbl( table ), index(0), ptr(NULL)
		{}

		Iterator( const Iterator & v )
			: _tbl( v._tbl ), index(v.index), ptr(v.ptr)
		{}

		ReceiverImplBase<ReturnT, ParamT>* next()
		{
			if ( index >= _tbl.receivers_.size() )
				return NULL;

			for ( ; index < _tbl.receivers_.size(); )
			{
				ptr = _tbl.receivers_[ index++ ];
				if ( ptr )
					return ptr;
			}
			return NULL;
		}
	};

protected:
	typedef std::vector<ReceiverImplBase<ReturnT, ParamT>*> ReceiversVector;
	ReceiversVector receivers_;
};

/** 观察者 */
template <typename ReturnT, typename ParamT>
class ReceiverImpl : public ReceiverImplBase<ReturnT, ParamT>
{
public:
	ReceiverImpl()
	{}

	virtual ~ReceiverImpl()	{}

	/** 增加观察对象 */
	virtual void AddObserver(ObserverImplBase<ReturnT, ParamT>* observer)
	{
		observers_.push_back(observer);
	}

	/** 移除观察对象 */
	virtual void RemoveObserver()
	{
		ObserversVector::iterator it = observers_.begin();
		for (; it != observers_.end(); ++it)
		{
			(*it)->RemoveReceiver(this);
		}
	}

	/** 响应通知 */
	virtual ReturnT Receive(ParamT param)
	{
		return ReturnT();
	}

	virtual ReturnT Respond(ParamT param, ObserverImplBase<ReturnT, ParamT>* observer)
	{
		return ReturnT();
	}

protected:
	typedef std::vector<ObserverImplBase<ReturnT, ParamT>*> ObserversVector;
	/** 观察对象数组 */
	ObserversVector observers_;
};


/////////////////////////////////////////////////////////////////////////////////////
//
struct ContextMenuParam
{
	// 1: remove all
	// 2: remove the sub menu
	WPARAM wParam;
	HWND hWnd;
};

enum MenuAlignment
{
	eMenuAlignment_Left = 1 << 1,
	eMenuAlignment_Top = 1 << 2,
	eMenuAlignment_Right = 1 << 3,
	eMenuAlignment_Bottom = 1 << 4,
};

typedef class ObserverImpl<BOOL, ContextMenuParam> ContextMenuObserver;
typedef class ReceiverImpl<BOOL, ContextMenuParam> ContextMenuReceiver;


extern ContextMenuObserver s_context_menu_observer;


extern const TCHAR* const kMenuUIClassName;// = _T("MenuUI");
extern const TCHAR* const kMenuUIInterfaceName;// = _T("Menu");

class CListUI;
/** 菜单控件 */
class CULIB_API CMenuUI : public CListUI
{
public:
	CMenuUI();
	~CMenuUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoEvent(TEventUI& event);

	/** 向菜单中增加菜单项 */
    virtual bool Add(CControlUI* pControl,CControlUI* pParent=NULL);
	/** 在指定位置向菜单中增加菜单项 */
    virtual bool AddAt(CControlUI* pControl, int iIndex);

	/** 得到菜单项位置 */
    virtual int GetItemIndex(CControlUI* pControl) const;
	/** 设置菜单项位置 */
    virtual bool SetItemIndex(CControlUI* pControl, int iIndex);
	/** 移除菜单项 */
    virtual bool Remove(CControlUI* pControl);

	SIZE EstimateSize(SIZE szAvailable);

	/** 设置属性 */
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

/////////////////////////////////////////////////////////////////////////////////////
//

// MenuElementUI
extern const TCHAR* const kMenuElementUIClassName;// = _T("MenuElementUI");
extern const TCHAR* const kMenuElementUIInterfaceName;// = _T("MenuElement);

class CMenuElementUI;
/**
 *  菜单窗口是popup类型的弹出窗口，负责菜单的显示，菜单项是菜单窗口
 *  的子控件，菜单窗口也是观察者模式中的观察者
 */
class  CULIB_API CMenuWnd : public CWindowWnd, public ContextMenuReceiver
{
public:
	CMenuWnd(HWND hParent = NULL);
    void Init(CControlUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point);
    LPCTSTR GetWindowClassName() const;
    void OnFinalMessage(HWND hWnd);

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(UINT uMag, WPARAM wParam, LPARAM lParam,BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	BOOL Receive(ContextMenuParam param);

public:
	HWND m_hParent;
	POINT m_BasedPoint;
	STRINGorID m_xml;
	CStdString m_sType;
    CPaintManagerUI m_pm;
    CMenuElementUI* m_pOwner;
	CControlUI* m_pControl;
    CMenuUI* m_pLayout;
};

class CListContainerElementUI;
/** 菜单项 */
class CULIB_API CMenuElementUI : public CListContainerElementUI
{
	friend CMenuWnd;
public:
    CMenuElementUI();
	~CMenuElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

	/** 绘制自身 */
    void DoPaint(HDC hDC, const RECT& rcPaint);
    /** 绘制文本 */
	void DrawItemText(HDC hDC, const RECT& rcItem);

	SIZE EstimateSize(SIZE szAvailable);

	/** 菜单项被激活后的响应函数 */
	bool Activate();

	void DoEvent(TEventUI& event);

	CMenuWnd* GetMenuWnd();

	void CreateMenuWnd();

protected:
	CMenuWnd* m_pWindow;
};

} 

#endif // 
