#include "stdafx.h"
#include <thread>
#include "ServerCheckNotifySink.h"
#include "RestWebServer.h"


CServerCheckNotifySink::CServerCheckNotifySink()
{
	std::thread check_th(check_thread);
	check_th.detach(); //the thread will run at background.
}

void CServerCheckNotifySink::onAdd(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), addTask);
	g_checkQueue.Put(ts);
}

void CServerCheckNotifySink::onRemove(BizObjectProxy object)
{
	SubscribeTask ts(object.id.c_str(), delTask);
	g_checkQueue.Put(ts);
}

void check_thread()
{
	SubscribeTask ts;
	while (true)
	{
		ts.wstrTaskId.clear();
		ts.mark = errOP;
		g_checkQueue.Take(ts);
		cout << ts.wstrTaskId.c_str() << endl;
		switch (ts.mark)
		{
		case addTask:
			//添加任务
			break;
		case delTask:
			//删除任务
			break;
		default:
			//输出错误任务信息，及mark
			break;
		}
	}
}