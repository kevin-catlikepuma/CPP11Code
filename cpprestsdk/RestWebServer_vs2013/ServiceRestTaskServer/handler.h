#pragma once
#include "stdafx.h"
#include <iostream>
#include<cpprest\http_client.h>
#include<cpprest\http_listener.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class CHandler
{
public:
	CHandler();
	CHandler(utility::string_t url);
	virtual ~CHandler();

	pplx::task<void>open() { return m_listener.open(); }
	pplx::task<void>close() { return m_listener.close(); }

	virtual web::json::value AsJSON() const = 0;

protected:
	virtual void handle_get(http_request message) = 0;
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_error(pplx::task<void>& t);

	http_listener m_listener;

	utility::string_t	m_wstrCacheJson;
	utility::string_t	m_wstrLastTimeStamp;
};


