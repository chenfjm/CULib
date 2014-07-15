CULib
=====   

该项目基于Duilib界面库，实现一个运行于Wince平台的界面库CULib。该界面库针对嵌入式硬件的特点和Wince操作系统的特性对Duilib进行重构和优化。CULib将用户界面和处理逻辑分离，极大地提高用户界面的开发效率。并且使用xml来描述界面风格和界面布局，可以很方便地构建高效、绚丽、易于扩展的界面，而且xml语法简单，易于学习和掌握。     

在Wince环境下的GUI软件一般采用传统的MFC界面库，开发出的软件不美观、界面细节处理不好、使用硬编码、开发效率低下、生成程序体积大，而且传统MFC界面美化大都使用HOOK等对系统影响比较大的技术，可能会导致系统不稳定或引发其他错误。CULib库采用DirectUI技术，完全基于GDI在窗口上自绘，弥补了MFC等传统界面库在Wince上的不足。CULib的出现使得在Wince这样的嵌入式环境中构建绚丽的界面变得简单、高效。  


CULib具有以下一些特点：   

- 界面与业务逻辑分离；
- 使用xml配置界面；
- 界面布局方式灵活多样；
- 支持ZIP和DLL两种资源方式；
- 支持png、gif、jpg等多种图片格式；
- 内置常用的控件；
- 支持自定义控件；
- 支持插件系统；
- 强大的事件处理机制；
- 类html字符串绘制技术；
- 基于GDI和脏矩形的高效绘制技术；
- 支持alpha混合和图片透明；
- 支持动态色调变换；
- 内存占用小。
- 支持键盘焦点切换。
- 增加日志功能。  


**源码结构**  

![](https://github.com/chenfjm/CULib/tree/gh-pages/images/MainFrame.jpg)   

**使用方法**  

- 界面库引入：  

    \#include <windows.h>
    \#include <objbase.h>
    \#include "CULib.h"
    using namespace CULib;
    \#ifdef _DEBUG
    \#pragma comment(lib,"..\\bin\\CULib_d.lib")
    \#else
    \#pragma comment(lib,"..\\bin\\CULib.lib")
    \#endif  

这部分主要是包含一些使用CULib所需的头文件，并自动链接相应的CULib库。一般不用改动，需要注意的是CULib.h头文件和CULib.lib等库文件在磁盘中的位置。  

- 窗口实例：  

    class CFrameWindowWnd : public CWindowWnd, public INotifyUI
    {
    public:
	CFrameWindowWnd() { };
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return  CS_DBLCLKS ; }
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init() 
	{
            /** 程序初始化 */
	}
	void Notify(TNotifyUI& msg)
	{
	   /** 事件处理 */
	}

	/** 消息处理 */
	LRESULT  OnCreate(uMsg, wParam, lParam, bHandled); 
	LRESULT  OnClose(uMsg, wParam, lParam, bHandled); 
	LRESULT  OnDestroy(uMsg, wParam, lParam, bHandled); 
	LRESULT  OnActivate(uMsg, wParam, lParam, bHandled);
	LRESULT  OnSize(uMsg, wParam, lParam, bHandled); 
	LRESULT  OnSysCommand(uMsg, wParam, lParam, bHandled); 
	LRESULT  HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		/** 消息响应 */
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
    	case WM_CREATE:     
     	  lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:   
      	  lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY: 
     	  lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_ACTIVATE:      
 	  lRes = OnActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:       
     	  lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND: 
      	  lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}
    public:
	CPaintManagerUI m_pm;
    };  

该部分是基本的窗口实现类和简单的消息响应，HandleMessage对消息进行预处理，之后传递给CPaintManagerUI::MessageHandler进行处理。Notify是对程序的事件处理，该事件通知是由控件对象向用户程序发送的。  

- 程序入口与CULib初始化：  

    int WINAPI WinMain(HINSTANCE hInstance, 
    HINSTANCE /*hPrevInstance*/,
    LPWSTR /*lpCmdLine*/,
     int nCmdShow)
    {
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CPaintManagerUI::SetResourceZip(_T("Res.zip"));

	HRESULT Hr = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
	if( FAILED(Hr) ) return 0;

	CFrameWindowWnd* pFrame = new CFrameWindowWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T(""), 0, 0L);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);
	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
    }    

主要的初始化是设置资源路径，设置资源文件以及创建窗口。最后进入消息循环CPaintManagerUI::MessageLoop()。
为了简化界面库的使用，库中提供了一个非常方便的工具：WindowImpBase。这个类将上述前两部分封装在其内部，并在其中提供了各种默认的虚函数，供派生类重载。通过这个类，用户可以非常方便的来实现一个简单的界面。  

更多内容请参见[CULib分析文档.pdf]()  
详细实现请参见Demo源码   

**Demo运行结果**  

![](https://github.com/chenfjm/CULib/tree/gh-pages/images/360Safe.jpg)
![](https://github.com/chenfjm/CULib/tree/gh-pages/images/menu.jpg)
![](https://github.com/chenfjm/CULib/tree/gh-pages/images/list.jpg)   

Duilib项目主页：[duilib.googlecode.com](http://duilib.googlecode.com)     