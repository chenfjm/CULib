/**  
*  @file  CUBase.h  
*  @brief  
*  @author        
*  @date  
*  @version  
*   
*/
#ifndef __CUBASE_H_
#define __CUBASE_H_

#pragma once

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

namespace CULib{

#define WC_EDIT                L"Edit"

/** 显示断言所在文件，行以及expr的内容 */
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

#ifdef _DEBUG
#ifndef TRACE
#define TRACE __Trace
#endif
#define TRACEMSG __TraceMsg
#else
#ifndef TRACE
#define TRACE
#endif
#define TRACEMSG _T("")
#endif

/** 将windows消息转化成其对应的宏名 */
void CULIB_API __Trace(LPCTSTR pstrFormat, ...);
LPCTSTR CULIB_API __TraceMsg(UINT uMsg);

/** 用来统一描述资源ID或字符串 */
class  STRINGorID
{
public:
	STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
	{ }
	STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
	{ }
	LPCTSTR m_lpstr;
};

/** 坐标点的抽象 */
class CULIB_API CPoint : public tagPOINT
{
public:
	CPoint();
	CPoint(const POINT& src);
	CPoint(int x, int y);
	CPoint(LPARAM lParam);
};

/** 窗体大小的抽象 */
class CULIB_API CSize : public tagSIZE
{
public:
	CSize();
	CSize(const SIZE& src);
	CSize(const RECT rc);
	CSize(int cx, int cy);
};

/** 矩形形状的抽象 */
class CULIB_API CRect : public tagRECT
{
public:
	CRect();
	CRect(const RECT& src);
	CRect(int iLeft, int iTop, int iRight, int iBottom);

	int GetWidth() const;
	int GetHeight() const;
	void Empty();
	bool IsNull() const;
	void Join(const RECT& rc);
	void ResetOffset();
	void Normalize();
	void Offset(int cx, int cy);
	void Inflate(int cx, int cy);
	void Deflate(int cx, int cy);
	void Union(CRect& rc);
};

/**  指针数组 */
class CULIB_API CStdPtrArray
{
public:
	CStdPtrArray(int iPreallocSize = 0);
	CStdPtrArray(const CStdPtrArray& src);
	~CStdPtrArray();

	void Empty();
	void Resize(int iSize);
	bool IsEmpty() const;
	int Find(LPVOID iIndex) const;
	bool Add(LPVOID pData);
	bool SetAt(int iIndex, LPVOID pData);
	bool InsertAt(int iIndex, LPVOID pData);
	bool Remove(int iIndex);
	int GetSize() const;
	LPVOID* GetData();

	LPVOID GetAt(int iIndex) const;
	LPVOID operator[] (int nIndex) const;

protected:
	LPVOID* m_ppVoid;
	int m_nCount;
	int m_nAllocated;
};

/**  值数组 */
class CULIB_API CStdValArray
{
public:
	CStdValArray(int iElementSize, int iPreallocSize = 0);
	~CStdValArray();

	void Empty();
	bool IsEmpty() const;
	bool Add(LPCVOID pData);
	bool Remove(int iIndex);
	int GetSize() const;
	LPVOID GetData();

	LPVOID GetAt(int iIndex) const;
	LPVOID operator[] (int nIndex) const;

protected:
	LPBYTE m_pVoid;
	int m_iElementSize;
	int m_nCount;
	int m_nAllocated;
};

/** 字符串封装 */
class CULIB_API CStdString
{
public:
	enum { MAX_LOCAL_STRING_LEN = 63 };

	CStdString();
	CStdString(const TCHAR ch);
	CStdString(const CStdString& src);
	CStdString(LPCTSTR lpsz, int nLen = -1);
	~CStdString();

	void Empty();

	/** 得到字符串的长度 */
	int GetLength() const;
	/** 判断字符串是否为空 */
	bool IsEmpty() const;
	/** 获取和设置索引位置的字符 */
	TCHAR GetAt(int nIndex) const;
	void SetAt(int nIndex, TCHAR ch);
	void Append(LPCTSTR pstr);

	/** 分配并存储字符串 */
	void Assign(LPCTSTR pstr, int nLength = -1);
	LPCTSTR GetData();

	operator LPCTSTR() const;

	TCHAR operator[] (int nIndex) const;
	const CStdString& operator=(const CStdString& src);
	const CStdString& operator=(const TCHAR ch);
	const CStdString& operator=(LPCTSTR );

	const CStdString& CStdString::operator=(LPCSTR lpStr);
	const CStdString& CStdString::operator+=(LPCSTR lpStr);

	CStdString operator+(const CStdString& src) const;
	CStdString operator+(LPCTSTR pstr) const;
	const CStdString& operator+=(const CStdString& src);
	const CStdString& operator+=(LPCTSTR pstr);
	const CStdString& operator+=(const TCHAR ch);

	bool operator == (LPCTSTR str) const;
	bool operator != (LPCTSTR str) const;
	bool operator <= (LPCTSTR str) const;
	bool operator <  (LPCTSTR str) const;
	bool operator >= (LPCTSTR str) const;
	bool operator >  (LPCTSTR str) const;

	int Compare(LPCTSTR pstr) const;
	int CompareNoCase(LPCTSTR pstr) const;

	void MakeUpper();
	void MakeLower();

	CStdString Left(int nLength) const;
	CStdString Mid(int iPos, int nLength = -1) const;
	CStdString Right(int nLength) const;

	int Find(TCHAR ch, int iPos = 0) const;
	int Find(LPCTSTR pstr, int iPos = 0) const;
	int ReverseFind(TCHAR ch) const;
	int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

	int __cdecl Format(LPCTSTR pstrFormat, ...);
	int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

protected:
	LPTSTR m_pstr;
	TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
};


struct TITEM
{
	CStdString Key;
	LPVOID Data;
	struct TITEM* pNext;
};

/** 字符串-指针map */
class CULIB_API CStdStringPtrMap
{
public:
	CStdStringPtrMap(int nSize = 83);
	~CStdStringPtrMap();

	void Resize(int nSize = 83);
	LPVOID Find(LPCTSTR key) const;
	bool Insert(LPCTSTR key, LPVOID pData);
	LPVOID Set(LPCTSTR key, LPVOID pData);
	bool Remove(LPCTSTR key);
	int GetSize() const;
	LPCTSTR GetAt(int iIndex) const;
	LPCTSTR operator[] (int nIndex) const;
protected:
	TITEM** m_aT;
	int m_nBuckets;
};

/** 窗口的封装类 */
class CULIB_API CWindowWnd
{
public:
	CWindowWnd();

	HWND GetHWND() const;
	operator HWND() const;

	/** RegisterClass的封装 */
	bool RegisterWindowClass();

	/** 超类化窗口 */
	bool RegisterSuperclass();

	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	HWND Subclass(HWND hWnd);
	void Unsubclass();
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);
	UINT ShowModal();
	void Close(UINT nRet = IDOK);
	void CenterWindow();
	void SetIcon(UINT nRes);

	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	void ResizeClient(int cx = -1, int cy = -1);

protected:
	virtual LPCTSTR GetWindowClassName() const = 0;
	virtual LPCTSTR GetSuperClassName() const;
	virtual UINT GetClassStyle() const;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	bool m_bSubclassed;
};

/** 
 *@brief	沙漏形鼠标
 *构造函数中设置鼠标形状为沙漏
 *析构函数中还原鼠标形状
 */
class CULIB_API CWaitCursor
{
public:
	CWaitCursor();
	~CWaitCursor();

protected:
	HCURSOR m_hOrigCursor;
};

}

#endif