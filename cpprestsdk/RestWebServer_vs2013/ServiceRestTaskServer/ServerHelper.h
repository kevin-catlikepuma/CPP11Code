#pragma once
#include "stdafx.h"
#include "Defs.h"

bool	ParserConfigFile();
MSXML2::IXMLDOMDocumentPtr Load(const wchar_t* wstrXmlFile, bool bXmlFile=true);
bool	GetUniXML(const wchar_t* pwsTaskId, std::wstring& wstrUniXml);
bool	PaserUniXML(const wchar_t* pwsUniXML, TaskInfo& tkInfo);

bool	IsFileExist(const wchar_t* pwstrFileName);
bool	IsFolderExist(const wchar_t* pwstrDirectory);

string  WChar2Ansi(LPCWSTR pwszSrc);
wstring Ansi2WChar(LPCSTR pszSrc, int nLen);

string  WChar2UTF8(const std::wstring& widestring);
wstring UTF82WChar(const std::string& utf8string);
wstring GetCurTimeStampString();