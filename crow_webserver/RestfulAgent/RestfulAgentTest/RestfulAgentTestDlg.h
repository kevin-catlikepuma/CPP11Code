
// RestfulAgentTestDlg.h : ͷ�ļ�
//

#pragma once


// CRestfulAgentTestDlg �Ի���
class CRestfulAgentTestDlg : public CDialogEx
{
// ����
public:
	CRestfulAgentTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESTFULAGENTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPut();
	afx_msg void OnBnClickedButtonPost();
	afx_msg void OnBnClickedButtonGet();
};
