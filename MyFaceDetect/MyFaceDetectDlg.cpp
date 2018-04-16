
// MyFaceDetectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyFaceDetect.h"
#include "MyFaceDetectDlg.h"
#include "afxdialogex.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyFaceDetectDlg �Ի���



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


// CMyFaceDetectDlg ��Ϣ�������

BOOL CMyFaceDetectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_catIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_catIcon, FALSE);		// ����Сͼ��
	//��ť����ͼƬ����
	//HBITMAP hbmp1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2));
	//m_btn.SetBitmap(hbmp1);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyFaceDetectDlg::OnPaint()
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
		dc.DrawIcon(x, y, m_catIcon);
	}
	else
	{
		/*�ı�Ի��򱳾�*/
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyFaceDetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_catIcon);
}



void CMyFaceDetectDlg::OnClickedFacedetect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	CFileDialog OpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("ͼƬ (*.jpg)|*.jpg|(*.*) |*.*|"), NULL);
	if (OpenDlg.DoModal() != IDOK)
	{
		return;
	}
	/*CStringת��*string*/
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
	namedWindow("�������", CV_WINDOW_AUTOSIZE);
	detectAndDraw(image, cascade, scale);
	imshow("�������", image);
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	CFileDialog OpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("��Ƶ(*.avi)|*.avi|(*.*)|*.*|"), NULL);
	if (OpenDlg.DoModal() != IDOK)
	{
		return;
	}
	/*CStringת��*string*/
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
		imshow("�������", image);
		if (waitKey(delay) >= 0)
			stop = true;

	}
	capture.release();
	return;
}


void CMyFaceDetectDlg::OnClickedFacec()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		imshow("�������", image);
		c = waitKey(33);
		if (c == 27)
			break;
			
	}
	capture.release();
	return;
}


void CMyFaceDetectDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
