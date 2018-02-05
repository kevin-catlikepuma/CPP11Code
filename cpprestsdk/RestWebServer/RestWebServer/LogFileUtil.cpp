#include "stdafx.h"
#include <stdio.h>
#include "LogFileUtil.h"

CLogRecord::CLogRecord(const wchar_t* pwstrFileName)
{
	InitializeCriticalSection(&m_csLog);
	m_wstrFileName = pwstrFileName;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strFormatTime;
	strFormatTime.Format(L"%d-%d-%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	m_wstrLogFile = m_wstrFileName + strFormatTime.GetString() + L".txt";
}

CLogRecord::~CLogRecord()
{
	m_wstrFileName.clear();
	m_wstrLogFile.clear();
	if(m_file.m_pStream != NULL)
	{
		m_file.Close();
	}

	DeleteCriticalSection(&m_csLog);
}

bool CLogRecord::OpenLogFile()
{
	if(m_file.m_pStream != NULL)
	{
		m_file.Close();
	}

	setlocale(LC_CTYPE, ("chs"));

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	std::wstring wstrExeFileName = szPath;
	size_t nPos = wstrExeFileName.rfind(L"\\");

	std::wstring wstrLogFile = wstrExeFileName.substr(0, nPos+1);
	wstrLogFile += L"Log\\";
	if(!IsFolderExist(wstrLogFile.c_str()))
	{
		CreateDirectory(wstrLogFile.c_str(), NULL);
	}

	wstrLogFile += m_wstrLogFile;

	bool bReturn = false;
	BOOL bOpenReturn = m_file.Open(wstrLogFile.c_str(), CFile::modeReadWrite);

	if(!bOpenReturn)
	{
		bOpenReturn = m_file.Open(wstrLogFile.c_str(), CFile::modeCreate|CFile::modeReadWrite);
	}

	if(bOpenReturn)
	{
		bReturn = true;
		SeekToEnd();
	}
	else
	{
		int nErrorCode = GetLastError();
		CString strErrorMsg;
		strErrorMsg.Format(L"Failed to open %s file, errorCode is %d", m_wstrFileName.c_str(), nErrorCode);
		OutputDebugString(strErrorMsg.GetString());
	}

	return bReturn;
}

bool CLogRecord::OpenNewLogFile(const wchar_t* pwstrYearMonthDay)
{
	wstring wstrYearMonthDay = pwstrYearMonthDay;
	if(wstrYearMonthDay.empty())
	{
		return false;
	}

	wstring wstrLogFile = m_wstrFileName + wstrYearMonthDay + L".txt";

	if (0 != wcscmp(m_wstrLogFile.c_str(), wstrLogFile.c_str()))
	{
		m_wstrLogFile = wstrLogFile;
		OpenLogFile();
	}

	return true;
}

void CLogRecord::WriteLog(const wchar_t* pwstrMessage, ELogLevel eLevel)
{
	if(m_file.m_pStream == NULL)
	{
		OutputDebugString(L"Please Open Log File first.");
	}
	else
	{
		CString strMessage;

		switch(eLevel)
		{
		case ELL_DEBUG:
			{
				strMessage.Format(L"%s DEBUG:   %s ", GetSysTimeString().c_str(), pwstrMessage);
				break;
			}
		case ELL_WARNING:
			{
				strMessage.Format(L"%s WARNING: %s ", GetSysTimeString().c_str(), pwstrMessage);
				break;
			}
		case ELL_ERROR:
			{
				strMessage.Format(L"%s ERROR:   %s ", GetSysTimeString().c_str(), pwstrMessage);
				break;
			}
		case ELL_INFO:
			{
				strMessage.Format(L"%s INFO:    %s ", GetSysTimeString().c_str(), pwstrMessage);
				break;
			}
		default:
			{
				break;
			}
		}

		wprintf(strMessage.GetString()); // 输出到控制台
		wprintf(L"\n");

		EnterCriticalSection(&m_csLog);
		m_file.WriteString(strMessage.GetString());
		m_file.WriteString(L"\n");
		LeaveCriticalSection(&m_csLog);
	}
}

void CLogRecord::SeekToEnd()
{
	if(m_file.m_pStream != NULL)
	{
		m_file.SeekToEnd();
	}
}

void CLogRecord::Flush()
{
	if(m_file.m_pStream != NULL)
	{
		m_file.Flush();
	}
}

void CLogRecord::CloseLogFile()
{
	if(m_file.m_pStream != NULL)
	{
		m_file.Close();
	}
}

wstring CLogRecord::GetSysTimeString()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strTime;
	strTime.Format(L"%4d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	return strTime.GetString();
}

bool CLogRecord::IsFolderExist(const wchar_t* pwstrDirectory)
{
	bool bRetValue = false;

	WIN32_FIND_DATA  wfd;
    HANDLE hFind = FindFirstFile(pwstrDirectory, &wfd);
    if((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        bRetValue = true;   
    }

    FindClose(hFind);

    return bRetValue;
}
