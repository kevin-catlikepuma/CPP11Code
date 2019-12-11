#include "stdafx.h"
#include "curl\curl.h"
#include "RestfulAgent.h"
#include "json\json.h"

#pragma comment(lib, "libcurl.lib")

CRestfulAgent::CRestfulAgent()
{
	m_nRetCode = 0;
}

std::string WChar2Ansi(LPCWSTR pwszSrc)  
{  
    int nLen = WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, NULL, 0, NULL, NULL);  
    if (nLen <= 0)
	{
        return std::string("");
	}

    char* pszDst = new char[nLen];
    if (NULL == pszDst)
	{
        return std::string("");
	}

    WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);  
    pszDst[nLen -1] = 0; 

    std::string strTemp(pszDst);

    delete [] pszDst;  
    return strTemp;  
}

wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
{  
    int nSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pszSrc, nLen, 0, 0);  
    if(nSize <= 0)
	{
        return L"";  
	}

    WCHAR *pwszDst = new WCHAR[nSize+1];
    if( NULL == pwszDst)
	{
        return L"";
	}

    MultiByteToWideChar(CP_UTF8, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);  
    pwszDst[nSize] = 0;  
    if( pwszDst[0] == 0xFEFF) // skip Oxfeff  
        for(int i = 0; i < nSize; i ++)   
            pwszDst[i] = pwszDst[i+1];   

    wstring wcharString(pwszDst);  
    delete pwszDst;  
    return wcharString;
}

static size_t process_data(void *data, size_t size, size_t nmemb, string &content)
{
    size_t sizes = size * nmemb;
    string temp = string((char*)data, sizes);
    content += temp;
    return sizes;
}

std::wstring CRestfulAgent::http_get(const wchar_t *pwstrUrl)
{
	m_nRetCode = 0;
	string strUrl = WChar2Ansi(pwstrUrl);

	CURL *curl;
	CURLcode res;

	string content;
	content.clear();

	curl = curl_easy_init();
	if(curl != NULL)
	{
		struct curl_slist *http_header = NULL;
		http_header = curl_slist_append(http_header, "Accept: *");
		http_header = curl_slist_append(http_header, "Content-Type:application/json;charset=UTF-8");

		
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &process_data);

		curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER, http_header);
		curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT_MS, 500);

		res = curl_easy_perform(curl);

		if(res == CURLE_OK)
		{
			long retcode = 0;
			CURLcode code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
			if ((code == CURLE_OK))
			{
				m_nRetCode = retcode;
			}
			else
			{
				m_nRetCode = code;
			}
		}
		else
		{
			m_nRetCode = res;
		}

		curl_slist_free_all(http_header);
		curl_easy_cleanup(curl);
	}

	if(m_nRetCode != 200)
	{
		content.clear();
	}

	wstring wstrContent = Ansi2WChar(content.c_str(), content.length());
	return wstrContent.c_str();
}

std::wstring CRestfulAgent::http_post(const wchar_t *pwstrUrl, const wchar_t *pwstrJson)
{
	m_nRetCode = 0;
	string strUrl = WChar2Ansi(pwstrUrl);
	string strJson = WChar2Ansi(pwstrJson);

	CURL *curl;
	CURLcode res;
	string content;
	content.clear();

	curl = curl_easy_init();

	if(curl != NULL)
	{
		struct curl_slist *http_header = NULL;
		http_header = curl_slist_append(http_header, "Accept: *");
		http_header = curl_slist_append(http_header, "Content-Type:application/json;charset=UTF-8");

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &process_data);

		curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
		curl_easy_setopt(curl,CURLOPT_POST,1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strJson.c_str());
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER, http_header);
		curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT_MS, 500);
		res = curl_easy_perform(curl);

		if(res == CURLE_OK)
		{
			long retcode = 0;
			CURLcode code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
			if ((code == CURLE_OK))
			{
				m_nRetCode = retcode;
			}
			else
			{
				m_nRetCode = code;
			}
		}
		else
		{
			m_nRetCode = res;
		}

		curl_slist_free_all(http_header);
		curl_easy_cleanup(curl);
	}
	
	if(m_nRetCode != 200)
	{
		content.clear();
	}

	wstring wstrContent = Ansi2WChar(content.c_str(), content.length());
	return wstrContent.c_str();
}

