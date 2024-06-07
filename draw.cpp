
// draw.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "draw.h"
#include "MainFrm.h"

#include "drawDoc.h"
#include "drawView.h"

#include "ChildFrame.h"
#include "CDataSocket.h"
#include <atlimage.h>
#include ".\IppImage\IppDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdrawApp

BEGIN_MESSAGE_MAP(CdrawApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CdrawApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_WINDOW_CLOSEALL, &CdrawApp::OnWindowCloseall)
END_MESSAGE_MAP()


// CdrawApp 생성

CdrawApp::CdrawApp() noexcept
	: m_pNewDib(NULL), m_pImageDocTemplate(NULL), m_pAviDocTemplate(NULL)
{
	//m_pGlobalDataSocket = NULL;
	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("draw.AppID.NoVersion"));

	
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CdrawApp 개체입니다.

CdrawApp theApp;


// CdrawApp 초기화

BOOL CdrawApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("연결 실패"));
		return FALSE;
	}
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(


	//CSingleDocTemplate* pDocTemplate;
	//pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CdrawDoc),
		RUNTIME_CLASS(ChildFrame), //특정행동 시 메뉴가 바뀌는 부분
		RUNTIME_CLASS(CdrawView));

	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	m_pMainWnd->SetWindowText(_T("그림판 & 영상처리"));
	return TRUE;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CdrawApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CdrawApp 메시지 처리기

void AfxNewBitmap(IppDib& dib)
{
	theApp.m_pNewDib = &dib;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

bool CdrawApp::ConnectServer()
{
	return TRUE;
}
void CdrawApp::DisconnectServer()
{
}

void CdrawApp::OnWindowCloseall()
{
	CloseAllDocuments(TRUE);
}
