// CColorCombineDlg.cpp: 구현 파일
//

#include "pch.h"
#include "draw.h"
#include "afxdialogex.h"
#include "CColorCombineDlg.h"


// CColorCombineDlg 대화 상자

IMPLEMENT_DYNAMIC(CColorCombineDlg, CDialogEx)

CColorCombineDlg::CColorCombineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLOR_COMBINE, pParent)
	, m_strColorSpace(_T(""))
{

}

CColorCombineDlg::~CColorCombineDlg()
{
}

void CColorCombineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COLOR_SPACE, m_strColorSpace);
	DDX_Control(pDX, IDC_COMBO_IMAGE1, m_comboImage1);
	DDX_Control(pDX, IDC_COMBO_IMAGE2, m_comboImage2);
	DDX_Control(pDX, IDC_COMBO_IMAGE3, m_comboImage3);
}


BEGIN_MESSAGE_MAP(CColorCombineDlg, CDialogEx)
END_MESSAGE_MAP()


// CColorCombineDlg 메시지 처리기


BOOL CColorCombineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
