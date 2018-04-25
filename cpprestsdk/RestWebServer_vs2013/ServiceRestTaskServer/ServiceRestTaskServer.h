#pragma once

#include "Defs.h"
#include "SimpleSyncQueue.hpp"
#include "LogFileUtil.h"

extern CLogRecord logRecord;

extern bool	g_bSubscribeCheck;  //从配置文件里面读取，是否订阅check角色任务
extern wstring g_wsCheckStatus;
extern bool	g_bSubscribeRecheck;
extern wstring g_wsRecheckStatus;
extern bool	g_bSubscribeCheckout;
extern wstring g_wsCheckoutStatus;

extern SimpleSyncQueue<SubscribeTask> g_checkQueue;
extern SimpleSyncQueue<SubscribeTask> g_recheckQueue;
extern SimpleSyncQueue<SubscribeTask> g_checkoutQueue;

extern std::vector<TaskItem>		g_vecTaskConfig;

extern std::mutex	g_checkLock;
extern std::vector<TaskInfo>		g_vecCheckTask;
extern std::wstring				g_wstrCheckTimeStamp;

extern std::mutex	g_recheckLock;
extern std::vector<TaskInfo>		g_vecRecheckTask;
extern std::wstring				g_wstrReCheckTimeStamp;

extern std::mutex	g_checkoutLock;
extern std::vector<TaskInfo>		g_vecCheckoutTask;
extern std::wstring				g_wstrCheckoutTimeStamp;

extern std::wstring	g_wstrServerIP;

BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();

void DebugServiceStart();