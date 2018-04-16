
// MyFaceDetectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyFaceDetect.h"
#include "MyFaceDetectDlg.h"
#include "afxdialogex.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyFaceDetectDlg 对话框



CMyFaceDetectDlg::CMyFaceDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyFaceDetectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_catIcon = AfxGetApp()->LoadIcon(IDI_ICON4);
	
	scale = 1.3;
}

void CMyFaceDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FACEDETECT, m_btn);
}

BEGIN_MESSAGE_MAP(CMyFaceDetectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FACEDETECT, &CMyFaceDetectDlg::OnClickedFacedetect)
	ON_BN_CLICKED(IDC_FACEV, &CMyFaceDetectDlg::OnClickedFacev)
	ON_BN_CLICKED(IDC_FACEC, &CMyFaceDetectDlg::OnClickedFacec)
	ON_BN_CLICKED(IDCANCEL, &CMyFaceDetectDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMyFaceDetectDlg 消息处理程序

BOOL CMyFaceDetectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_catIcon, TRUE);			// 设置大图标
	SetIcon(m_catIcon, FALSE);		// 设置小图标
	//按钮加载图片背景
	//HBITMAP hbmp1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2));
	//m_btn.SetBitmap(hbmp1);

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyFaceDetectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_catIcon);
	}
	else
	{
		/*改变对话框背景*/
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcBmp;
		dcBmp.CreateCompatibleDC(&dc);
		CBitmap bmpBackGround;
		bmpBackGround.LoadBitmap(IDB_BITMAP4);
		BITMAP m_bitmap;
		bmpBackGround.GetBitmap(&m_bitmap);
		CBitmap *pbmpOld = dcBmp.SelectObject(&bmpBackGround);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
		CDialogEx::OnPaint();
		
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyFaceDetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_catIcon);
}



void CMyFaceDetectDlg::OnClickedFacedetect()
{
	// TODO:  在此添加控件通知处理程序代码
	CString filename;
	CFileDialog OpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("图片 (*.jpg)|*.jpg|(*.*) |*.*|"), NULL);
	if (OpenDlg.DoModal() != IDOK)
	{
		return;
	}
	/*CString转换*string*/
	filename = OpenDlg.GetPathName();
	USES_CONVERSION;
	std::string tempName(W2A(filename));
	image = imread(tempName);
	const String cascade_name = "haarcascade_frontalface_alt2.xml";
	if (!cascade.load(cascade_name))
	{
		MessageBox(_T("ERROR:Could not load cascade!"));
		return;
	}
	if (!image.data)
	{
		MessageBox(_T("ERROR:Could not load image!"));
		return;
	}
	namedWindow("人脸检测", CV_WINDOW_AUTOSIZE);
	detectAndDraw(image, cascade, scale);
	imshow("人脸检测", image);
	return;
}
void CMyFaceDetectDlg::detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale)
{
	cv::Mat imageRoi[2];

	int i = 0;
	vector<Rect>faces;
	const static Scalar colors[] = {
		CV_RGB(0, 0, 255),
		CV_RGB(0, 128, 255),
		CV_RGB(0, 255, 255),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 128, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(255, 0, 0),
		CV_RGB(255, 0, 255)
	};
	Mat gray, smallImage(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);
	cvtColor(img, gray, CV_BGR2GRAY);
	resize(gray, smallImage, smallImage.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImage, smallImage);
	cascade.detectMultiScale(smallImage, faces);
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		Point center;
		Scalar color = colors[i % 8];
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);
		circle(img, center, radius, color, 3);

	}
}

void CMyFaceDetectDlg::OnClickedFacev()
{
	// TODO:  在此添加控件通知处理程序代码
	CString filename;
	CFileDialog OpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("视频(*.avi)|*.avi|(*.*)|*.*|"), NULL);
	if (OpenDlg.DoModal() != IDOK)
	{
		return;
	}
	/*CString转换*string*/
	filename = OpenDlg.GetPathName();
	USES_CONVERSION;
	std::string tempName(W2A(filename));
	const String cascade_name = "haarcascade_frontalface_alt2.xml";
	if (!cascade.load(cascade_name))
	{
		MessageBox(_T("ERROR:Could not load cascade!"));
		return;
	}
	VideoCapture capture(tempName);
	if (!capture.isOpened())
	{
		MessageBox(_T("ERROR:Could not load Video!"));
		return;
	}
	double rate = capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	int delay = 1000 / rate;
	while (!stop)
	{
		if (!capture.read(image))
			break;
		detectAndDraw(image, cascade, scale);
		imshow("人脸检测", image);
		if (waitKey(delay) >= 0)
			stop = true;

	}
	capture.release();
	return;
}


void CMyFaceDetectDlg::OnClickedFacec()
{
	// TODO:  在此添加控件通知处理程序代码
	const String cascade_name = "haarcascade_frontalface_alt2.xml";
	if (!cascade.load(cascade_name))
	{
		MessageBox(_T("ERROR:Could not load cascade!"));
		return;
	}
	VideoCapture capture(0);
	if (!capture.isOpened())
	{
		MessageBox(_T("ERROR:Could not load capture!"));
		return;
	}
	double rate = capture.get(CV_CAP_PROP_FPS);
	//bool stop(false);
	int c=0;
	int delay = 1000 / rate;
	while (1)
	{
		if (!capture.read(image))
			break;
		detectAndDraw(image, cascade, scale);
		imshow("人脸检测", image);
		c = waitKey(33);
		if (c == 27)
			break;
			
	}
	capture.release();
	return;
}


void CMyFaceDetectDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
