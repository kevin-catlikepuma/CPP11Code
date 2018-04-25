#pragma once
#include "stdafx.h"

enum ELogLevel
{
	ELL_DEBUG,
	ELL_WARNING,
	ELL_ERROR,
	ELL_INFO
};

void process_log_file(std::wstring wstrLogDir);

class CLogRecord
{
public:
	CLogRecord(const wchar_t* pwstrFileName);
	~CLogRecord();

	bool OpenLogFile();
	bool OpenNewLogFile(const wchar_t* pwstrYearMonthDay);
	void CloseLogFile();

	void WriteLog(const wchar_t* pwstrMessage, ELogLevel eLevel = ELL_INFO);

	void SeekToEnd();
	void Flush();

private:
	std::wstring GetSysTimeString();

private:
	std::wstring		m_wstrFileName;
	std::wstring		m_wstrLogFile; // ���ļ����������ļ���m_wstrFileName�Ļ����������������Ϣ
	CStdioFile			m_file;
	CRITICAL_SECTION	m_csLog;
};