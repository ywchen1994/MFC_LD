
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

CStringA CMFC_LDDlg::FileName = "20150210-130128CO.avi";
CvCapture *cap;
Mat CMFC_LDDlg::frame;
uint16_t CMFC_LDDlg::frame_counter = 0;
CMFC_LDDlg::CMFC_LDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_LD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_LDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageSource, m_ImageSource);
	DDX_Control(pDX, IDC_ImageResult, m_ImageResult);
}

BEGIN_MESSAGE_MAP(CMFC_LDDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnOpenFIle, &CMFC_LDDlg::OnBnClickedBtnopenfile)
	ON_BN_CLICKED(IDC_BtnStart, &CMFC_LDDlg::OnBnClickedBtnstart)
	
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
	m_ImageResult.SetWindowPos(NULL,640, 45, 1255, 610, SWP_SHOWWINDOW);
	m_ImageSource.SetWindowPos(NULL, 0, 45,640,360, SWP_SHOWWINDOW);
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


void CMFC_LDDlg::OnBnClickedBtnopenfile()
{
	CFileDialog OpnDlg(TRUE);
	if (OpnDlg.DoModal() == IDOK)
	   FileName = OpnDlg.GetPathName(); // return full path and filename
	cap = cvCaptureFromFile(FileName);
	frame_counter = 0;
}
void CMFC_LDDlg::ShowImage(cv::Mat Image, CWnd* pWnd)
{
	cv::Mat imgTmp;
	CRect rect;
	pWnd->GetClientRect(&rect);
	cv::resize(Image, imgTmp, cv::Size(rect.Width(), rect.Height()));

	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA);
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1);
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = pWnd->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}
UINT CMFC_LDDlg::threadFun(LPVOID LParam)
{
	CthreadParam* para = (CthreadParam*)LParam;
	CMFC_LDDlg* lpview = (CMFC_LDDlg*)(para->m_lpPara);
	para->m_blthreading = TRUE;

	switch (para->m_case)
	{
	case 0:
		lpview->Thread_CapFromFile(LParam);//0������ǰ���
		break;

	default:
		break;
	}

	para->m_blthreading = FALSE;
	para->m_case = 0xFF;
	return 0;

}

void CMFC_LDDlg::OnBnClickedBtnstart()
{
	/*�}��0��������*/
	m_threadPara.m_case = 0;
	m_threadPara.hWnd = m_hWnd;
	m_lpThread = AfxBeginThread(&CMFC_LDDlg::threadFun, (LPVOID)&m_threadPara);
}

