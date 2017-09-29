
// MFC_LDDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MFC_LD.h"
#include "MFC_LDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_LDDlg ��ܤ��



CMFC_LDDlg::CMFC_LDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_LD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_LDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC_LDDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFC_LDDlg �T���B�z�`��

BOOL CMFC_LDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CMFC_LDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CMFC_LDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

