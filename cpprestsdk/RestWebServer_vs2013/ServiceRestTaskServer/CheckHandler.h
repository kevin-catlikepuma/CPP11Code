#pragma once
#include "handler.h"


class CCheckHandler : public CHandler
{
public:
	CCheckHandler() {};
	CCheckHandler(utility::string_t url);
	virtual ~CCheckHandler() {};

	virtual web::json::value AsJSON() const;

	virtual void handle_get(http_request message);
};


class CReCheckHandler : public CHandler
{
public:
	CReCheckHandler() {};
	CReCheckHandler(utility::string_t url);
	virtual ~CReCheckHandler() {};

	virtual web::json::value AsJSON() const;

	virtual void handle_get(http_request message);
};

class CCheckoutHandler : public CHandler
{
public:
	CCheckoutHandler() {};
	CCheckoutHandler(utility::string_t url);
	virtual ~CCheckoutHandler() {};

	virtual web::json::value AsJSON() const;

	virtual void handle_get(http_request message);
};