#pragma once
#include <fstream>
#include <Windows.h>

#include "CULib.h"

using namespace std;

using namespace CULib;

/************************************************************************/
/* 使用该类需要首先 调用 CreateLog(LogFileName) 创建一个LogFile, 然后
	每次调用 AppendLog(strMsg) 向文件中增加内容
                                                                     */
/************************************************************************/

class GELog  
{
public:
	/** 最后清除c_pGELog指针 */
	void FinalCleanup();

	/** 得到唯一的Log指针 */
	static GELog * GetSingleton() { return c_pGELog; };

	/** 向日志文件中加入消息 \n
	 * @param [string]	errFile: 文件名 一定要写 __FILE__
	 * @return 是否添加成功
	 */
	bool AppendLog(LPCTSTR pstrFormat, ...);
	void GetSyStime();

	/** 创建新的日志文件,同时会关闭旧的日志文件 \n
	 * @param [string] strLogFileName : 新的日志文件名
	 * @return 是否创建成功
	 */
	bool CreateLog(CStdString strLogFileName);

	
	virtual ~GELog();

private:
	GELog();

protected:
	CStdString m_slog;
	CStdString m_logname;
	CStdString m_buffer;
	CStdString m_time;
	SYSTEMTIME m_date;
	char* m_strdate;
	DWORD m_pos;
	HANDLE hFile;
	bool showtime;
	
	/** 静态类成员指针,用来GetSingleton*/
	static GELog * c_pGELog;

};