/**  
*  @file      CUManager.h
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include <commctrl.h>
#pragma once

namespace CULib {

class CControlUI;

typedef enum EVENTTYPE_UI
{
	UIEVENT__FIRST = 1,
	UIEVENT__KEYBEGIN,
	UIEVENT_KEYDOWN,
	UIEVENT_KEYUP,
	UIEVENT_CHAR,
	UIEVENT_SYSKEY,
	UIEVENT__KEYEND,
	UIEVENT__MOUSEBEGIN,
	UIEVENT_MOUSEMOVE,
	UIEVENT_MOUSELEAVE,
	UIEVENT_MOUSEENTER,
	UIEVENT_MOUSEHOVER,
	UIEVENT_BUTTONDOWN,
	UIEVENT_BUTTONUP,
	UIEVENT_RBUTTONDOWN,
	UIEVENT_DBLCLICK,
	UIEVENT_CONTEXTMENU,
	UIEVENT_SCROLLWHEEL,
	UIEVENT__MOUSEEND,
	UIEVENT_KILLFOCUS,
	UIEVENT_SETFOCUS,
	UIEVENT_WINDOWSIZE,
	UIEVENT_SETCURSOR,
	UIEVENT_TIMER,
	UIEVENT_NOTIFY,
	UIEVENT_COMMAND,
	UIEVENT__LAST,
};

/** CControlUI::GetControlFlags()函数的标志 */
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

/** FindControl()函数的标志 */
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

/** CDialogLayout stretching 的标志 */
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020

/** 控制绘画的标志 */
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040

/** 定时器ID */
#define MOUSEMOVE_TIMERID	0x00000001

/** 字体信息结构体 */
typedef struct tagTFontInfo
{
	HFONT hFont;
	CStdString sFontName;
	int iSize;
	bool bBold;
	bool bUnderline;
	bool bItalic;
	TEXTMETRIC tm;
} TFontInfo;

/** 图片信息结构体 */
typedef struct tagTImageInfo
{
	HBITMAP hBitmap;
	int nX;
	int nY;
	bool alphaChannel;
	CStdString sResType;
	DWORD dwMask;
} TImageInfo;

/** 系统事件的结构体 */
typedef struct tagTEventUI
{
	int Type;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	TCHAR chKey;
	WORD wKeyState;
	WPARAM wParam;
	LPARAM lParam;
} TEventUI;

/** 面向应用的通知结构体 */
typedef struct tagTNotifyUI 
{
	CStdString sType;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
} TNotifyUI;

/**  父控件相对位置的结构体 */
typedef struct tagTRelativePosUI
{
	bool bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
}TRelativePosUI;

/** 通知接口 */
class INotifyUI
{
public:
	virtual void Notify(TNotifyUI& msg) = 0;
};

/** 消息过滤器接口 */
class IMessageFilterUI
{
public:
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);

class CULIB_API CPaintManagerUI
{
public:
	CPaintManagerUI();
	~CPaintManagerUI();

public:

	/** 初始化 */
	void Init(HWND hWnd);

	/** 设置界面更新标志位 */
	void NeedUpdate();

	/** InvalidateRect API的封装 */
	void Invalidate(RECT& rcItem);

	/** 得到设备句柄 */
	HDC GetPaintDC() const;

	/** 得到窗口句柄 */
	HWND GetPaintWindow() const;

	/** 得到鼠标位置 */
	POINT GetMousePos() const;

	/** 得到客户端区域大小 */
	SIZE GetClientSize() const;

	/** 设置和获取窗口初始化时大小 */
	SIZE GetInitSize();
	void SetInitSize(int cx, int cy);

	/** 获取和设置窗口边框大小 */
	RECT& GetSizeBox();
	void SetSizeBox(RECT& rcSizeBox);

	/** 设置和获取标题栏大小 */
	RECT& GetCaptionRect();
	void SetCaptionRect(RECT& rcCaption);
	
	/** 设置和获取窗口圆角大小 */
	SIZE GetRoundCorner() const;
	void SetRoundCorner(int cx, int cy);

	/** 设置和获得窗口最小大小 */
	SIZE GetMinInfo() const;
	void SetMinInfo(int cx, int cy);

