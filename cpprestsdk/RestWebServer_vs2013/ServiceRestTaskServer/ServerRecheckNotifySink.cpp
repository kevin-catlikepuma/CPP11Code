#include "stdafx.h"
#include "ServerRecheckNotifySink.h"
#include "ServiceRestTaskServer.h"
#include "ServerHelper.h"


CServerRecheckNotifySink::CServerRecheckNotifySink()
{
	g_wstrReCheckTimeStamp = GetCurTimeStampString();
	std::thread recheck_th(recheck_thread);
	recheck_th.detach(); //the thread will run on background.
}

void CServerRecheckNotifySink::onAdd(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), addTask);
	g_recheckQueue.Put(ts);
	g_wstrReCheckTimeStamp = GetCurTimeStampString();
}

void CServerRecheckNotifySink::onRemove(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), delTask);
	g_recheckQueue.Put(ts);

	g_wstrReCheckTimeStamp = GetCurTimeStampString();
}

void recheck_thread()
{
	SubscribeTask ts;
	while (true)
	{
		ts.wstrTaskId.clear();
		ts.mark = errOP;
		g_recheckQueue.Take(ts);
		cout << ts.wstrTaskId.c_str() << endl;
		switch (ts.mark)
		{
		case addTask:
			{
				CString strFormat;
				strFormat.Format(L"Subscribe TaskId:%s; Status is recheck.ready.\n", ts.wstrTaskId.c_str());
				logRecord.WriteLog(strFormat.GetString());
				AddRecheckTasktoVector(ts.wstrTaskId.c_str());
				break;
			}
		case delTask:
			{
				CString strFormat;
				strFormat.Format(L"Remove TaskId:%s from recheck memory .\n", ts.wstrTaskId.c_str());
				logRecord.WriteLog(strFormat.GetString());
				RemoveRecheckTaskFromVector(ts.wstrTaskId.c_str());
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

void AddRecheckTasktoVector(const wchar_t* pwstrTaskId)
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

	g_recheckLock.lock();
	g_vecRecheckTask.emplace_back(tkInfo);
	g_recheckLock.unlock();
}

void RemoveRecheckTaskFromVector(const wchar_t* pwstrTaskId)
{
	std::vector<TaskInfo>::iterator itBegin = g_vecRecheckTask.begin();
	std::vector<TaskInfo>::iterator itEnd = g_vecRecheckTask.end();
	for (; itBegin != itEnd; itBegin++)
	{
		if (0 == wcscmp(pwstrTaskId, (*itBegin).wstrTaskId.c_str()))
		{
			break;
		}
	}

	if (itBegin != itEnd)
	{
		g_recheckLock.lock();
		g_vecRecheckTask.erase(itBegin);
		g_recheckLock.unlock();
	}
}