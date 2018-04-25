#include "stdafx.h"
#include "handler.h"

CHandler::CHandler()
{
	m_wstrCacheJson = L"";
	m_wstrLastTimeStamp = L"";
}

CHandler::CHandler(utility::string_t url) : m_listener(url)
{
//	m_listener.support(methods::GET, std::bind(&CHandler::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&CHandler::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&CHandler::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&CHandler::handle_delete, this, std::placeholders::_1));
}

CHandler::~CHandler()
{
	//dtor
}

void CHandler::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
	}
	catch (...)
	{
		// Ignore the error, Log it if a logger is available
	}
}

//
// A POST request
//
void CHandler::handle_post(http_request message)
{
	ucout << message.to_string() << endl;

	message.reply(status_codes::OK, message.to_string());
	return;
};

//
// A DELETE request
//
void CHandler::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;

	string rep = "WRITE YOUR OWN DELETE OPERATION";
	message.reply(status_codes::OK, rep);
	return;
};


//
// A PUT request 
//
void CHandler::handle_put(http_request message)
{
	ucout << message.to_string() << endl;
	string rep = "WRITE YOUR OWN PUT OPERATION";
	message.reply(status_codes::OK, rep);
	return;
};
