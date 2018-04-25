// ServiceRestTaskServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServiceRestTaskServer.h"
#include "CheckHandler.h"
#include "LogFileUtil.h"
#include "ServerHelper.h"
#include "ServerCheckNotifySink.h"
#include "ServerRecheckNotifySink.h"
#include "ServerCheckoutNotifySink.h"
#include "Include\BizClient.h"
#include "Include\IdrAgent.h"
#include "Include\idrdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

// 唯一的应用程序对象
SERVICE_STATUS        g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

VOID WINAPI		ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI		ServiceCtrlHandler(DWORD); //与Windows服务界面交互
BOOL WINAPI		ControlHandler(DWORD dwCtrlType); //debug运行时，相应ctrl+c停止任务

DWORD WINAPI	ServiceWorkerThread(LPVOID lpParam); //工作线程

#define ServiceName _T("Nuctech IPSTASKServer") 

int	g_nDay = 0;  //天，数字1—31
int	g_nMonth = 0; //
int	g_nYear = 0; //

CLogRecord logRecord(L"WebServerLog");

bool	g_bSubscribeCheck = false;  //从配置文件里面读取，是否订阅check角色任务
wstring g_wsCheckStatus = L"check";
bool	g_bSubscribeRecheck = false;
wstring g_wsRecheckStatus = L"recheck";
bool	g_bSubscribeCheckout = false;
wstring g_wsCheckoutStatus = L"checkout";

SimpleSyncQueue<SubscribeTask> g_checkQueue;
SimpleSyncQueue<SubscribeTask> g_recheckQueue;
SimpleSyncQueue<SubscribeTask> g_checkoutQueue;

std::vector<TaskItem>		g_vecTaskConfig;

std::mutex	g_checkLock;
std::vector<TaskInfo>		g_vecCheckTask;
std::wstring				g_wstrCheckTimeStamp;

std::mutex	g_recheckLock;
std::vector<TaskInfo>		g_vecRecheckTask;
std::wstring				g_wstrReCheckTimeStamp;

std::mutex	g_checkoutLock;
std::vector<TaskInfo>		g_vecCheckoutTask;
std::wstring				g_wstrCheckoutTimeStamp;

std::unique_ptr<CCheckHandler> g_httpCheckHandler;
std::unique_ptr<CReCheckHandler> g_httpReCheckHandler;
std::unique_ptr<CCheckoutHandler> g_httpCheckoutHandler;

std::shared_ptr<CServerCheckNotifySink>		g_CheckNotifySink = NULL;
std::shared_ptr<CServerRecheckNotifySink>	g_RecheckNotifySink = NULL;
std::shared_ptr<CServerCheckoutNotifySink>	g_CheckoutNotifySink = NULL;

std::unique_ptr<BizClient>	g_pBizClient = NULL;

std::wstring	g_wstrServerIP = L"";

