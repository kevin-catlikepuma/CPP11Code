#include "stdafx.h"
#include "ServerCheckoutNotifySink.h"
#include "ServiceRestTaskServer.h"
#include "ServerHelper.h"

CServerCheckoutNotifySink::CServerCheckoutNotifySink()
{
	g_wstrCheckoutTimeStamp = GetCurTimeStampString();
	std::thread checkout_th(checkout_thread);
	checkout_th.detach(); //the thread will run on background.
}

void CServerCheckoutNotifySink::onAdd(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), addTask);
	g_checkoutQueue.Put(ts);
	g_wstrCheckoutTimeStamp = GetCurTimeStampString();
}

void CServerCheckoutNotifySink::onRemove(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), delTask);
	g_checkoutQueue.Put(ts);

	g_wstrCheckoutTimeStamp = GetCurTimeStampString();
}

void checkout_thread()
{
	SubscribeTask ts;
	while (true)
	{
		ts.wstrTaskId.clear();
		ts.mark = errOP;
		g_checkoutQueue.Take(ts);//block this thread when queue is empty
		switch (ts.mark)
		{
		case addTask:
			{
				CString strFormat;
				strFormat.Format(L"Subscribe TaskId:%s; Status is checkout.ready.\n", ts.wstrTaskId.c_str());
				logRecord.WriteLog(strFormat.GetString());
				AddCheckoutTasktoVector(ts.wstrTaskId.c_str());
				break;
			}
		case delTask:
			{
				CString strFormat;
				strFormat.Format(L"Remove TaskId:%s from checkout memory .\n", ts.wstrTaskId.c_str());
				logRecord.WriteLog(strFormat.GetString());
				RemoveCheckoutTaskFromVector(ts.wstrTaskId.c_str());
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

void AddCheckoutTasktoVector(const wchar_t* pwstrTaskId)
{
	std::wstring wstrUniXml;
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

	g_checkoutLock.lock();
	g_vecCheckoutTask.emplace_back(tkInfo);
	g_checkoutLock.unlock();
}

void RemoveCheckoutTaskFromVector(const wchar_t* pwstrTaskId)
{
	std::vector<TaskInfo>::iterator itBegin = g_vecCheckoutTask.begin();
	std::vector<TaskInfo>::iterator itEnd = g_vecCheckoutTask.end();
	for (; itBegin != itEnd; itBegin++)
	{
		if (0 == wcscmp(pwstrTaskId, (*itBegin).wstrTaskId.c_str()))
		{
			break;
		}
	}

	if (itBegin != itEnd)
	{
		g_checkoutLock.lock();
		g_vecCheckoutTask.erase(itBegin);
		g_checkoutLock.unlock();
	}
}