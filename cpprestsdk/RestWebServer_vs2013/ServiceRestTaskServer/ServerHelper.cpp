#include "stdafx.h"
#include "ServerHelper.h"
#include "ServiceRestTaskServer.h"
#include "Include\IdrAgent.h"

bool ParserConfigFile()
{
	logRecord.WriteLog(L"Get ConfigFile infomation SAVED in ConfigInfo.xml.");
	bool bReturn = false;

	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	std::wstring wstrFileName = szPath;
	size_t nPos = wstrFileName.rfind(L"\\");

	std::wstring wstrConfigPath = wstrFileName.substr(0, nPos);
	wstrConfigPath += L"\\ConfigInfo.xml";

	if (!IsFileExist(wstrConfigPath.c_str()))
	{
		CString strFormatError;
		strFormatError.Format(L"%s File NOT exist, ConfigInfo.xml must exist in applicaiotn root directory.", wstrConfigPath.c_str());
		logRecord.WriteLog(strFormatError.GetString(), ELL_ERROR);
		return bReturn;
	}

	MSXML2::IXMLDOMDocumentPtr ptrDoc = Load(wstrConfigPath.c_str());
	if (ptrDoc == NULL)
	{
		logRecord.WriteLog(L"Load ConfigInfo.xml Failed.", ELL_ERROR);
		return bReturn;
	}

	MSXML2::IXMLDOMNodePtr ptrNode = ptrDoc->selectSingleNode(L"/ConfigInfo/ServerIP");
	if (ptrNode != NULL)
	{
		g_wstrServerIP = ptrNode->Gettext();
	}
	
	ptrNode = NULL;
	ptrNode = ptrDoc->selectSingleNode(L"/ConfigInfo/SubscribeRole");
	if (ptrNode != NULL)
	{
		MSXML2::IXMLDOMNodePtr ptrChildNode = ptrNode->selectSingleNode(L"check");
		if (ptrChildNode != NULL)
		{
			wstring wstrCheck = ptrChildNode->Gettext();
			if (0 == wcscmp(wstrCheck.c_str(), L"true") || 0 == wcscmp(wstrCheck.c_str(), L"True") || 0 == wcscmp(wstrCheck.c_str(), L"TRUE"))
			{
				g_bSubscribeCheck = true;
				MSXML2::IXMLDOMNamedNodeMapPtr attributes = ptrChildNode->Getattributes();
				if (attributes != NULL)
				{
					MSXML2::IXMLDOMNodePtr pAttriNode = attributes->getNamedItem(L"subscribe_status");
					if (pAttriNode != NULL)
					{
						g_wsCheckStatus = pAttriNode->Gettext();
					}
				}
			}
		}

		ptrChildNode = NULL;
		ptrChildNode = ptrNode->selectSingleNode(L"recheck");
		if (ptrChildNode != NULL)
		{
			wstring wstrRecheck = ptrChildNode->Gettext();
			if (0 == wcscmp(wstrRecheck.c_str(), L"true") || 0 == wcscmp(wstrRecheck.c_str(), L"True") || 0 == wcscmp(wstrRecheck.c_str(), L"TRUE"))
			{
				g_bSubscribeRecheck = true;
				MSXML2::IXMLDOMNamedNodeMapPtr attributes = ptrChildNode->Getattributes();
				if (attributes != NULL)
				{
					MSXML2::IXMLDOMNodePtr pAttriNode = attributes->getNamedItem(L"subscribe_status");
					if (pAttriNode != NULL)
					{
						g_wsRecheckStatus = pAttriNode->Gettext();
					}
				}
			}
		}

		ptrChildNode = NULL;
		ptrChildNode = ptrNode->selectSingleNode(L"checkout");
		if (ptrChildNode != NULL)
		{
			wstring wstrCheckout = ptrChildNode->Gettext();
			if (0 == wcscmp(wstrCheckout.c_str(), L"true") || 0 == wcscmp(wstrCheckout.c_str(), L"True") || 0 == wcscmp(wstrCheckout.c_str(), L"TRUE"))
			{
				g_bSubscribeCheckout = true;
				MSXML2::IXMLDOMNamedNodeMapPtr attributes = ptrChildNode->Getattributes();
				if (attributes != NULL)
				{
					MSXML2::IXMLDOMNodePtr pAttriNode = attributes->getNamedItem(L"subscribe_status");
					if (pAttriNode != NULL)
					{
						g_wsCheckoutStatus = pAttriNode->Gettext();
					}
				}
			}
		}
	}
	
	MSXML2::IXMLDOMNodeListPtr ptrTaskItems = ptrDoc->selectNodes(L"/ConfigInfo/TaskItem");
	if (ptrTaskItems != NULL)
	{
		int nTaskItems = static_cast<int>(ptrTaskItems->Getlength());
		MSXML2::IXMLDOMNodePtr ptrTaskItem = NULL;
		for (int i = 0; i < nTaskItems; i++)
		{
			ptrTaskItem = ptrTaskItems->Getitem(i);

			MSXML2::IXMLDOMNodePtr ptrKeyNode = ptrTaskItem->selectSingleNode(L"Key");
			MSXML2::IXMLDOMNodePtr ptrPathNode = ptrTaskItem->selectSingleNode(L"ValuePath");
			if (ptrKeyNode != NULL && ptrPathNode != NULL)
			{
				wstring wstrKey = ptrKeyNode->Gettext();
				wstring wstrXPath = ptrPathNode->Gettext();
				g_vecTaskConfig.emplace_back(TaskItem(wstrKey.c_str(), wstrXPath.c_str()));
			}
		}

		bReturn = true;
	}

	logRecord.WriteLog(L"Finished: Get ConfigFile infomation SAVED in ConfigInfo.xml.");
	return bReturn;
}

