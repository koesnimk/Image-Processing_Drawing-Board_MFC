#pragma once
#include <afxwin.h> // MFC core and standard components
#include <vector>   // std::vector를 사용하기 위한 헤더
#include <ctime>
#include <fstream>
#include <iostream>

class CDataSocket;
class CdrawView : public CScrollView
{
protected:
	CdrawView() noexcept;
	DECLARE_DYNCREATE(CdrawView)
	
public:
	int RW, RH;
	int w;
	int h;
	CdrawDoc* GetDocument() const;
	//CDataSocket* m_pDataSocket;

	struct MyShape
	{
		int shapeType; // 도형 모양		
		int penType; // 펜 타입		
		int penWidth; // 펜의 두깨
		CRect rect; // 도형 그리기 좌표
		COLORREF fgColor; // 전경색저장		
	};
	MyShape shape, shape2; // 도형 값을 저장하기 위한 구조체 변수 선언	
	// 그린 도형을 저장할 동적 배열 선언
	CArray<MyShape, MyShape&> data;

public:
	COLORREF m_Color; //색 정보
	int m_nThick; //두께 정보
	int m_nType; //도형 정보
	CPoint m_ptStart, m_ptEnd;   // (x1,y1)-(x2,y2) 좌표정보
	BOOL m_bDrawMode, flag_checkthick, flag_killallcolors, m_bLableCheck, m_bResizeImage;
	int m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos;
	int m_nPenType;

	BOOL m_bMouseDown; // 마우스 버튼이 눌렸는지 상태를 추적
	CPoint m_ptPrevious; // 마지막 마우스 위치 저장
	CDC m_memDC; // 메모리 DC를 위한 변수
	CBitmap m_bitmap; // 메모리 비트맵을 위한 변수


	CString m_cType, m_cThick, m_cColor, m_cPenType, m_cFilter;

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~CdrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawFreestyle();
	afx_msg void OnDrawErase();
	afx_msg void OnThick1px();
	afx_msg void OnThick3px();
	afx_msg void OnThick5px();
	afx_msg void OnThick8px();
	afx_msg void OnColorRed();
	afx_msg void OnColorYellow();
	afx_msg void OnColorLightgreen();
	afx_msg void OnColorGreen();
	afx_msg void OnColorSky();
	afx_msg void OnColorBlue();
	afx_msg void OnColorNavy();
	afx_msg void OnColorPink();
	afx_msg void OnColorPurple();
	afx_msg void OnColorBrown();
	afx_msg void OnColorGrey();
	afx_msg void OnColorBlack();
	afx_msg void OnBrushSolid();
	afx_msg void OnBrushDash();
	afx_msg void OnBrushDot();
	afx_msg void OnBrushDashdot();
	afx_msg void OnDrawRoundrect();
	afx_msg void OnDrawTriangle();
	afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawFreestyle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawErase(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThick1px(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThick3px(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThick5px(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThick8px(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorRed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorYellow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorLightgreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorGreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorSky(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorBlue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorNavy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorPink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorPurple(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorBrown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorGrey(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorBlack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushSolid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushDash(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushDot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushDashdot(CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateDrawRoundrect(CCmdUI* pCmdUI);
	
	afx_msg void Draw_Triangle(CDC* pDC, int s_xpos, int s_ypos, int e_xpos, int e_ypos);
	void Draw_Arrow(CDC* pDC, int m_nSXpos, int m_nSYpos, int m_nEXpos, int m_nEYpos);
	afx_msg void OnDrawArrow();
	afx_msg void OnColorOrange();
	afx_msg void OnColorGold1();
	afx_msg void OnColorLightpink();
	afx_msg void OnColorLightgrey();
	afx_msg void OnColorLightbrown();
	virtual void OnInitialUpdate();
	void SetScrollSizeToFit(void);
	int m_nZoom;
	//afx_msg void OnConnect();
	//afx_msg void OnDisconnect();
	afx_msg void OnSend();
	void drawShape(int shapeNum, int penTy, int penWd, int sx, int sy, int ex, int ey, COLORREF fgcolor);

	afx_msg void OnTake();
	afx_msg void OnSegmentLabeling();
	afx_msg void OnAllDelete();
	afx_msg void OnResizeImage();
};

#ifndef _DEBUG
inline CdrawDoc* CdrawView::GetDocument() const
   { return reinterpret_cast<CdrawDoc*>(m_pDocument); }
#endif

