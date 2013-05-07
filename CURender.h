/**  
*  @file	  CURender.h  
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __CURENDER_H__
#define __CURENDER_H__

#pragma once

namespace CULib {

/** 渲染区域控制，设置在dc上绘制的区域，防止子控件绘制到父控件外 */
class CULIB_API CRenderClip
{
public:
	~CRenderClip();
	RECT rcItem;
	HDC hDC;
	HRGN hRgn;
	HRGN hOldRgn;
	
	/** 产生一个剪切区域 */
	static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
	/** 产生一个圆角剪切区域 **/
	static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
	/** 使用整个活跃区域作为剪切区 */
	static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
	/** 使用当前控件有效区域作为剪切区 */
	static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
};

/** 渲染引擎 */
class CULIB_API CRenderEngine
{
public:
	static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
	/** 加载图像 */
	static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);
	static void FreeImage(const TImageInfo* bitmap);
	/** 绘制图像 */
	static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
		const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
		bool hole = false, bool xtiled = false, bool ytiled = false);
	/** 按标识字符串绘制图像 */
	static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
		LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
	static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
	/** 绘制渐变 */
	static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

	/** 以下函数中的颜色参数alpha值无效 */
	/** 绘制直线 */
	static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
	/** 绘制矩形 */
	static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
	/** 绘制圆角矩形 */
	static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor);
	/** 绘制文字 */
	static void DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
		DWORD dwTextColor, int iFont, UINT uStyle);
	/** 绘制 "mini-html" 标识字符串 */
	static void DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, 
		DWORD dwTextColor, RECT* pLinks, CStdString* sLinks, int& nLinkRects, UINT uStyle);
	/** 产生一个BMP位图 */
	static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc);
};

} // namespace DuiLib

#endif // __UIRENDER_H__
