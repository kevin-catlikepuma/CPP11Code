#pragma once
#include "Include\INotifySink.h"

class CServerCheckoutNotifySink : public INotifySink
{
public:
	CServerCheckoutNotifySink();
	virtual ~CServerCheckoutNotifySink() {};

	//INotifySink
	virtual void onAdd(BizObjectProxy object);
	virtual void onRemove(BizObjectProxy object);
};

void checkout_thread();
void AddCheckoutTasktoVector(const wchar_t* pwstrTaskId);
void RemoveCheckoutTaskFromVector(const wchar_t* pwstrTaskId);