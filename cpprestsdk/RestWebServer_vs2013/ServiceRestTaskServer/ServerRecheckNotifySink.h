#pragma once
#include "Include\INotifySink.h"

class CServerRecheckNotifySink : public INotifySink
{
public:
	CServerRecheckNotifySink();
	virtual ~CServerRecheckNotifySink() {};

	//INotifySink
	virtual void onAdd(BizObjectProxy object);
	virtual void onRemove(BizObjectProxy object);
};

void recheck_thread();
void AddRecheckTasktoVector(const wchar_t* pwstrTaskId);
void RemoveRecheckTaskFromVector(const wchar_t* pwstrTaskId);