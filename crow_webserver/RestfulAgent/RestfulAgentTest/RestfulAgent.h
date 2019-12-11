#pragma once

#ifdef RESTFULAGENT_EXPORTS
#define RESTFULAGENT_API __declspec(dllexport)
#else
#define RESTFULAGENT_API __declspec(dllimport)
#endif

// �����Ǵ� RestfulAgent.dll ������
class RESTFULAGENT_API CRestfulAgent {
public:
	CRestfulAgent(void);

	/*
	������	pwstrUrl ���ʵ�restful api url����http://ip:port/path
	����ֵ���ӿڽ�������õ�JSON��ʽ�ַ���
	*/
	std::wstring  http_get(const wchar_t *pwstrUrl);

	/*
	������	pwstrUrl ���ʵ�restful api url����http://ip:port/path
			pwstrJson ��json��ʽ��ʾ�Ĳ����ַ���
	����ֵ���ӿڽ�������õ�JSON��ʽ�ַ���
	*/
	std::wstring http_post(const wchar_t *pwstrUrl, const wchar_t *pwstrJson);

	/*
	������	pwstrUrl ���ʵ�restful api url����http://ip:port/path
			pwstrJson ��json��ʽ��ʾ�Ĳ����ַ���
	����ֵ���ӿڽ�������õ�JSON��ʽ�ַ���
	*/
	std::wstring http_put(const wchar_t *pwstrUrl, const wchar_t *pwstrJson);

	/*
	����ֵ��http���ʷ���״̬��
	*/
	int get_response_code();

private:
	int m_nRetCode;
};


//extern RESTFULAGENT_API int nRestfulAgent;
//
//RESTFULAGENT_API int fnRestfulAgent(void);