std::wstring CRestfulAgent::http_put(const wchar_t *pwstrUrl, const wchar_t *pwstrJson)
{
	m_nRetCode = 0;
	string strUrl = WChar2Ansi(pwstrUrl);
	string strJson = WChar2Ansi(pwstrJson);

	CURL *curl;
	CURLcode res;
	string content;
	content.clear();

	curl = curl_easy_init();

	if(curl != NULL)
	{
		struct curl_slist *http_header = NULL;
		http_header = curl_slist_append(http_header, "Accept: *");
		http_header = curl_slist_append(http_header, "Content-Type:application/json;charset=UTF-8");

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &process_data);

		curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
		// curl_easy_setopt(curl, CURLOPT_PUT,1L); //dont use this statement
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strJson.c_str());
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER, http_header);
		curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT_MS,1000);
		res = curl_easy_perform(curl);

		if(res == CURLE_OK)
		{
			long retcode = 0;
			CURLcode code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
			if ((code == CURLE_OK))
			{
				m_nRetCode = retcode;
			}
			else
			{
				m_nRetCode = code;
			}
		}
		else
		{
			m_nRetCode = res;
		}

		curl_slist_free_all(http_header);
		curl_easy_cleanup(curl);
	}

	if(m_nRetCode != 200)
	{
		content.clear();
	}

	wstring wstrContent = Ansi2WChar(content.c_str(), content.length());
	return wstrContent.c_str();
}

int CRestfulAgent::get_response_code()
{
	return m_nRetCode;	
	std::string json_str = "{ \"encoding\": \"UTF-8\", \"tab-length\": [], \"plug-ins\": [\"python\", \"C++\", \"ruby\", \"perl\"], \"indent\": { \"length\": 3, \"use_space\": true}, \"objectlist\": [ {\"int\": 123}, {\"string\": \"kevin\"}, {\"bool\": false}]}";
	Json::Reader reader;
	Json::Value	root;

	if(reader.parse(json_str.c_str(), root))
	{
		std::string value1 = root["encoding"].asString();
		Json::Value inObj = root["indent"];
		if(inObj.type() == Json::objectValue)
		{
			int nValue = inObj["length"].asInt();
			bool bValue = inObj["use_space"].asBool();
		}

		bool bMember = root.isMember("tab"); //root对象里不存在一个key为"tab"的元素，此时返回false
		bool bNull = root["tab"].isNull();  //访问一个不存在的 key 时，会自动生成这样一个key-value默认为null的值对，此时返回true
		bMember = root.isMember("tab");  //此时再访问key为"tab"的元素是否存在，则返回true
		bNull = root["tab"].isNull();  //判断key为"tab"的元素值是否为null，仍然返回true

		if(root.isMember("tab-length")) // 判断是否存在key为tab-length的元素
		{
			if(root["tab-length"].isNull()) //判断key为tab-length的元素值是否为null，注意不是空
			{
				;
			}
			else
			{
				bool bEmpty = root["tab-length"].empty();  //不为空，注意不是null
				int nSize = root["tab-length"].size();
				int k = 1000;
			}
		}

		Json::Value arrayObj = root["plug-ins"];
		Json::ValueType type = arrayObj.type();
		if(type == Json::arrayValue)
		{
			for(int i = 0; i < static_cast<int>(arrayObj.size()); i++)
			{
				Json::Value obj = arrayObj[i];  // 取出列表中的对象
				if(obj.type() == Json::stringValue)  // 判断列表中的对象是否为Json::stringValue
				{
					std::string strValue = obj.asString();
					int k = 1000;
				}
			}
		}

		Json::Value listObj = root["objectlist"];
		if(listObj.type() == Json::arrayValue)
		{
			for(int i = 0; i < static_cast<int>(listObj.size()); i++)
			{
				Json::Value obj = listObj[i];
				if(obj.isMember("int"))  //判断json对象里是否含有key为"int"的元素
				{
					int nValue = obj["int"].asInt();
					int k = 1000;
				}
				
				if(obj.isMember("bool"))
				{
					bool bValue = obj["bool"].asBool();
					int k = 1000;
				}

				if(obj.isMember("string"))
				{
					std::string strValue = obj["string"].asString();
					int k = 1000;
				}
			}
		}


		//写入文件
		Json::FastWriter fwriter;
		Json::StyledWriter swriter;


		std::string str = fwriter.write(root);
	
		str = swriter.write(root);

	}
}