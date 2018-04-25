#include "stdafx.h"
#include "ServerCheckNotifySink.h"
#include "ServiceRestTaskServer.h"
#include "ServerHelper.h"
#include "Include\IdrAgent.h"


CServerCheckNotifySink::CServerCheckNotifySink()
{
	g_wstrCheckTimeStamp = GetCurTimeStampString();
	std::thread check_th(check_thread);
	check_th.detach(); //the thread will run on background.
}

void CServerCheckNotifySink::onAdd(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), addTask);
	g_checkQueue.Put(ts);
	g_wstrCheckTimeStamp = GetCurTimeStampString();
}

void CServerCheckNotifySink::onRemove(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), delTask);
	g_checkQueue.Put(ts);

	g_wstrCheckTimeStamp = GetCurTimeStampString();
}

void check_thread()
{
	SubscribeTask ts;
	while (true)
	{
		ts.wstrTaskId.clear();
		ts.mark = errOP;
		g_checkQueue.Take(ts);
		switch (ts.mark)
		{
		case addTask:
			{
				CString strFormat;
				strFormat.Format(L"Subscribe TaskId:%s; Status is check.ready.\n", ts.wstrTaskId.c_str());
				logRecord.WriteLog(strFormat.GetString());
				AddCheckTasktoVector(ts.wstrTaskId.c_str());
				break;
			}
		case delTask:
			{
				CString strFormat;
				strFormat.Format(L"Remove TaskId:%s from check memory .\n", ts.wstrTaskId.c_str());
				logRecord.WriteLog(strFormat.GetString());
				RemoveCheckTaskFromVector(ts.wstrTaskId.c_str());
				break;
			}
		default:
			{
				CString strFormat;
				strFormat.Format(L"Error Task:%s has wrong mark.\n", ts.wstrTaskId.c_str());
				logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
				break;
			}
		}
	}
}

void AddCheckTasktoVector(const wchar_t* pwstrTaskId)
{
	wstring	wstrUniXml = L"";
	bool bReturn = GetUniXML(pwstrTaskId, wstrUniXml);
	if (!bReturn || wstrUniXml.empty())
	{
		CString strFormat;
		strFormat.Format(L"Get UniXml Failed. TaskId is: %s.\n", pwstrTaskId);
		logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
		return;
	}

	TaskInfo tkInfo;
	tkInfo.wstrTaskId = pwstrTaskId;
	bool bParserRet = PaserUniXML(wstrUniXml.c_str(), tkInfo);
	if (!bParserRet)
	{
		CString strFormat;
		strFormat.Format(L"Parser UniXml Failed. TaskId is: %s.\n", pwstrTaskId);
		logRecord.WriteLog(strFormat.GetString(), ELL_ERROR);
		return;
	}

	g_checkLock.lock();
	g_vecCheckTask.emplace_back(tkInfo);
	g_checkLock.unlock();
}

void RemoveCheckTaskFromVector(const wchar_t* pwstrTaskId)
{
	std::vector<TaskInfo>::iterator itBegin = g_vecCheckTask.begin();
	std::vector<TaskInfo>::iterator itEnd = g_vecCheckTask.end();
	for (; itBegin != itEnd; itBegin++)
	{
		if (0 == wcscmp(pwstrTaskId, (*itBegin).wstrTaskId.c_str()))
		{
			break;
		}
	}

	if (itBegin != itEnd)
	{
		g_checkLock.lock();
		g_vecCheckTask.erase(itBegin);
		g_checkLock.unlock();
	}
}
