
// MyFaceDetectDlg.h : ͷ�ļ�
//

#pragma once

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\ml\ml.hpp>
#include <opencv.hpp>
#include "afxwin.h"
using namespace cv;

// CMyFaceDetectDlg �Ի���
class CMyFaceDetectDlg : public CDialogEx
{
// ����
public:
	CMyFaceDetectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYFACEDETECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HICON m_catIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedFacedetect();

public:
	CascadeClassifier cascade;
	Mat image;
	double scale;
public:
	void detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale);

	CButton m_btn;
	afx_msg void OnClickedFacev();
	afx_msg void OnClickedFacec();
	afx_msg void OnBnClickedCancel();
};
