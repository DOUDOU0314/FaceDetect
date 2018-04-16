
// MyFaceDetectDlg.h : 头文件
//

#pragma once

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\ml\ml.hpp>
#include <opencv.hpp>
#include "afxwin.h"
using namespace cv;

// CMyFaceDetectDlg 对话框
class CMyFaceDetectDlg : public CDialogEx
{
// 构造
public:
	CMyFaceDetectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYFACEDETECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HICON m_catIcon;

	// 生成的消息映射函数
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
