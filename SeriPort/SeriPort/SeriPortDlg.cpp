
// SeriPortDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SeriPort.h"
#include "SeriPortDlg.h"
#include "afxdialogex.h"
#include "SerialPort.h"
#include "SetDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SENDMAX 2048
#define CHECKCOM 1     
#define SHOWDATA 2

unsigned char CSeriPortDlg::s_uDataBit = 8;//����λ
unsigned char CSeriPortDlg::s_uStopBit = 1;//ֹͣλ
char CSeriPortDlg::s_CheCkBit = 'N';//��żУ��λ

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CSeriPortDlg �Ի���
CSeriPortDlg::CSeriPortDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERIPORT_DIALOG, pParent)
	, m_strSendData(_T(""))
	, m_strRecvData(_T(""))
	, m_iSetTime(1000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSeriPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RECV, m_EditRecv);
	DDX_Control(pDX, IDC_COMBO_COM_NUM, m_ComBox_ComNum);
	DDX_Control(pDX, IDC_COMBO_BANDRATE, m_ComBox_BandRate);
	DDX_Control(pDX, IDC_CHECK_SENDHEX, m_BtnChkSendHex);
	DDX_Control(pDX, IDC_EDIT_DATA, m_EditSnedData);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strSendData);
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRecvData);
	DDX_Control(pDX, IDC_CHECK_SHOWHEX, m_BtnChkShowHex);
	DDX_Control(pDX, IDC_CHECK_TIMER_SEND, m_BtnChkTimeSend);
	DDX_Control(pDX, IDC_EDIT_TEMER_PERIOD, m_EditTime);
	DDX_Text(pDX, IDC_EDIT_TEMER_PERIOD, m_iSetTime);
}

BEGIN_MESSAGE_MAP(CSeriPortDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommReceivedata) //������Ϣ����ֻ�������һ��
	ON_BN_CLICKED(IDC_BTN_SETDATA, &CSeriPortDlg::OnBnClickedBtnSetdata)
	ON_BN_CLICKED(IDC_BTN_OPENCOM, &CSeriPortDlg::OnBnClickedBtnOpencom)
	ON_BN_CLICKED(IDC_BTN_RECVCLEAR, &CSeriPortDlg::OnBnClickedBtnRecvclear)
	ON_CBN_DROPDOWN(IDC_COMBO_COM_NUM, &CSeriPortDlg::OnCbnDropdownComboComNum)
	ON_CBN_SELENDOK(IDC_COMBO_BANDRATE, &CSeriPortDlg::OnCbnSelendokComboBandrate)
	ON_CBN_SELENDOK(IDC_COMBO_COM_NUM, &CSeriPortDlg::OnCbnSelendokComboComNum)
	ON_BN_CLICKED(IDC_BTN_CLEARCOUNT, &CSeriPortDlg::OnBnClickedBtnClearcount)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSeriPortDlg::OnBnClickedBtnSend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_TIMER_SEND, &CSeriPortDlg::OnBnClickedCheckTimerSend)
	ON_EN_CHANGE(IDC_EDIT_TEMER_PERIOD, &CSeriPortDlg::OnEnChangeEditTemerPeriod)
END_MESSAGE_MAP()

// CSeriPortDlg ��Ϣ�������
BOOL CSeriPortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_bSerialProtOpen = FALSE;
	m_iaudRate = 9600;
	s_uDataBit = 8;
	s_uStopBit = 1;
	s_CheCkBit = 'N';
	m_lRxdataCount = 0;
	m_lSenddataCount = 0;

	m_CSerialPort.Hkey2ComboBox(m_ComBox_ComNum);
	m_ComNum = m_nComArray[0];
	m_ComBox_BandRate.SetCurSel(5);

	m_pRecvData = new unsigned char[1024 * 1024];
	m_lBufUnm = 0;
	SetTimer(SHOWDATA, 100, NULL);
	m_ucRecvDataFlag = 0;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSeriPortDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CSeriPortDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSeriPortDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�������ں�����չ���б��
void CSeriPortDlg::OnCbnDropdownComboComNum()
{
	CloseAllCom();

	m_CSerialPort.Hkey2ComboBox(m_ComBox_ComNum);

	m_ComBox_ComNum.SetCurSel(0);

	m_ComNum = m_nComArray[0];	
}

//ѡ�񴮿ں�
void CSeriPortDlg::OnCbnSelendokComboComNum()
{
	unsigned char i;
	i = m_ComBox_ComNum.GetCurSel();
	m_ComNum = m_nComArray[i];
}

