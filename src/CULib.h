/**  
*  @file	  CULib.h 
*  @brief  
*  @author        
*  @date  
*  @version 
* 下列 ifdef 块是创建使从 DLL 导出更简单的
* 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CULIB_EXPORTS
* 符号编译的。在使用此 DLL 的
* 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
* CULIB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
* 符号视为是被导出的。
*/
#ifdef  CULIB_EXPORTS
#define CULIB_API  __declspec(dllexport)
#else
#define CULIB_API  __declspec(dllimport)
#endif

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <olectl.h>
#include <tchar.h>
#include <assert.h>
#include <malloc.h>
#include <string>

#define  _CRT_SECURE_NO_WARNING

#include "CUBase.h"
#include "CUManager.h"
#include "CUDelegate.h"
#include "CUControl.h"
#include "CUContainer.h"
#include "CUCommonControl.h"
#include "CUList.h"
#include "CUMenu.h"
#include "CUCombo.h"
#include "CUActiveX.h"
#include "CMarkup.h"
#include "CUDlgBuilder.h"
#include "CURender.h"
#include "UIRichEdit.h"
#include "XUnzip.h"
#include "CULog.h"

#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))
