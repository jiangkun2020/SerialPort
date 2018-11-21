// SetDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SeriPort.h"
#include "SetDataDlg.h"
#include "afxdialogex.h"


// SetDataDlg 对话框

IMPLEMENT_DYNAMIC(SetDataDlg, CDialog)

SetDataDlg::SetDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SETDATA_DIALOG, pParent)
{

}

SetDataDlg::~SetDataDlg()
{
}

void SetDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATABIT, m_ComBox_DataBit);
	DDX_Control(pDX, IDC_COMBO_STOPBIT, m_ComBox_StopBit);
	DDX_Control(pDX, IDC_COMBO_CHECKBIT, m_ComBox_CheCkBit);
}

BEGIN_MESSAGE_MAP(SetDataDlg, CDialog)
	ON_CBN_SELENDOK(IDC_COMBO_DATABIT, &SetDataDlg::OnCbnSelendokComboDatabit)
	ON_CBN_SELENDOK(IDC_COMBO_STOPBIT, &SetDataDlg::OnCbnSelendokComboStopbit)
	ON_CBN_SELENDOK(IDC_COMBO_CHECKBIT, &SetDataDlg::OnCbnSelendokComboCheckbit)
END_MESSAGE_MAP()


// SetDataDlg 消息处理程序


BOOL SetDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (CSeriPortDlg::s_uDataBit)
	{
	case 5:    //数据位默认为5位	
		m_ComBox_DataBit.SetCurSel(0);
		break;
	case 6:    //数据位默认为6位	
		m_ComBox_DataBit.SetCurSel(1);
		break;
	case 7:   //数据位默认为7位	
		m_ComBox_DataBit.SetCurSel(2);
		break;
	case 8:   //数据位默认为8位
		m_ComBox_DataBit.SetCurSel(3);
		break;	
	default:
		break;
	}

	switch (CSeriPortDlg::s_uStopBit)
	{
	case 1:   //停止位1位	
		m_ComBox_StopBit.SetCurSel(0);
		break;
	case 2:   //停止位2位	
		m_ComBox_StopBit.SetCurSel(1);
		break;	
	default:
		break;
	}

	switch (CSeriPortDlg::s_CheCkBit)
	{
	case 'N':  //默认无校验
		m_ComBox_CheCkBit.SetCurSel(0);
		break;
	case 'O':  //奇校验
		m_ComBox_CheCkBit.SetCurSel(1);
		break;	
	case 'E':  //偶校验	
		m_ComBox_CheCkBit.SetCurSel(2);
		break;
	default:
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SetDataDlg::OnCbnSelendokComboDatabit()
{
	unsigned char ch;
	ch = m_ComBox_DataBit.GetCurSel();
	switch (ch)
	{
	case 0:
		ch = 5;
		break;
	case 1:
		ch = 6;
		break;
	case 2:
		ch = 7;
		break;
	case 3:
		ch = 8;
		break;
	default:
		break;
	}
	CSeriPortDlg::s_uDataBit = ch;
}


void SetDataDlg::OnCbnSelendokComboStopbit()
{
	unsigned char ch;
	ch = m_ComBox_StopBit.GetCurSel();
	switch (ch)
	{
	case 0:
		ch = 1;
		break;
	case 1:
		ch = 2;
		break;
	default:
		break;
	}
	CSeriPortDlg::s_uStopBit = ch;
}


void SetDataDlg::OnCbnSelendokComboCheckbit()
{
	char temp;
	int i = m_ComBox_CheCkBit.GetCurSel();
	switch (i)
	{
	case 0:
		temp = 'N';//无校验
		break;
	case 1:
		temp = 'O';//奇校验
		break;
	case 2:
		temp = 'E';//偶校验
		break;
	}
	CSeriPortDlg::s_CheCkBit = temp;
}