void on_initialize(const string_t& address)
{
	IdrInitInfo initInfo;
	initInfo.strIp = WChar2Ansi(g_wstrServerIP.c_str());
	initInfo.nPort = 47361;
	bool bInitSuccess = IdrInit(initInfo);

	if (g_bSubscribeCheck)
	{
		//subscribe check
		wstring wstrHttp = L"http://";
		wstring wstrBpmSocket = wstrHttp + g_wstrServerIP + L":997";
		wstring wstrModel = L"container";
		CString csCondition;
		csCondition.Format(L"<state stage='%s' substate='ready'/>", g_wsCheckStatus.c_str());
		wstring wstrCondition = csCondition.GetString();//L"<state stage='check' substate='ready'/>";
		wstring wstrPropValue = L"";

		if (true)
		{
			if (g_pBizClient == NULL)
			{
				g_pBizClient = std::unique_ptr<BizClient>(new BizClient(WChar2Ansi(wstrBpmSocket.c_str()), L"RestWebServer"));
			}

			if (g_CheckNotifySink == NULL)
			{
				g_CheckNotifySink = std::shared_ptr<CServerCheckNotifySink>(new CServerCheckNotifySink());
			}

			bool bSubscribeRet = g_pBizClient->subscribe(wstrModel, L"", wstrCondition, wstrPropValue, g_CheckNotifySink.get());

			if (bSubscribeRet)
			{
				//listen thread
				string_t addrCheck = address;
				addrCheck.append(U("/check"));
				uri_builder uri(addrCheck);

				auto addr = uri.to_uri().to_string();
				g_httpCheckHandler = std::unique_ptr<CCheckHandler>(new CCheckHandler(addr));
				g_httpCheckHandler->open().wait();

				CString strFormat;
				strFormat.Format(L"Listening for check requests at: %s.\n", addr.c_str());
				logRecord.WriteLog(strFormat.GetString());
			}
			else
			{
				CString strFormat;
				strFormat.Format(L"CHECK BizClient Subscribe Failed: Socket Ip:%s, Model:%s, Condition:%s PropValue: %s .\n", wstrBpmSocket, wstrModel, wstrCondition, wstrPropValue);
				logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
			}
		}
		else
		{
			CString strFormat;
			strFormat.Format(L"CHECK CConfigManager::GetConfigInfo Failed: Socket Ip:%s, Model:%s, Condition:%s PropValue: %s .\n", wstrBpmSocket, wstrModel, wstrCondition, wstrPropValue);
			logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
		}
	}

	if (g_bSubscribeRecheck)
	{
		wstring wstrHttp = L"http://";
		wstring wstrBpmSocket = wstrHttp + g_wstrServerIP + L":997";
		wstring wstrModel = L"container";
		CString csCondition;
		csCondition.Format(L"<state stage='%s' substate='ready'/>", g_wsRecheckStatus.c_str());
		wstring wstrCondition = csCondition.GetString();// L"<state stage='recheck' substate='ready'/>";
		wstring wstrPropValue = L"";

		if (true)
		{
			if (g_pBizClient == NULL)
			{
				g_pBizClient = std::unique_ptr<BizClient>(new BizClient(WChar2Ansi(wstrBpmSocket.c_str()), L"RestWebServer"));
			}

			if (g_RecheckNotifySink == NULL)
			{
				g_RecheckNotifySink = std::shared_ptr<CServerRecheckNotifySink>(new CServerRecheckNotifySink());
			}

			bool bSubscribeRet = g_pBizClient->subscribe(wstrModel, L"", wstrCondition, wstrPropValue, g_RecheckNotifySink.get());

			if (bSubscribeRet)
			{
				//listen recheck thread
				string_t addrReCheck = address;
				addrReCheck.append(U("/recheck"));
				uri_builder uri(addrReCheck);

				auto addr = uri.to_uri().to_string();
				g_httpReCheckHandler = std::unique_ptr<CReCheckHandler>(new CReCheckHandler(addr));
				g_httpReCheckHandler->open().wait();

				CString strFormat;
				strFormat.Format(L"Listening for recheck requests at: %s.\n", addr.c_str());
				logRecord.WriteLog(strFormat.GetString());
			}
			else
			{
				CString strFormat;
				strFormat.Format(L"RECHECK BizClient Subscribe Failed: Socket Ip:%s, Model:%s, Condition:%s PropValue: %s .\n", wstrBpmSocket, wstrModel, wstrCondition, wstrPropValue);
				logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
			}
		}
		else
		{
			CString strFormat;
			strFormat.Format(L"RECHECK CConfigManager::GetConfigInfo Failed: Socket Ip:%s, Model:%s, Condition:%s PropValue: %s .\n", wstrBpmSocket, wstrModel, wstrCondition, wstrPropValue);
			logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
		}
	}

	if (g_bSubscribeCheckout)
	{
		wstring wstrHttp = L"http://";
		wstring wstrBpmSocket = wstrHttp + g_wstrServerIP + L":997";
		wstring wstrModel = L"container";
		CString csCondition;
		csCondition.Format(L"<state stage='%s' substate='ready'/>", g_wsCheckoutStatus.c_str());
		wstring wstrCondition = csCondition.GetString();// L"<state stage='checkOut' substate='ready'/>";
		wstring wstrPropValue = L"";
		if (true)
		{
			if (g_pBizClient == NULL)
			{
				g_pBizClient = std::unique_ptr<BizClient>(new BizClient(WChar2Ansi(wstrBpmSocket.c_str()), L"RestWebServer"));
			}

			if (g_CheckoutNotifySink == NULL)
			{
				g_CheckoutNotifySink = std::shared_ptr<CServerCheckoutNotifySink>(new CServerCheckoutNotifySink());
			}

			bool bSubscribeRet = g_pBizClient->subscribe(wstrModel, L"", wstrCondition, wstrPropValue, g_CheckoutNotifySink.get());

			if (bSubscribeRet)
			{
				//listen checkout thread
				string_t addrCheckout = address;
				addrCheckout.append(U("/checkout"));
				uri_builder uri(addrCheckout);

				auto addr = uri.to_uri().to_string();
				g_httpCheckoutHandler = std::unique_ptr<CCheckoutHandler>(new CCheckoutHandler(addr));
				g_httpCheckoutHandler->open().wait();

				CString strFormat;
				strFormat.Format(L"Listening for checkout requests at: %s.\n", addr.c_str());
				logRecord.WriteLog(strFormat.GetString());
			}
			else
			{
				CString strFormat;
				strFormat.Format(L"CHECKOUT BizClient Subscribe Failed: Socket Ip:%s, Model:%s, Condition:%s PropValue: %s .\n", wstrBpmSocket, wstrModel, wstrCondition, wstrPropValue);
				logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
			}
		}
		else
		{
			CString strFormat;
			strFormat.Format(L"CHECKOUT CConfigManager::GetConfigInfo Failed: Socket Ip:%s, Model:%s, Condition:%s PropValue: %s .\n", wstrBpmSocket, wstrModel, wstrCondition, wstrPropValue);
			logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
		}
	}

	return;
}