void CMFC_LDDlg::Thread_CapFromFile(LPVOID lParam)
{
	CthreadParam * Thread_Info = (CthreadParam *)lParam;
	CMFC_LDDlg * hWnd = (CMFC_LDDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	uint16_t total_frameNo = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT);
	bool flag_Night;
	if (cvGrabFrame(cap))
	{
		frame = cvRetrieveFrame(cap);
		frame_counter++;
		
		Mat Img= frame;
		flag_Night = NightMod(Img);
		if (!flag_Night)hWnd->SetDlgItemText(IDC_FrameMode, _T("Day"));
		if (flag_Night)hWnd->SetDlgItemText(IDC_FrameMode,_T("Night"));
	}
	//cvSetCaptureProperty(cap,CV_CAP_PROP_POS_FRAMES,80);
	while (cvGrabFrame(cap))
	{
			frame = cvRetrieveFrame(cap);
			frame_counter++;
			Mat Img = frame;
			Mat Img_ROI = Img(cv::Rect(cv::Point(0, 450), cv::Point(1225, 610)));
			Mat Img_Mask;
			Mat Img_finalResult = Mat::zeros(Img_ROI.size(), CV_8UC3);
			FindLineMask(Img_ROI, Img_Mask);
			vector<vector<cv::Point>> contours; // Vector for storing contour
			vector<Vec4i>hierarchy;
			findContours(Img_Mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image
			for (int i = 0; i < contours.size(); i++) // Iterate through each contour
			{
				double Area = contourArea(contours[i], false);
				if (Area > 250)/*�p�G���n�O250�H�U�P�_���������T*/
				{
					Mat temp = Mat::zeros(Img_Mask.size(), CV_8UC1);
					drawContours(temp, contours, i, Scalar(255), CV_FILLED, 8, hierarchy);
 					cv::Moments m = cv::moments(temp, false);
					cv::Point center = cv::Point(m.m10 / m.m00, m.m01 / m.m00);
					string LineType = LineClassify(temp, center, Area);
					Mat Img_ColorMask;	std::string LineColor;
					if (LineType != "false")
					{
						cvtColor(temp, Img_ColorMask, CV_GRAY2RGB);
						bitwise_and(Img_ColorMask, Img_ROI, Img_ColorMask);//�Q��ROI�P���D�u�B�n���o��@���D���C���T
						LineColor = GetColor(Img_ColorMask, flag_Night);
					}
					
					if (LineColor != "false" && LineType != "false")
					{
						std::string L;
						L = LineColor + " " + LineType;
						bitwise_or(Img_ColorMask, Img_finalResult, Img_finalResult);//�N�C�@�������������D�u���G�|�[�_�ӦX���@�i��
						ImgText(&(IplImage)Img_finalResult, L, center.x, center.y);
					}	
				}
			}
			
			hWnd->ShowImage(frame,hWnd->GetDlgItem(IDC_ImageSource));
		    hWnd->ShowImage(Img_finalResult, hWnd->GetDlgItem(IDC_ImageResult));
			CString str;
			str.Format(_T("%d / %d"), frame_counter, total_frameNo);
			hWnd->SetDlgItemText(IDC_FrameCount, str);
		
	}

}
void CMFC_LDDlg::FindLineMask(Mat src, Mat &dst) {
	dst = Mat::zeros(src.size(), CV_8UC1);
	Mat img_gray;
	cvtColor(src, img_gray, CV_RGB2GRAY);
	Mat img_blur;
	GaussianBlur(img_gray, img_blur, cv::Size(5, 5), 0, 0);
	Mat edges;
	Canny(img_blur, edges, 70, 150);
	Mat Img_Line;
	HoughLineDetection(edges, Img_Line);//�M���N�ҽu
	LineCompensate(Img_Line, Img_Line, 35);
	dst = Img_Line;
}
/*********NightMod****�Q�ΫG�קP�_�v���O�_���]��***********/
bool CMFC_LDDlg::NightMod(cv::Mat img)
{
	Mat Img_gray;
	
	cvtColor(img, Img_gray,CV_BGR2GRAY);
	double sum = 0;
	for(uint16_t i=0;i<Img_gray.cols;i++)
		for (uint16_t j = 0; j < Img_gray.rows; j++)
		{
			sum+=Img_gray.at<uchar>(j, i);
		}
	sum = sum / (Img_gray.rows*Img_gray.cols);
	if (sum >= 100)return false;
	if (sum < 100)return true;
}


/*******LineCompensate***�N���檺�u�s������ߪ��u*******thrshold���Z���A�ثe�]�w��35��pixel***********/
void CMFC_LDDlg::LineCompensate(cv::Mat src, cv::Mat &dst, uint16_t thrshold)
{
	clock_t t0 = clock();
	Mat Img_Mask;
	src.copyTo(Img_Mask);
	for (uint16_t j = 0; j < src.rows; j++)
	{
		Vector<cv::Point> Compensates;
		for (uint16_t i = 0; i < src.cols; i++)
		{
			if (src.at<uchar>(j, i) > 0)
				Compensates.push_back(cv::Point(i, j));
		}
		if (Compensates.size() >= 2)
		{
			for (size_t i = 0; i < Compensates.size() - 1; i++)
				if ((Compensates[i + 1].x - Compensates[i].x) < thrshold && (Compensates[i + 1].x - Compensates[i].x)>2)
					line(Img_Mask, Compensates[i], Compensates[i + 1], Scalar(255), 2);
		}
	}
	IplConvKernel *pKernel = NULL;
	pKernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);
	cvErode(&(IplImage)dst, &(IplImage)dst, pKernel, 1);
	dst = Img_Mask;
	clock_t t2 = clock();
	float t = (float)(t2 - t0) / CLK_TCK;
	
}

