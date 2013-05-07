/**  
*  @file   CUDlgBuilder.cpp
*  @brief  此文件中的函数通过调用解析库解析xml，并生成控件 
*  
*		   LoadFromFile函数调用LoadFromMem函数，后者调用一系列zip函数进行解压缩
*		   前边的Create函数是包装函数，具体操作在后边的Create函数中，其中的内容
*		   是对通用元素的解析。具体控件的解析在parse函数中。
* @author  chencheng
* @data    2013-04-05
*/

#ifndef __CUDLGBUILDER_H__
#define __CUDLGBUILDER_H__

#pragma once

#include "Markup.h"
namespace CULib {

enum
{
	XMLFILE_ENCODING_UTF8 = 0,
	XMLFILE_ENCODING_UNICODE = 1,
	XMLFILE_ENCODING_ASNI = 2,
};

/** 构建UI时回调类 用于创造用户自定义的控件 */
class IDialogBuilderCallback
{
public:
	/** 创建用户自定义控件 */
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) = 0;
};

/** UI构建者, 用于构建UI */
class CULIB_API CDialogBuilder
{
public:
	CDialogBuilder();
	/** 构建UI */
	CControlUI* Create(STRINGorID xml, IDialogBuilderCallback* pCallback=NULL, 
		CPaintManagerUI* pManager=NULL, CControlUI* pParent=NULL, LPCTSTR type = NULL);
	bool _Failed(LPCTSTR pstrError, LPCTSTR pstrLocation=NULL);
	bool LoadFromFile(LPCTSTR pstrFilename, int encoding=XMLFILE_ENCODING_UNICODE);
	bool LoadFromMem(LPCTSTR pstrFilename, int encoding=XMLFILE_ENCODING_UNICODE);
	CControlUI* Create(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent);
	CControlUI* parse(CControlUI* pParent,CPaintManagerUI* pManager);
private:
	CMarkup m_xml;
	IDialogBuilderCallback* m_pCallback;
	/**  资源dll */
	LPCTSTR m_pstrtype;     

	TCHAR m_szErrorMsg[100];
	TCHAR m_szErrorXML[50];
};

} 
#endif