void on_shutdown()
{
	if (g_bSubscribeCheck)
	{
		g_httpCheckHandler->close().wait();
	}

	if (g_bSubscribeRecheck)
	{
		g_httpReCheckHandler->close().wait();
	}

	if (g_bSubscribeCheckout)
	{
		g_httpCheckoutHandler->close().wait();
	}

	return;
}

BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	// Open the Service Control Manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
		NULL, // ServicesActive database
		SC_MANAGER_ALL_ACCESS); // full access
	if (hSCM)
	{
		// Try to open the service
		SC_HANDLE hService = ::OpenService(hSCM, ServiceName, SERVICE_QUERY_CONFIG);
		if (hService)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}

		::CloseServiceHandle(hSCM);
	}

	return bResult;
}

BOOL Install()
{
	// Open the Service Control Manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
		NULL, // ServicesActive database
		SC_MANAGER_ALL_ACCESS); // full access
	if (!hSCM)
	{
		//OpenSCManager failed, error code = %d
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[_MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));

	// Create the service
	SC_HANDLE hService = ::CreateService(hSCM,
		ServiceName,
		ServiceName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,        // start condition
		SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, NULL, NULL, NULL);
	if (!hService)
	{
		//Failed to create service
		int nError = GetLastError();

		::CloseServiceHandle(hSCM);
		return FALSE;
	}

	//Service is installed successfully.
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

BOOL Uninstall()
{
	// Open the Service Control Manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
		NULL, // ServicesActive database
		SC_MANAGER_ALL_ACCESS); // full access
	if (!hSCM)
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	SC_HANDLE hService = ::OpenService(hSCM, ServiceName, DELETE);

	if (hService)
	{
		if (::DeleteService(hService))
		{
			bResult = TRUE;
		}

		::CloseServiceHandle(hService);
	}

	::CloseServiceHandle(hSCM);

	return bResult;
}

