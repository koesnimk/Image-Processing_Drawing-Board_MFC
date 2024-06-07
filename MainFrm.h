

#pragma once
// MainFrm.h: CMainFrame 클래스의 인터페이스
#include "CDataSocket.h"	
#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"
#include "IppImage\IppColor.h"
#include "IppImage\IppFilter.h"
#include "IppImage\IppFeature.h"
#include "IppImage\IppGeometry.h"
#include "IppImage\IppSegment.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
protected: // serialization에서만 만들어집니다.
	
	
	//CSplitterWnd m_wndSplitter; //정적분할 하기 위한 코드 비활성화
// 특성입니다.
public:
	//CDataSocket* m_pDataSocket;
	CMainFrame() noexcept;
	std::vector<CRect> m_labelRects;
	CArray<CRect> m_testRect;
	CString  m_cFilter;
// 작업입니다.
public:
	CDataSocket* m_pDataSocket; // 데이터 소켓 객체에 대한 포인터
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

	CMFCMenuBar m_wndMenuBar;
	
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	//정적분할 하기 위한 코드 비활성화
	//virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void OnUpdateFrameMenu(/*HMENU hMenuAlt*/BOOL bActive, CWnd* pActivateWnd, BOOL bActivate);
public:
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	//afx_msg void OnSend();
};


