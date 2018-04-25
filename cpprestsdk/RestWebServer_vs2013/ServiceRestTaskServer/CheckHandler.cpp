#include "stdafx.h"
#include "CheckHandler.h"
#include "ServerHelper.h"
#include "ServiceRestTaskServer.h"



//check role
CCheckHandler::CCheckHandler(utility::string_t url):CHandler(url)
{
	m_listener.support(methods::GET, std::bind(&CCheckHandler::handle_get, this, std::placeholders::_1));
}

//// Get Request 
void CCheckHandler::handle_get(http_request message)
{
	/*ucout << message.to_string() << endl;*/
	//auto requst_uri = message.request_uri();
	//auto relate_uri = message.relative_uri();
	//auto paths = http::uri::split_path(http::uri::decode(message.request_uri().path()));
	//auto headers = message.headers();
	//auto path = message.relative_uri().path();

	/*auto remote = message.get_remote_address();
	ucout << L"Received check request. Remote IP: " << remote << endl;*/
	
	if (0 == wcscmp(g_wstrCheckTimeStamp.c_str(), m_wstrLastTimeStamp.c_str()))
	{
		ucout << L"check Cache Data: " << g_wstrCheckTimeStamp.c_str() << endl;
		message.reply(status_codes::OK, m_wstrCacheJson);
	}
	else
	{
		m_wstrLastTimeStamp = g_wstrCheckTimeStamp;
		m_wstrCacheJson = AsJSON().serialize();
		ucout << L"check New Data: " << g_wstrCheckTimeStamp.c_str() << endl;
		ucout << m_wstrCacheJson.c_str() << endl;
		message.reply(status_codes::OK, m_wstrCacheJson);
	}

	return;
}

web::json::value CCheckHandler::AsJSON() const
{
	web::json::value res = web::json::value::object();
	wstring wstrRole = L"check";
	res[U("role")] = web::json::value::string(wstrRole);
	res[U("timestamp")] = web::json::value::string(m_wstrLastTimeStamp);

	g_checkLock.lock();

	web::json::value tasks = web::json::value::array(g_vecCheckTask.size());
	size_t idx = 0;
	for (auto iter = g_vecCheckTask.begin(); iter != g_vecCheckTask.end(); ++iter)
	{
		tasks[idx++] = iter->AsJSON();
	}

	g_checkLock.unlock();

	res[U("tasksFromBPM")] = tasks;

	return res;
}

//recheck role
CReCheckHandler::CReCheckHandler(utility::string_t url) :CHandler(url)
{
	m_listener.support(methods::GET, std::bind(&CReCheckHandler::handle_get, this, std::placeholders::_1));
}

////
//// Get Request 
void CReCheckHandler::handle_get(http_request message)
{
	//auto remote = message.get_remote_address();
	//ucout << L"Received recheck request. Remote IP: " << remote << endl;

	if (0 == wcscmp(g_wstrReCheckTimeStamp.c_str(), m_wstrLastTimeStamp.c_str()))
	{
		ucout << L"recheck Cache Data: " << g_wstrReCheckTimeStamp.c_str() << endl;
		message.reply(status_codes::OK, m_wstrCacheJson);
	}
	else
	{
		m_wstrLastTimeStamp = g_wstrReCheckTimeStamp;
		m_wstrCacheJson = AsJSON().serialize();
		ucout << L"recheck New Data: " << g_wstrReCheckTimeStamp.c_str() << endl;
		ucout << m_wstrCacheJson.c_str() << endl;
		message.reply(status_codes::OK, m_wstrCacheJson);
	}

	return;
}

web::json::value CReCheckHandler::AsJSON() const
{
	web::json::value res = web::json::value::object();
	wstring wstrRole = L"recheck";
	res[U("role")] = web::json::value::string(wstrRole);
	res[U("timestamp")] = web::json::value::string(m_wstrLastTimeStamp);

	g_recheckLock.lock();

	web::json::value tasks = web::json::value::array(g_vecRecheckTask.size());
	size_t idx = 0;
	for (auto iter = g_vecRecheckTask.begin(); iter != g_vecRecheckTask.end(); ++iter)
	{
		tasks[idx++] = iter->AsJSON();
	}

	g_recheckLock.unlock();

	res[U("tasksFromBPM")] = tasks;

	return res;
}

//checkout role
CCheckoutHandler::CCheckoutHandler(utility::string_t url) :CHandler(url)
{
	m_listener.support(methods::GET, std::bind(&CCheckoutHandler::handle_get, this, std::placeholders::_1));
}

////
//// Get Request 
void CCheckoutHandler::handle_get(http_request message)
{
	//auto remote = message.get_remote_address();
	//ucout << L"Received checkout request. Remote IP: " << remote << endl;

	if (0 == wcscmp(g_wstrCheckoutTimeStamp.c_str(), m_wstrLastTimeStamp.c_str()))
	{
		ucout << L"checkout Cache Data: " << g_wstrCheckoutTimeStamp.c_str() << endl;
		message.reply(status_codes::OK, m_wstrCacheJson);
	}
	else
	{
		m_wstrLastTimeStamp = g_wstrCheckoutTimeStamp;
		m_wstrCacheJson = AsJSON().serialize();
		ucout << L"checkout New Data: " << g_wstrCheckoutTimeStamp.c_str() << endl;
		ucout << m_wstrCacheJson.c_str() << endl;
		message.reply(status_codes::OK, m_wstrCacheJson);
	}

	return;
}

web::json::value CCheckoutHandler::AsJSON() const
{
	web::json::value res = web::json::value::object();
	wstring wstrRole = L"checkout";
	res[U("role")] = web::json::value::string(wstrRole);
	res[U("timestamp")] = web::json::value::string(m_wstrLastTimeStamp);

	g_checkoutLock.lock();

	web::json::value tasks = web::json::value::array(g_vecCheckoutTask.size());
	size_t idx = 0;
	for (auto iter = g_vecCheckoutTask.begin(); iter != g_vecCheckoutTask.end(); ++iter)
	{
		tasks[idx++] = iter->AsJSON();
	}

	g_checkoutLock.unlock();

	res[U("tasksFromBPM")] = tasks;

	return res;
}