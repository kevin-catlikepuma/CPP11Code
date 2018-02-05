#pragma once
#include "Defs.h"
#include "SimpleSyncQueue.hpp"
#include "LogFileUtil.h"

extern CLogRecord logRecord;
extern SimpleSyncQueue<SubscribeTask> g_checkQueue;
extern SimpleSyncQueue<SubscribeTask> g_recheckQueue;
extern SimpleSyncQueue<SubscribeTask> g_checkoutQueue;
extern std::vector<TaskInfo>		g_vecCheckTask;
extern std::vector<TaskInfo>		g_vecRecheckTask;
extern std::vector<TaskInfo>		g_vecCheckoutTask;

BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();

void DebugServiceStart();