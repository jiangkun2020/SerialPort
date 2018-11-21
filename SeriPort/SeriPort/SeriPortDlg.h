
// SeriPortDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "SerialPort.h"

// CSeriPortDlg �Ի���
class CSeriPortDlg : public CDialogEx
{
// ����
public:
	CSeriPortDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIPORT_DIALOG };
#endif

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
	afx_msg void OnBnClickedBtnSetdata();
	afx_msg void OnBnClickedBtnOpencom();
	afx_msg void OnBnClickedBtnRecvclear();
	afx_msg void OnCbnDropdownComboComNum();
	afx_msg void OnCbnSelendokComboBandrate();
	afx_msg void OnCbnSelendokComboComNum();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LONG OnCommReceivedata(WPARAM ch, LPARAM port); //�����¼���Ϣ��Ӧ����
	afx_msg void OnBnClickedBtnClearcount();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedCheckTimerSend();
	afx_msg void OnEnChangeEditTemerPeriod();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_EditRecv;
	CComboBox m_ComBox_ComNum;
	CComboBox m_ComBox_BandRate;
	CButton m_BtnChkSendHex;
	CEdit m_EditSnedData;
	CButton m_BtnChkTimeSend;
	CEdit m_EditTime;
	CButton m_BtnChkShowHex;

public:
	CSerialPort m_CSerialPort;

public:
	void CloseAllCom(void);
	char HexToChar(char c);
	int StrToHex(CString str, char * data);
	void ShowData();

public:
	static unsigned char s_uDataBit;//����λ
	static unsigned char s_uStopBit;//ֹͣλ
	static char s_CheCkBit;//��żУ��λ
	unsigned int m_iaudRate;//������
	unsigned char m_ComNum;//���ں�
	unsigned char *m_pRecvData;
	unsigned char m_ucRecvDataFlag; //���յ����ݱ�־λ
	BOOL m_bSerialProtOpen;  //��־�����Ƿ��
	long m_lRxdataCount; //��������
	long m_lSenddataCount; //��������
	long unsigned  m_lBufUnm;
	int m_iSetTime;
	CString m_strSendData;
	CString m_strRecvData;	
};
