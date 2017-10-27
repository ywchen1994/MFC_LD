
// MFC_LDDlg.h : ���Y��
//

#pragma once
#include "afxwin.h"
#define _USE_MATH_DEFINES
#include"math.h"
#include"cv.h"
#include"highgui.h"
using namespace cv;
struct CthreadParam
{
public:
	HWND hWnd;
	LPVOID m_lpPara;
	UINT   m_case;
	BOOL m_blthreading;
};

// CMFC_LDDlg ��ܤ��
class CMFC_LDDlg : public CDialogEx
{
// �غc
public:
	CMFC_LDDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_LD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CthreadParam m_threadPara;
	CWinThread*  m_lpThread;
	static UINT threadFun(LPVOID LParam);
	
	CStatic m_ImageSource;
	static CStringA FileName;

	afx_msg void OnBnClickedBtnopenfile();
	void ShowImage(cv::Mat Image, CWnd * pWnd);
	afx_msg void OnBnClickedBtnstart();
	void Thread_CapFromFile(LPVOID lParam);
	void FindLineMask(Mat src, Mat & dst);

	bool NightMod(cv::Mat img);
	void LineCompensate(cv::Mat src, cv::Mat & dst, uint16_t thrshold);
	string LineClassify(cv::Mat src, cv::Point Center, double Area);
	void HoughLineDetection(cv::Mat src, cv::Mat & dst);
	std::string GetColor(cv::Mat img,bool Night);
	void ImgText(IplImage * img, std::string text, int x, int y);

	static Mat frame;
	static uint16_t frame_counter;
	CStatic m_ImageResult;
};