int wmain(int argc, wchar_t *argv[])
{
	int nRetCode = 0;
	logRecord.OpenLogFile();

	if (argc > 1)
	{
		wstring wstrParameter = argv[1];
		//logRecord.WriteLog(wstrParameter.c_str());
		if (wcscmp(wstrParameter.c_str(), L"-install") == 0)
		{
			if (IsInstalled())
			{
				CString strFormat;
				strFormat.Format(L"%s is already installed\n", ServiceName);
				logRecord.WriteLog(strFormat.GetString());
			}
			else
			{
				if (Install())
				{
					logRecord.WriteLog(L"Service Installed Successfully!");
				}
				else
				{
					CString strFormat;
					strFormat.Format(L"%s failed to install. ErrorCode %d\n", ServiceName, GetLastError());
					logRecord.WriteLog(strFormat.GetString());
				}
			}
		}
		else if (wcscmp(wstrParameter.c_str(), L"-remove") == 0)
		{
			if (!IsInstalled())
			{
				CString strFormat;
				strFormat.Format(L"%s is not installed\n", ServiceName);
				logRecord.WriteLog(strFormat.GetString());
			}
			else
			{
				if (Uninstall())
				{
					logRecord.WriteLog(L"Servic Removed Successfully!");
				}
				else
				{
					CString strFormatError;
					strFormatError.Format(L"Could not remove Service %s. ErrorCode: %d\n", ServiceName, GetLastError());
					wprintf(L"Could not remove %s. Error %d\n", ServiceName, GetLastError());
				}
			}

		}
		else if (wcscmp(wstrParameter.c_str(), L"-debug") == 0)
		{
			logRecord.WriteLog(L"Debug Mode start the application");
			::SetConsoleCtrlHandler(ControlHandler, TRUE);

			DebugServiceStart();
		}
	}
	else
	{
		SERVICE_TABLE_ENTRY ServiceTable[] =
		{
			{ ServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ NULL, NULL }
		};

		if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
		{
			logRecord.WriteLog(L"Failed to Call StartServiceCtrlDispatcher()");
			BOOL bSuccess = StartServiceCtrlDispatcher(ServiceTable);
			int nError = GetLastError();
			return nError;
		}
	}

	return nRetCode;
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	DWORD Status = E_FAIL;

	logRecord.WriteLog(L"Enter ServiceMain funciton");
	// Register our service control handler with the SCM
	g_StatusHandle = RegisterServiceCtrlHandler(ServiceName, ServiceCtrlHandler);

	if (g_StatusHandle == NULL)
	{
		logRecord.WriteLog(L"RegisterServiceCtrlHandler failed");
		goto EXIT;
	}

	logRecord.WriteLog(L"RegisterServiceCtrlHandler successful");
	// Tell the service controller we are starting
	ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		logRecord.WriteLog(_T("ServiceMain: SetServiceStatus returned error"));
	}

	// Create a service stop event to wait on later
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_ServiceStopEvent == NULL)
	{
		// Error creating event
		// Tell service controller we are stopped and exit
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = GetLastError();
		g_ServiceStatus.dwCheckPoint = 1;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			logRecord.WriteLog(_T("ServiceMain: SetServiceStatus returned error"));
		}
		goto EXIT;
	}

	// Tell the service controller we are started
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	logRecord.WriteLog(L"SetServiceStatus Running");
	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		logRecord.WriteLog(_T("ServiceMain: SetServiceStatus returned error"));
	}

	// Start a thread that will perform the main task of the service
	HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

	// Wait until our worker thread exits signaling that the service needs to stop
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(g_ServiceStopEvent);

	// Tell the service controller we are stopped
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 3;

	logRecord.WriteLog(L"SetServiceStatus Stoped");
	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		logRecord.WriteLog(_T("ServiceMain: SetServiceStatus returned error"));
	}

EXIT:
	logRecord.WriteLog(L"Leave ServiceMain funciton");
	return;
}

BOOL WINAPI ControlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_BREAK_EVENT:
	case CTRL_C_EVENT:
	{
		logRecord.WriteLog(L"Stop Service in Control_Hander!");
		SetEvent(g_ServiceStopEvent);
		return TRUE;
	}
	}

	return FALSE;
}

VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode)
{
	switch (CtrlCode)
	{
	case SERVICE_CONTROL_STOP:

		if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
		{
			break;
		}

		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 4;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			logRecord.WriteLog(_T("ServiceCtrlHandler: SetServiceStatus returned error"));
		}

		// This will signal the worker thread to start shutting down
		SetEvent(g_ServiceStopEvent);

		logRecord.WriteLog(L"The end user Stoped current service manually.");

		break;

	default:
		break;
	}
}

void DebugServiceStart()
{
	logRecord.WriteLog(L"DebugServiceStart: Start Service in Debug Mode.");

	// Create a service stop event to wait on later
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_ServiceStopEvent == NULL)
	{
		logRecord.WriteLog(L"DebugServiceStart: Failed to create Event Object.");
		goto DEBUGEXIT;
	}

	// Start a thread that will perform the main task of the service
	HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

	// Wait until our worker thread exits signaling that the service needs to stop
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(g_ServiceStopEvent);

DEBUGEXIT:
	logRecord.WriteLog(L"DebugServiceStart: Leave Service in Debug Mode.");

}


DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
	logRecord.WriteLog(L"Enter ServiceWorkerThread");

	bool bSuscess = ParserConfigFile();
	logRecord.Flush();
	if (!bSuscess)
	{
		return ERROR_SUCCESS;
	}

	utility::string_t port = U("34568");

	utility::string_t address = U("http://*:");
	address.append(port);

	on_initialize(address);

	//  Periodically check if the service has been requested to stop
	while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		g_nMonth = sysTime.wMonth;
		g_nYear = sysTime.wYear;
		int nDay = sysTime.wDay;

		if (nDay != g_nDay)
		{
			g_nDay = nDay;
			CString strFormatTime;
			strFormatTime.Format(L"%d-%d-%d", g_nYear, g_nMonth, g_nDay);
			logRecord.OpenNewLogFile(strFormatTime.GetString()); //新的一天，更新日志文件
		}

		//休眠10s
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	logRecord.WriteLog(L"Leave ServiceWorkerThread");

	on_shutdown();

	return ERROR_SUCCESS;
}
