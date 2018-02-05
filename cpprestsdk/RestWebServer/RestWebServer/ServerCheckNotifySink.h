#pragma once
#include "Include\INotifySink.h"

class CServerCheckNotifySink : public INotifySink
{
public:
	CServerCheckNotifySink();
	virtual ~CServerCheckNotifySink() {};

	//INotifySink
	virtual void onAdd(BizObjectProxy object);
	virtual void onRemove(BizObjectProxy object);
};

void check_thread();