	/** 设置和获取窗口最大大小 */
	SIZE GetMaxInfo() const;
	void SetMaxInfo(int cx, int cy);

	/** 设置窗体透明度 */
	void SetTransparent(int nOpacity);

	/** 设置背景的透明度 */
	void SetBackgroundTransparent(bool bTrans);

	/** 设置和获取窗口更新信息 */
	bool IsShowUpdateRect() const;
	void SetShowUpdateRect(bool show);

	/** 设置和获取窗口实例句柄 */
	static HINSTANCE GetInstance();
    static void SetInstance(HINSTANCE hInst);
	
	/** 设置实例路径 */
	static CStdString GetInstancePath();
	
	/** 设置和获取资源DLL */
	static void SetResourceDll(HINSTANCE hInst);
	static HINSTANCE GetResourceDll();

	/** 设置和获取资源路径 */
	static void SetResourcePath(LPCTSTR pStrPath);
	static const CStdString& GetResourcePath();

	/** 设置和获取资源名称 */
	static void SetResourceZip(LPVOID pVoid, unsigned int len);
	static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false);
	static const CStdString& GetResourceZip();

	/** 资源是否缓存 */
	static bool IsCachedResourceZip();

	/** 得到资源句柄 */
	static HANDLE GetResourceZipHandle();


	static void GetHSL(short* H, short* S, short* L);
	static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 

	/** 重新加载皮肤 */
	static void ReloadSkin();

	/** 加载和获取插件 */
	static bool LoadPlugin(LPCTSTR pstrModuleName);
	static CStdPtrArray* GetPlugins();
	
	/** 设置和获取父资源管理指针 */
	bool UseParentResource(CPaintManagerUI* pm);
	CPaintManagerUI* GetParentResource() const;

	/** 设置和获取非可用状态文字颜色值 */
	DWORD GetDefaultDisabledColor() const;
	void SetDefaultDisabledColor(DWORD dwColor);
	
	/** 设置和获取默认字体颜色值 */
	DWORD GetDefaultFontColor() const;
	void SetDefaultFontColor(DWORD dwColor);

	/** 设置和获取超链接字体颜色值 */
	DWORD GetDefaultLinkFontColor() const;
	void SetDefaultLinkFontColor(DWORD dwColor);

	/** 设置和获取停留到超链接时的字体颜色 */
	DWORD GetDefaultLinkHoverFontColor() const;
	void SetDefaultLinkHoverFontColor(DWORD dwColor);

	/** 设置和获取选中后背景颜色 */
	DWORD GetDefaultSelectedBkColor() const;
	void SetDefaultSelectedBkColor(DWORD dwColor);

	/** 设置和获取默认字体 */
	TFontInfo* GetDefaultFontInfo();
	void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/** 得到字体列表中项数量 */
	DWORD GetCustomFontCount() const;

	/** 加入一种字体到字体列表中 */
	HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/** 加入一种字体到字体列表的指定位置 */
	HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/** 按照序号得到字体列表中的字体 */
	HFONT GetFont(int index);

	/** 按字体属性得到字体列表中的字体 */
	HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/** 按句柄查找字体列表中的一项 */
	bool FindFont(HFONT hFont);

	/** 按属性查找字体列表中的一项 */
	bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/** 得到字体序号 */
	int GetFontIndex(HFONT hFont);
	int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	
	/** 按句柄移除字体列表中的指定项 */
	bool RemoveFont(HFONT hFont);

	/** 按序号移除字体列表中的指定项 */
	bool RemoveFontAt(int index);

	/** 移除字体列表中的所有项 */
	void RemoveAllFonts();

	/** 按序号得到字体列表中项的属性 */
	TFontInfo* GetFontInfo(int index);

	/** 按句柄得到字体列表中项的属性 */
	TFontInfo* GetFontInfo(HFONT hFont);

	/** 获取图片项 如果列表中不存依次从其上层绘制管理者中获取 */
	const TImageInfo* GetImage(LPCTSTR bitmap);

	/** 获取图片项 如果不存在则向图片列表中添加一项 */
	const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);

	/** 添加图片到hash中 */
	const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
	const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
	
	/** 移除图片 */
	bool RemoveImage(LPCTSTR bitmap);

	/** 移除图片列表中所有项 */
	void RemoveAllImages();

	/** 重新加载所有图片 */
	void ReloadAllImages();

	/** 加入和获取默认属性列表 */
	void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
	LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
	
	/** 从默认属性列表中移除项 */
	bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);

	/** 获取默认属性列表中的项 */
	const CStdStringPtrMap& GetDefaultAttribultes() const;

	/** 移除默认属性列表中所有项 */
	void RemoveAllDefaultAttributeList();

	/** 将控件树附加到当前绘制管理者 */
	bool AttachDialog(CControlUI* pControl);

	/** 初始化控件 把控件名称加入到绘制管理者的控件名map中 */
	bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
	
	/** 重置、删除控件 */
	void ReapObjects(CControlUI* pControl);

	/** 添加控件到组中 */
	bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);

	/** 通过名称得到选项组 */
	CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);

	/** 移除组中控件 如果组中没有控件则移除组 */
	void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);

	/** 移除所有选项组 */
	void RemoveAllOptionGroups();

	/** 得到控件焦点 */
	CControlUI* GetFocus() const;

	/** 控件获得焦点 */
	void SetFocus(CControlUI* pControl);
	void SetFocusNeeded(CControlUI* pControl);

	
	bool SetNextTabControl(bool bForward = true);
	bool SetLeftControl();//设置左控件
	bool SetRightControl();//
	bool SetUpControl();
	bool SetDownControl();


	/** 设置和清除定时器 */
	bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
	bool KillTimer(CControlUI* pControl, UINT nTimerID);
	void KillTimer(CControlUI* pControl);
	void RemoveAllTimers();

	/** 捕获和释放鼠标、鼠标是否被捕获 */
	void SetCapture();
	void ReleaseCapture();
	bool IsCaptured();

	/** 增加移除一个Listener */
	bool AddNotifier(INotifyUI* pControl);
	bool RemoveNotifier(INotifyUI* pControl);

	/** 发送一个通知 */
	void SendNotify(TNotifyUI& Msg, bool bAsync = false);
	void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

	/** 加入和移除一个预处理Message Filter */
	bool AddPreMessageFilter(IMessageFilterUI* pFilter);
	bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

	/** 加入和移除一个Message Filter */
	bool AddMessageFilter(IMessageFilterUI* pFilter);
	bool RemoveMessageFilter(IMessageFilterUI* pFilter);

	/** 得到PostPaint控件数量 */
	int GetPostPaintCount() const;

	/** 加入和移除一个PostPaint控件 */
	bool AddPostPaint(CControlUI* pControl);
	bool RemovePostPaint(CControlUI* pControl);

	/** 设置控件到指定的序列位置 */
	bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

	/** 加入控件到延迟处理列表当中 */
	void AddDelayedCleanup(CControlUI* pControl);

	/** 得到根控件 */
	CControlUI* GetRoot() const;

	/** 调用根节点控件的函数, 按位置匹配控件 */
	CControlUI* FindControl(POINT pt) const;

	/** 在控件名map中按名字匹配控件 */
	CControlUI* FindControl(LPCTSTR pstrName) const;

	/** FindControl(CControlUI*, POINT) 调用父控件的函数, 查找控件 */
	CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
	CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
	CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
	CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
	CStdPtrArray* GetSubControlsByClass();

	/** 消息循环 */
	static void MessageLoop();
	static bool TranslateMessage(const LPMSG pMsg);
	static void Term();

	/** 消息处理和消息预处理 */
	bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

