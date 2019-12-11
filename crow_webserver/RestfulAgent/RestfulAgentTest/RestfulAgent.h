#pragma once

#ifdef RESTFULAGENT_EXPORTS
#define RESTFULAGENT_API __declspec(dllexport)
#else
#define RESTFULAGENT_API __declspec(dllimport)
#endif

// 此类是从 RestfulAgent.dll 导出的
class RESTFULAGENT_API CRestfulAgent {
public:
	CRestfulAgent(void);

	/*
	参数：	pwstrUrl 访问的restful api url，如http://ip:port/path
	返回值：接口交互定义好的JSON格式字符串
	*/
	std::wstring  http_get(const wchar_t *pwstrUrl);

	/*
	参数：	pwstrUrl 访问的restful api url，如http://ip:port/path
			pwstrJson 以json格式表示的参数字符串
	返回值：接口交互定义好的JSON格式字符串
	*/
	std::wstring http_post(const wchar_t *pwstrUrl, const wchar_t *pwstrJson);

	/*
	参数：	pwstrUrl 访问的restful api url，如http://ip:port/path
			pwstrJson 以json格式表示的参数字符串
	返回值：接口交互定义好的JSON格式字符串
	*/
	std::wstring http_put(const wchar_t *pwstrUrl, const wchar_t *pwstrJson);

	/*
	返回值：http访问返回状态码
	*/
	int get_response_code();

private:
	int m_nRetCode;
};


//extern RESTFULAGENT_API int nRestfulAgent;
//
//RESTFULAGENT_API int fnRestfulAgent(void);
