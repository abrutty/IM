

#include "stdafx.h"

#include "resource.h"
#include "Startup.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "UpdateDlg.h"
#include "Updater.h"
#include "LoginSettingsDlg.h"
#include "File.h"
#include "IULog.h"
#include "UserSessionData.h"
#include "Utils.h"

CAppModule _Module;

//关于g_hwndOwner的说明：
//之所以创建这个窗口为为了产生主窗口和登录对话框不在任务栏显示的效果，
//如果单纯地将窗口风格由WS_EX_APPWINDOW设置为WS_EX_TOOLWINDOW后，此种风格的窗口
//会在失去焦点后默认Z轴变为0，这点特别讨厌。
HWND	   g_hwndOwner = NULL;	

HWND CreateOwnerWindow()
{
	PCTSTR pszOwnerWindowClass = _T("__EdoyunIMClient_Owner__");
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = pszOwnerWindowClass;
	wcex.hIconSm = NULL;

	if (!::RegisterClassEx(&wcex))
		return NULL;

	HWND hOwnerWindow = ::CreateWindow(pszOwnerWindowClass, NULL, WS_OVERLAPPEDWINDOW, 
									   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	
	return hOwnerWindow;
}

int Run(LPTSTR /*lpstrCmdLine = NULL*/, int nCmdShow/* = SW_SHOWDEFAULT*/)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop); // 加载消息循环

	CMainDlg dlgMain;
	
	g_hwndOwner = CreateOwnerWindow();
	if(dlgMain.Create(g_hwndOwner) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}


BOOL InitSocket()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int nErrorID = ::WSAStartup(wVersionRequested, &wsaData); // WSAStartup()主要就是进行相应的socket库绑定。
	if(nErrorID != 0)
		return FALSE;

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        UnInitSocket();
        return FALSE;
    }

	return TRUE;
}

void UnInitSocket()
{
	::WSACleanup();
}

//默认的入口函数 WinMain main _tWinMain _tmain
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDebugFillThreshold( 0 ); // 设置调试缓冲区，设为0表示如果有日志，立马显示出来
#endif
	_tcscpy_s(g_szHomePath, MAX_PATH, Edoyun::CPath::GetAppPath().c_str());		// 拷贝应用路径，宽字节的
	strcpy_s(g_szHomePathAscii, MAX_PATH, Edoyun::CPath::GetAppPathAscii().c_str()); // 多字节的

	SYSTEMTIME st = {0};
	::GetLocalTime(&st);
	TCHAR szLogFileName[MAX_PATH] = {0};
	// 时间日志 年月日时分秒
	_stprintf_s(szLogFileName, MAX_PATH, _T("%sLog\\%04d%02d%02d%02d%02d%02d.log"), g_szHomePath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	CIULog::Init(TRUE, szLogFileName); // 初始化日志


	if(!InitSocket())
		return 0;

	tstring strFileName = Edoyun::CPath::GetAppPath() + _T("richFace.dll");
	BOOL bRet = DllRegisterServer(strFileName.c_str());	// 注册COM组件
	if (!bRet)
	{
		::MessageBox(NULL, _T("COM组件注册失败，应用程序无法完成初始化操作！"), _T("提示"), MB_OK);
		return 0;
	}

	HRESULT hRes = ::OleInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
	HMODULE hRichEditDll = ::LoadLibrary(CRichEditCtrl::GetLibraryName());	// 加载RichEdit控件DLL

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	CSkinManager::Init();	// 初始化皮肤管理器 （只能初始化一次，单例模式）
	
	tstring strSkinPath = Edoyun::CPath::GetAppPath() + _T("Skins\\");	// 设置皮肤文件夹路径
	CSkinManager::GetInstance()->SetSkinPath(strSkinPath.c_str());
	
	CSkinManager::GetInstance()->LoadConfigXml();	// 加载皮肤列表配置文件

	int nRet = Run(lpstrCmdLine, nCmdShow);

	CSkinManager::UnInit();	// 反初始化皮肤管理器

	if (hRichEditDll != NULL)		// 卸载RichEdit控件DLL
	{
		::FreeLibrary(hRichEditDll);
		hRichEditDll = NULL;
	}

	_Module.Term();

	UnInitSocket();

	CIULog::Unit();
	
	::OleUninitialize();

	return nRet;
}