MSXML2::IXMLDOMDocumentPtr Load(const wchar_t* wstrXmlFile, bool bXmlFile)
{
	CoInitialize(NULL);
	MSXML2::IXMLDOMDocumentPtr ptrDoc = NULL;
	ptrDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if (ptrDoc == NULL)
	{
		return ptrDoc; //fail to create document
	}

	if (bXmlFile)
	{
		VARIANT_BOOL varbool = ptrDoc->load(wstrXmlFile);

		if (0 == varbool || NULL == ptrDoc->documentElement)
		{
			ptrDoc = NULL; // fail to load print template file
		}
	}
	else
	{
		VARIANT_BOOL varbool = ptrDoc->loadXML(wstrXmlFile);

		if (0 == varbool || NULL == ptrDoc->documentElement)
		{
			ptrDoc = NULL; // fail to load print template file
		}
	}
	
	//for debug
	MSXML2::IXMLDOMElementPtr ptrElement = ptrDoc->GetdocumentElement();
	wstring docName = ptrElement->GetnodeName();
	wstring wstrText = ptrElement->Gettext();
	//end debug

	return ptrDoc;
}

bool GetUniXML(const wchar_t* pwsTaskId, std::wstring& wstrUniXml)
{
	//¶©ÔÄÄ£ÐÍÎªcontainer
	std::wstring wstrCheckUnitId;
	if (!GetCheckUnitId(pwsTaskId, wstrCheckUnitId))
	{
		logRecord.WriteLog(_T("GetBrowseTaskContent: GetCheckUnitId failed"));
		return false;
	}

	std::wstring wstrImageId;
	if (!GetImageId(wstrCheckUnitId, wstrImageId))
	{
		logRecord.WriteLog(_T("GetBrowseTaskContent: GetImageId failed"));
		return false;
	}

	if (!GetIdrRdbXml(wstrImageId, wstrUniXml))
	{
		logRecord.WriteLog(_T("GetBrowseTaskContent: GetIdrRdbXml failed"));
		return false;
	}

	return true;
}

bool PaserUniXML(const wchar_t* pwsUniXML, TaskInfo& tkInfo)
{
	MSXML2::IXMLDOMDocumentPtr pDocument = Load(pwsUniXML, false);
	if (NULL == pDocument)
	{
		logRecord.WriteLog(L"PaserUniXML: Failed to Load UniXML string.");
		return false;
	}

	MSXML2::IXMLDOMNodePtr ptrNode = NULL;
	size_t nTaskItems = g_vecTaskConfig.size();
	for (int i = 0; i < nTaskItems; i++)
	{
		ptrNode = NULL;
		TaskItem ti = g_vecTaskConfig.at(i);
		wstring wstrValue = L"None";

		ptrNode = pDocument->selectSingleNode(ti.wstrValuePath.c_str());
		if (NULL != ptrNode)
		{
			wstrValue = ptrNode->Gettext();
		}

		tkInfo.map_info.emplace(ti.wstrKey, wstrValue);
	}

	return true;
}

bool IsFileExist(const wchar_t* pwstrFileName)
{
	return 0 == _waccess(pwstrFileName, 0);
}

bool IsFolderExist(const wchar_t* pwstrDirectory)
{
	bool bRetValue = false;

	WIN32_FIND_DATA  wfd;
	HANDLE hFind = FindFirstFile(pwstrDirectory, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		bRetValue = true;
	}

	FindClose(hFind);

	return bRetValue;
}

string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0)
	{
		return std::string("");
	}
	char* pszDst = new char[nLen];
	if (NULL == pszDst)
	{
		return std::string("");
	}
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp;
}

wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if (nSize <= 0)
	{
		return NULL;
	}
	WCHAR *pwszDst = new WCHAR[nSize + 1];
	if (NULL == pwszDst)
	{
		return NULL;
	}

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;
	if (pwszDst[0] == 0xFEFF) // skip Oxfeff  
		for (int i = 0; i < nSize; i++)
			pwszDst[i] = pwszDst[i + 1];

	wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}

string WChar2UTF8(const std::wstring& widestring)
{
	int nUFT8Size = WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (CP_UTF8 <= 0)
	{
		return std::string("");
	}
	std::vector<char> resultstring(nUFT8Size);

	int nConvertSize = WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], nUFT8Size, NULL, NULL);
	if (nConvertSize != nUFT8Size)
	{
		return std::string("");
	}

	return std::string(&resultstring[0]);
}

wstring UTF82WChar(const std::string& utf8string)
{
	int nSize = MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (nSize <= 0)
	{
		return L"";
	}

	std::vector<wchar_t> resultstring(nSize);

	int nConvertSize = MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], nSize);
	if (nConvertSize != nSize)
	{
		return L"";
	}

	return std::wstring(&resultstring[0]);
}

wstring GetCurTimeStampString()
{
	GUID guid_;
	HRESULT hReturn = ::CoCreateGuid(&guid_);

	ASSERT(S_OK == hReturn);

	CString strGuid = L"";

	strGuid.Format(_T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
		guid_.Data1, guid_.Data2, guid_.Data3,
		guid_.Data4[0], guid_.Data4[1], guid_.Data4[2], guid_.Data4[3],
		guid_.Data4[4], guid_.Data4[5], guid_.Data4[6], guid_.Data4[7]);


	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strTime;
	strTime.Format(L"%4d-%02d-%02d %02d:%02d:%02d %d %s", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, strGuid.GetString());

	return strTime.GetString();
}