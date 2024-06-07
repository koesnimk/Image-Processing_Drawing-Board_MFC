#pragma once
#include "afxdialogex.h"


// CResizeImage 대화 상자

class CResizeImage : public CDialogEx
{
	DECLARE_DYNAMIC(CResizeImage)

public:
	CResizeImage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CResizeImage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESIZE_IMAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderResizeImage;
	int m_nResizeImage;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeResizeEdit();
};