void CSeriPortDlg::OnCbnSelendokComboBandrate()
{
	CString temp;
	int i = m_ComBox_BandRate.GetCurSel();
	switch (i)
	{
	case 0:
		i = 300;
		break;
	case 1:
		i = 600;
		break;
	case 2:
		i = 1200;
		break;
	case 3:
		i = 2400;
		break;
	case 4:
		i = 4800;
		break;
	case 5:
		i = 9600;
		break;
	case 6:
		i = 19200;
		break;
	case 7:
		i = 38400;
		break;
	case 8:
		i = 43000;
		break;
	case 9:
		i = 56000;
		break;
	case 10:
		i = 57600;
		break;
	case 11:
		i = 115200;
		break;
	default:
		break;
	}
	m_iaudRate = i;		 
	CloseAllCom();
}

void CSeriPortDlg::OnBnClickedBtnSetdata()
{
	CloseAllCom();
	SetDataDlg DataDlg;
	DataDlg.DoModal();
}

void CSeriPortDlg::OnBnClickedBtnOpencom()
{
	if (!m_bSerialProtOpen)
	{
		if (m_CSerialPort.InitPort(this->m_hWnd,m_ComNum,m_iaudRate,s_CheCkBit,s_uDataBit
			,s_uStopBit ,EV_RXFLAG | EV_RXCHAR, 512))
		{
			m_CSerialPort.StartMonitoring(); //���ô���ͨ�ż���߳�
			m_bSerialProtOpen = TRUE;
			SetDlgItemText(IDC_BTN_OPENCOM,"�رմ���");
		} 
		else
		{
			m_bSerialProtOpen = FALSE;
			CString str;
			str.Format("COM%d û�з��ֻ������豸ռ��",m_ComNum);
			AfxMessageBox(str);
		}
	}
	else
	{
		m_bSerialProtOpen = FALSE;
		CloseAllCom();
	}
}

void CSeriPortDlg::OnBnClickedBtnRecvclear()
{
	SetDlgItemText(IDC_EDIT_RECV, "");
}

void CSeriPortDlg::CloseAllCom(void) 
{
	KillTimer(1);
	m_CSerialPort.ClosePort();
	SetDlgItemText(IDC_BTN_OPENCOM, "�򿪴���");
}

void CSeriPortDlg::OnBnClickedBtnClearcount()
{
	m_lRxdataCount = 0;
	CString strTemp;
	strTemp.Format("%ld", m_lRxdataCount);
	strTemp = "����:" + strTemp;
	SetDlgItemText(IDC_STATIC_RECVCOUNT, strTemp);
	m_lSenddataCount = 0;
	strTemp.Format("%ld", m_lSenddataCount);
	strTemp = "����:" + strTemp;
	SetDlgItemText(IDC_STATIC_SENDDATACOUNT, strTemp);
}

void CSeriPortDlg::OnBnClickedBtnSend()
{
	if (!m_bSerialProtOpen)
	{
		CString str;
		str.Format("COM%d û�д�", m_ComNum);
		AfxMessageBox(str);
		return; 
	}
	char cbuf[SENDMAX + 1] = { 0 };
	SendDlgItemMessage(IDC_EDIT_DATA, WM_GETTEXT, sizeof(cbuf), (LPARAM)cbuf);
	if (strcmp("", cbuf) == 0)
		return;

	m_strSendData = cbuf;
	if (m_BtnChkSendHex.GetCheck())
	{
		char data[SENDMAX + 1] = { 0 };
		int len = StrToHex(m_strSendData, data);
		m_CSerialPort.WriteToPort(data, len); 
		m_lSenddataCount += (long)((m_strSendData.GetLength() + 1) / 3);
	} 
	else
	{
		m_CSerialPort.WriteToPort(cbuf);
		m_lSenddataCount += m_strSendData.GetLength();
	}

	CString strTemp;
	strTemp.Format("����:%d", m_lSenddataCount);
	SetDlgItemText(IDC_STATIC_SENDDATACOUNT, strTemp);
}

void CSeriPortDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case CHECKCOM:
		if (!m_bSerialProtOpen) 
		{
			KillTimer(CHECKCOM);
			CString str;
			str.Format("COM%d û�д�", m_ComNum);
			AfxMessageBox(str);
			return; 
		}
		OnBnClickedBtnSend();
		break;
	case  SHOWDATA:
		ShowData();
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

