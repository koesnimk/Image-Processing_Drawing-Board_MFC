// CResizeImage.cpp: 구현 파일
//

#include "pch.h"
#include "draw.h"
#include "afxdialogex.h"
#include "CResizeImage.h"


// CResizeImage 대화 상자

IMPLEMENT_DYNAMIC(CResizeImage, CDialogEx)

CResizeImage::CResizeImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESIZE_IMAGE, pParent)
	, m_nResizeImage(1)
{

}

CResizeImage::~CResizeImage()
{
}

void CResizeImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESIZE_SLIDER, m_sliderResizeImage);
	DDX_Text(pDX, IDC_RESIZE_EDIT, m_nResizeImage);
	DDV_MinMaxInt(pDX, m_nResizeImage, -3, 3);
}


BEGIN_MESSAGE_MAP(CResizeImage, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_RESIZE_EDIT, &CResizeImage::OnEnChangeResizeEdit)
END_MESSAGE_MAP()


// CResizeImage 메시지 처리기


BOOL CResizeImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderResizeImage.SetRange(-3, 3);
	m_sliderResizeImage.SetTicFreq(1);
	m_sliderResizeImage.SetPageSize(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CResizeImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_sliderResizeImage.GetSafeHwnd() == pScrollBar->GetSafeHwnd()) {
		m_nResizeImage = m_sliderResizeImage.GetPos();
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CResizeImage::OnEnChangeResizeEdit()
{
	UpdateData(TRUE);
	m_sliderResizeImage.SetPos(m_nResizeImage);
}
