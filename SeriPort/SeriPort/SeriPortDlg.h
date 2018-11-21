
// SeriPortDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "SerialPort.h"

// CSeriPortDlg 对话框
class CSeriPortDlg : public CDialogEx
{
// 构造
public:
	CSeriPortDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIPORT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
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
	afx_msg LONG OnCommReceivedata(WPARAM ch, LPARAM port); //串口事件消息响应函数
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
	static unsigned char s_uDataBit;//数据位
	static unsigned char s_uStopBit;//停止位
	static char s_CheCkBit;//奇偶校验位
	unsigned int m_iaudRate;//波特率
	unsigned char m_ComNum;//串口号
	unsigned char *m_pRecvData;
	unsigned char m_ucRecvDataFlag; //接收到数据标志位
	BOOL m_bSerialProtOpen;  //标志串口是否打开
	long m_lRxdataCount; //接收数量
	long m_lSenddataCount; //发送数量
	long unsigned  m_lBufUnm;
	int m_iSetTime;
	CString m_strSendData;
	CString m_strRecvData;	
};