private:
	static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlsFromLeft(CControlUI* pThis, LPVOID pData);//向左找
	static CControlUI* CALLBACK __FindControlsFromRight(CControlUI* pThis, LPVOID pData);//向右找
	static CControlUI* CALLBACK __FindControlsFromUp(CControlUI* pThis, LPVOID pData);//向上找
	static CControlUI* CALLBACK __FindControlsFromDown(CControlUI* pThis, LPVOID pData);//向下找

private:
	/** 绘制窗口句柄 */
	HWND m_hWndPaint;
	/** 绘制DC */
	HDC m_hDcPaint;
	/** 屏幕兼容内存DC */
	HDC m_hDcOffscreen;
	/** 背景DC */
	HDC m_hDcBackground;
	/** 屏幕DIB */
	HBITMAP m_hbmpOffscreen;
	/** 背景DIB */
	HBITMAP m_hbmpBackground;
	/** 是否更新 */
	bool m_bShowUpdateRect;

	/** 根控件指针 */
	CControlUI* m_pRoot;
	/** 获得焦点控件指针 */
	CControlUI* m_pFocus;
	/** 鼠标停留控件指针 */
	CControlUI* m_pEventHover;
	/** 点击事件控件指针 */
	CControlUI* m_pEventClick;
	/** 相应键盘事件控件指针 */
	CControlUI* m_pEventKey;
	/** 鼠标位置 */
	POINT m_ptLastMousePos;
	POINT m_ptLastScreenMouse;
	/** 窗口最小限制 */
	SIZE m_szMinWindow;
	/** 窗口最大限制 */
	SIZE m_szMaxWindow;
	/** 初始化窗口大小 */
	SIZE m_szInitWindowSize;
	/** 窗口内边距 */
	RECT m_rcSizeBox;
	/** 圆角大小 */
	SIZE m_szRoundCorner;
	/** 标题大小 */
	RECT m_rcCaption;
	/** 窗口坐标 */
	RECT m_swPosition;
	/** 计时器ID */
	UINT m_uTimerID;
	/** 是否为第一层 */
	bool m_bFirstLayout;
	/** 是否需要更新 */
	bool m_bUpdateNeeded;
	/** 是否需要焦点 */
	bool m_bFocusNeeded;
	/** 是否绘制m_hDcOffscreen */
	bool m_bOffscreenPaint;
	/** 背景是否透明 */
	bool m_bAlphaBackground;
	/** 是否追踪鼠标 */
	bool m_bMouseTracking;
	/** 是否捕获鼠标 */
	bool m_bMouseCapture;
	/** 鼠标拖拽窗口标志 */
	bool m_bMoveFlag;
	
	
	/** 计时器列表 */
	CStdPtrArray m_aTimers;
	/** 预处理消息列表 */
	CStdPtrArray m_aPreMessageFilters;
	/** 消息处理列表 */
	CStdPtrArray m_aMessageFilters;
	/** PostPaint控件列表 */
	CStdPtrArray m_aPostPaintControls;
	/** 延迟释放控件列表 */
	CStdPtrArray m_aDelayedCleanup;
	/** 消息异步处理 */
	CStdPtrArray m_aAsyncNotify;
	CStdPtrArray m_aFoundControls;
	/** 控件名哈希列表 */
	CStdStringPtrMap m_mNameHash;
	/** 选项组列表 */
	CStdStringPtrMap m_mOptionGroup;
	
	/** 当前绘制管理者的上一层绘制管理 */
	CPaintManagerUI* m_pParentResourcePM;
	/** 默认非可用状态字体颜色 */
	DWORD m_dwDefaultDisabledColor;
	/** 默认字体颜色 */
	DWORD m_dwDefaultFontColor;
	/** 默认链接颜色 */
	DWORD m_dwDefaultLinkFontColor;
	/** 默认鼠标漂浮时字体颜色 */
	DWORD m_dwDefaultLinkHoverFontColor;
	/** 默认被选择后背景颜色 */
	DWORD m_dwDefaultSelectedBkColor;
	/** 默认字体信息 */
	TFontInfo m_DefaultFontInfo;
	/** 字体列表 */
	CStdPtrArray m_aCustomFonts;

	/** 图片资源名哈希表 */
	CStdStringPtrMap m_mImageHash;
	/** 默认属性哈希表 */
	CStdStringPtrMap m_DefaultAttrHash;
	
	/** 应用程序对象实例句柄 */
	static HINSTANCE m_hInstance;
	/** 资源对象实例句柄 */
	static HINSTANCE m_hResourceInstance;
	/** 资源路径 */
	static CStdString m_pStrResourcePath;
	/** zip包资源名称 */
	static CStdString m_pStrResourceZip;
	static bool m_bCachedResourceZip;
	static HANDLE m_hResourceZip;
	static short m_H;
	static short m_S;
	static short m_L;
	/** 预处理消息队列 */
	static CStdPtrArray m_aPreMessages;
	static CStdPtrArray m_aPlugins;
	/** Listener 列表 */
	static CStdPtrArray m_aNotifiers;
};

}
#endif