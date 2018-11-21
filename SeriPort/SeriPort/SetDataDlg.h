#pragma once
#include "afxwin.h"
#include "SeriPortDlg.h"

// SetDataDlg �Ի���

class SetDataDlg : public CDialog
{
	DECLARE_DYNAMIC(SetDataDlg)

public:
	SetDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetDataDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETDATA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComBox_DataBit;
	CComboBox m_ComBox_StopBit;
	CComboBox m_ComBox_CheCkBit;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelendokComboDatabit();
	afx_msg void OnCbnSelendokComboStopbit();
	afx_msg void OnCbnSelendokComboCheckbit();
};
