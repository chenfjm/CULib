#include "CULog.h"
#include <assert.h>
#include <iomanip>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GELog * GELog::c_pGELog = new GELog;

GELog::GELog()
{
	static bool IfHasSingleton = false;

	assert(!IfHasSingleton);

	IfHasSingleton = true;

	showtime=false;

	ZeroMemory(&m_date,sizeof(m_date));
	m_strdate=(char*)malloc(sizeof(char)*60);
}

GELog::~GELog()
{
	
}



bool GELog::CreateLog(CStdString strLogFileName)
{

	hFile=::CreateFile(strLogFileName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ, \
		NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		OutputDebugString(_T("CreateLogFile fail!\r\n")); 
	}

	AppendLog(_T("----------------------------LOG FILE OF  UILib-----------------------------"));
	AppendLog(_T("                         COPYRIGHT(C) 2013  UILib"));
	AppendLog(_T("---------------------------------------------------------------------------"));
	showtime=true;
	
	return true;
}

bool GELog::AppendLog(LPCTSTR pstrFormat, .../*const string& strMsg, const string& errFile, int errLine*/)
{
	TCHAR szBuffer[1025] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::wvsprintf(szBuffer,pstrFormat, argList);
	va_end(argList);
	m_buffer.Empty();
	if (showtime)
	{
		GetSyStime();
		m_buffer=m_time+_T("  ");
	}
	m_buffer.Append(szBuffer);
	m_buffer.Append(_T("\r\n"));

	DWORD nWide=::WideCharToMultiByte(CP_UTF8,0,m_buffer.GetData(),m_buffer.GetLength(),NULL,0,NULL,NULL);
	char tempstr[500];//tempstr=(LPSTR)malloc(sizeof(char)*(nWide+1));
	ZeroMemory(tempstr,sizeof(LPCSTR)*(nWide+1));
	::WideCharToMultiByte(CP_UTF8,0,m_buffer.GetData(),m_buffer.GetLength(),tempstr,nWide,NULL,NULL);
	tempstr[nWide]='\0';	
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		int n=m_buffer.GetLength();
		BOOL bRet=::WriteFile(hFile,tempstr,nWide,&m_pos,NULL);	
	}


	return true;
}

void GELog::GetSyStime()
{
	
	::GetLocalTime(&m_date);
	m_time.Empty();
	m_time.Format(_T("%d/%d/%d %d:%d:%d"),m_date.wYear,m_date.wMonth,\
		m_date.wDay,m_date.wHour,m_date.wMinute,m_date.wSecond);

}

void GELog::FinalCleanup()
{
	if (c_pGELog)
	{
		delete c_pGELog;
		c_pGELog = NULL;
	}
}