LONG CSeriPortDlg::OnCommReceivedata(WPARAM ch, LPARAM port)
{
	m_lRxdataCount++;  //�ñ������ڽ����ַ�����
	m_pRecvData[m_lBufUnm] = ch;//���뻺����
	m_lBufUnm++;
	m_ucRecvDataFlag = 1;//���յ�����һ
						 //�������ch���Ǵ��������ַ������ݣ�����ʹ�õ�����
	return 0;
}

void CSeriPortDlg::ShowData()
{
	CString strTemp;
	if (m_lBufUnm > 0)//�����յ����Ͷ�ʱ��ʱ�䵽����ʾ
	{
		long unsigned int buff_num_temp = m_lBufUnm;
		if (m_ucRecvDataFlag == 1)//rxh2.8���ڽ���
		{
			if (buff_num_temp > 300)///���ÿ�δ�����ʾ300���ֽ�
				buff_num_temp = 300;
		}
		else //�������,�����д���ʾ����
		{
			if (buff_num_temp > 3000)///���ÿ�δ�����ʾ3000���ֽ�
				buff_num_temp = 3000;
		}
		int i;
		for (i = 0;i < buff_num_temp;i++)
		{
			BYTE bt = *(char*)(m_pRecvData + i);
			//ʮ��������ʾ
			if (m_BtnChkShowHex.GetCheck())
				strTemp.Format("%02X ", bt);
			else
				strTemp.Format("%c", bt);
			m_strRecvData += strTemp;
		}
		//�ܻ��崮��
		memmove(m_pRecvData, &m_pRecvData[i], m_lBufUnm - i);
		m_lBufUnm -= i;//�ܻ���ָ�����*/

		m_EditRecv.SetSel(-1, -1);//ѡ�б༭���е��ı�
		m_EditRecv.ReplaceSel(m_strRecvData);//�滻�ı�
		m_strRecvData.Empty();//ʹ��������
	}

	strTemp.Format("%ld", m_lRxdataCount);
	strTemp = "����:" + strTemp;
	SetDlgItemText(IDC_STATIC_RECVCOUNT, strTemp);
	m_ucRecvDataFlag = 0;
}

char CSeriPortDlg::HexToChar(char c)
{
	if ((c >= '0') && (c <= '9'))
		return c - 0x30;
	else if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;
	else if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;
	else
		return 0x10;
}

int CSeriPortDlg::StrToHex(CString str, char* data)
{
	int t, t1;
	int rlen = 0, len = str.GetLength();
	for (int i = 0;i < len;)
	{
		char l, h = str[i];
		if (h == ' ')
		{
			i++;
			continue;
		}
		i++;
		if (i >= len)
			break;
		l = str[i];
		t = HexToChar(h);
		t1 = HexToChar(l);
		if ((t == 16) || (t1 == 16))
		{
			break;
		}
		else
		{
			t = t * 16 + t1;
		}
		i++;
		data[rlen] = (char)t;
		rlen++;
	}
	return rlen;
}

void CSeriPortDlg::OnBnClickedCheckTimerSend()
{
	UpdateData(TRUE);
	if (m_BtnChkTimeSend.GetCheck())//��ѡ��ʱ����
	{
		if (m_bSerialProtOpen)//�����Ѿ���
		{
			if (m_iSetTime < 1)
				KillTimer(CHECKCOM);
			else
				SetTimer(CHECKCOM, m_iSetTime, NULL);
		}
		else
		{
			m_BtnChkTimeSend.SetCheck(FALSE);//ȡ��ѡ�ж�ʱ����
			CString str;
			str.Format("COM%d û�д�", m_ComNum);
			AfxMessageBox(str);
			return; 
		}
	}
	else
		KillTimer(CHECKCOM);
}

void CSeriPortDlg::OnEnChangeEditTemerPeriod()
{
	UpdateData(TRUE);
	if (m_BtnChkTimeSend.GetCheck() && m_bSerialProtOpen)//��ѡ��ʱ���Ͳ��Ҵ����Ѿ���
	{
		if (m_iSetTime < 1)
			KillTimer(CHECKCOM);
		else
			SetTimer(CHECKCOM, m_iSetTime, NULL);
	}
}

void CSeriPortDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (m_pRecvData != nullptr)
	{
		delete[] m_pRecvData;
		m_pRecvData = nullptr;//��ֵ��ָ�룬��ֹ��ɾ�ռ�
	}
}
