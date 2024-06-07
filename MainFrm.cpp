
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "draw.h"

#include "MainFrm.h"
#include "drawDoc.h"
#include "drawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(IDC_CONNECT, &CMainFrame::OnConnect)
	ON_COMMAND(IDC_DISCONNECT, &CMainFrame::OnDisconnect)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()noexcept
{
	m_pDataSocket = NULL; // 초기화
}

CMainFrame::~CMainFrame()
{
	if (m_pDataSocket != NULL) {
		delete m_pDataSocket;
		m_pDataSocket = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;




	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	m_wndToolBar.SetButtonText(0, _T("새메모"));
	m_wndToolBar.SetButtonText(1, _T("열기"));
	m_wndToolBar.SetButtonText(2, _T("저장"));

	m_wndToolBar.SetButtonText(4, _T("네모1"));
	m_wndToolBar.SetButtonText(5, _T("네모2"));
	m_wndToolBar.SetButtonText(6, _T("원"));
	m_wndToolBar.SetButtonText(7, _T("세모"));
	m_wndToolBar.SetButtonText(8, _T("화살표"));

	m_wndToolBar.SetButtonText(10, _T("직선"));
	m_wndToolBar.SetButtonText(11, _T("자유선"));
	m_wndToolBar.SetButtonText(12, _T("지우개"));

	m_wndToolBar.SetButtonText(14, _T("1px"));
	m_wndToolBar.SetButtonText(15, _T("3px"));
	m_wndToolBar.SetButtonText(16, _T("5px"));
	m_wndToolBar.SetButtonText(17, _T("8px"));

	m_wndToolBar.SetButtonText(19, _T("빨강"));
	m_wndToolBar.SetButtonText(20, _T("노랑"));
	
	m_wndToolBar.SetButtonText(21, _T("초록"));
	m_wndToolBar.SetButtonText(22, _T("파랑"));
	m_wndToolBar.SetButtonText(23, _T("핑크"));
	m_wndToolBar.SetButtonText(24, _T("보라"));
	m_wndToolBar.SetButtonText(25, _T("브라운"));
	m_wndToolBar.SetButtonText(26, _T("그레이"));
	m_wndToolBar.SetButtonText(27, _T("검정"));

	m_wndToolBar.SetButtonText(29, _T("실선펜"));
	m_wndToolBar.SetButtonText(30, _T("대쉬펜"));
	m_wndToolBar.SetButtonText(31, _T("점선펜"));
	m_wndToolBar.SetButtonText(32, _T("대쉬점"));
	m_wndToolBar.SetSizes(CSize(60, 45), CSize(23, 23));
	//첫번째 사이즈:툴바 버튼의 크기
	//두번째 사이즈:비트맵 크기

	return 0;
}

// 예를 들어 A 뷰에서는 ID_A_MENU를, B 뷰에서는 ID_B_MENU를 사용하도록 할 때:
void CMainFrame::OnUpdateFrameMenu(/*HMENU hMenuAlt*/BOOL bActive, CWnd* pActivateWnd, BOOL bActivate)
{
	CMenu* pMenu = GetMenu();
	if (pMenu)
	{
		CMenu* pSubMenu = pMenu->GetSubMenu(0); // Assuming the menu you want to change is the first one
		if (pSubMenu)
		{
			pSubMenu->DestroyMenu();
			if (bActive && pActivateWnd == this)
				pSubMenu->LoadMenu(IDR_MAINFRAME); // Load the A menu resource
			else
				pSubMenu->LoadMenu(IDR_ImageToolTYPE); // Load the B menu resource
			SetMenu(pMenu);
		}
	}
}




BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = 1700;   //폭
	cs.cy = 1000;    //너비

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::OnConnect()
{
	if (m_pDataSocket == NULL) {
		m_pDataSocket = new CDataSocket(this);
		m_pDataSocket->Create();
		CString addr;
		if (m_pDataSocket->Connect(_T("127.0.0.1"), 8000)) {
			MessageBox(_T("서버에 접속 성공!"), _T("알림"), MB_ICONINFORMATION);

		}
		else {
			MessageBox(_T("서버에 접속 실패!"), _T("알림"), MB_ICONERROR);
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
	}
	else {
		MessageBox(_T("서버에 이미 접속됨!"), _T("알림"), MB_ICONINFORMATION);
	}

}


void CMainFrame::OnDisconnect()
{
	if (m_pDataSocket == NULL) {
		MessageBox(_T("서버에 접속 안함!"), _T("알림"), MB_ICONERROR);
	}
	else {
		CString sendStr;
		sendStr.Format(_T("클라이언트와 연결을 종료합니다."));

		m_pDataSocket->Send((LPCTSTR)sendStr, (sendStr.GetLength() + 1) * sizeof(TCHAR));
		MessageBox(_T("서버 접속 종료"), _T("알림"), MB_ICONERROR);
		m_pDataSocket->Close();
		delete m_pDataSocket;
		m_pDataSocket = NULL;
	}
}