/*******LineClassify************�Q�νu�e�νu���P�w���u�B��u�B��u�B��u*****************/
string CMFC_LDDlg::LineClassify(cv::Mat src, cv::Point Center, double Area)
{
	int LineWidth[2] = { 0 };
	
	for (uint16_t i = 1; i < src.cols; i++)
	{
		if (src.at<uchar>(Center.y, i) > 0 && src.at<uchar>(Center.y, i - 1) == 0)//�����k����
			LineWidth[0] = i;
		if (src.at<uchar>(Center.y, src.cols - i) > 0 && src.at<uchar>(Center.y, src.cols - i + 1) == 0)//�k��������
			LineWidth[1] = src.cols - i - 1;
	}
	double Length = Area / (abs(LineWidth[1] - LineWidth[0]));
	if (abs(LineWidth[1] - LineWidth[0]) > 15 && abs(LineWidth[1] - LineWidth[0]) < 48)//��u
	{
		if (Length > 35 && Length < 120)//��u
		{
			return "dotted line";
		}
		else if (Length > 130 && Length < 180)//��u
		{
		
			return "Solid line";
		}
		else
		return "false";
	}
	else if (abs(LineWidth[1] - LineWidth[0])>=48 && abs(LineWidth[1] - LineWidth[0])<80 && Length > 130 && Length < 180)//���u
	{
		return "double Line";
	}
	else
		return "false";

}
//�N�ҽu�����N+-20~+-60�ת��u�^���X��
void CMFC_LDDlg::HoughLineDetection(cv::Mat src, cv::Mat &dst)
{
	vector<Vec4i> lines;
	int hough_threshold = 10;
	int hough_minLineLength = 60;
	IplConvKernel *pKernel = NULL;
	pKernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);
	cvDilate(&(IplImage)src, &(IplImage)src, pKernel, 1);
	cvErode(&(IplImage)src, &(IplImage)src, pKernel, 1);
	HoughLinesP(src, lines, 1, CV_PI / 180, hough_threshold, hough_minLineLength, 50);
	dst = Mat::zeros(src.size(), CV_8UC1);
	for (uint16_t i = 0; i < lines.size(); i++)
	{
		if (lines[i][2] != lines[i][0])
		{
			double theta = atan((double)(lines[i][3] - lines[i][1]) / (double)(lines[i][2] - lines[i][0]));
			theta = theta * 180 / M_PI;
			if (20 <= theta  && theta <= 60 && lines[i][0]>= src.cols/2)//�k��
				cv::line(dst, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), Scalar(255));
		    if (-20 >= theta  && theta >= -60 && lines[i][0]<=src.cols / 2)//����
				cv::line(dst, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), Scalar(255));
		}
	}
	cvDilate(&(IplImage)dst, &(IplImage)dst, pKernel, 1);
}


/*******GetColor******RGB��HSV������鶡�Ω]�����֭�******************/
std::string CMFC_LDDlg::GetColor(cv::Mat img,bool f_Night)
{
	Mat temp_w, temp_y, temp_r;
	Mat Img_HSV;
	cv::circle(img, cv::Point(0, 1), 1, CV_RGB(255, 0, 0));
	cvtColor(img, Img_HSV, CV_RGB2HSV);
	if (f_Night == false)
	{
		inRange(Img_HSV, Scalar(0, 0, 180), Scalar(180, 60, 255), temp_w);
		inRange(Img_HSV, Scalar(120, 43, 46), Scalar(165, 255, 255), temp_r);
		inRange(Img_HSV, Scalar(90, 60, 160), Scalar(180, 245, 255), temp_y);
	}
	if (f_Night)
	{
		inRange(Img_HSV, Scalar(0,0, 60), Scalar(170, 40, 255), temp_w);
		
		inRange(Img_HSV, Scalar(77, 16, 105), Scalar(117, 116, 252), temp_y);
	}
	std::vector<int>grade;
	grade.push_back(cv::countNonZero(temp_w)); grade.push_back(cv::countNonZero(temp_r)); grade.push_back(cv::countNonZero(temp_y));
	std::vector<int>::iterator biggest = std::max_element(std::begin(grade), std::end(grade));

	if (*biggest > 10)
	{
		switch (std::distance(std::begin(grade), biggest))
		{
		case 0:
			return "White";
			break;
		case 1:
			return "Red";
			break;
		case 2:
			return "Yellow";
			break;
		}
	}
	else
		return "false";
}
/*******ImgText*******�N��r�q�b�ϤW(�i���i�L)�A�]���S���Q���L���e�{�覡***/
void CMFC_LDDlg::ImgText(IplImage* img, std::string text, int x, int y)
{
	CvFont font;
	double hscale = 0.6;
	double vscale = 0.6;
	int linewidth = 2;
	char buf[1024] = { 0 };
	memcpy(buf, text.c_str(), text.size());
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
	CvScalar textColor = cvScalar(0, 0, 255);
	CvPoint textPos = cvPoint(x, y);
	cvPutText(img, buf, textPos, &font, textColor);
}


