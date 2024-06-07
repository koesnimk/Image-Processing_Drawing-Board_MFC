#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "draw.h"
#endif
#include "MainFrm.h"
#include "drawDoc.h"
#include "drawView.h"
#include "CDataSocket.h"
#include <atlimage.h>
#include "CResizeImage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 데이터를 저장할 구조체 정의

#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	IppByteImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_DIB_TO_RGBIMAGE(m_Dib, img) \
	IppRgbImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img, dib) \
	IppDib dib; \
	IppImageToDib(img, dib);
IMPLEMENT_DYNCREATE(CdrawView, CScrollView)

BEGIN_MESSAGE_MAP(CdrawView, CScrollView)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_Draw_Rectangle, &CdrawView::OnDrawRectangle)
	ON_COMMAND(ID_Draw_Ellipse, &CdrawView::OnDrawEllipse)
	ON_COMMAND(ID_Draw_Line, &CdrawView::OnDrawLine)
	ON_COMMAND(ID_Draw_FreeStyle, &CdrawView::OnDrawFreestyle)
	ON_COMMAND(ID_Draw_Erase, &CdrawView::OnDrawErase)
	ON_COMMAND(ID_Thick_1px, &CdrawView::OnThick1px)
	ON_COMMAND(ID_Thick_3px, &CdrawView::OnThick3px)
	ON_COMMAND(ID_Thick_5px, &CdrawView::OnThick5px)
	ON_COMMAND(ID_Thick_8px, &CdrawView::OnThick8px)
	ON_COMMAND(ID_Color_Red, &CdrawView::OnColorRed)
	ON_COMMAND(ID_Color_Yellow, &CdrawView::OnColorYellow)
	ON_COMMAND(ID_Color_LightGreen, &CdrawView::OnColorLightgreen)
	ON_COMMAND(ID_Color_Green, &CdrawView::OnColorGreen)
	ON_COMMAND(ID_Color_Sky, &CdrawView::OnColorSky)
	ON_COMMAND(ID_Color_Blue, &CdrawView::OnColorBlue)
	ON_COMMAND(ID_Color_Navy, &CdrawView::OnColorNavy)
	ON_COMMAND(ID_Color_Pink, &CdrawView::OnColorPink)
	ON_COMMAND(ID_Color_Purple, &CdrawView::OnColorPurple)
	ON_COMMAND(ID_Color_Brown, &CdrawView::OnColorBrown)
	ON_COMMAND(ID_Color_Grey, &CdrawView::OnColorGrey)
	ON_COMMAND(ID_Color_Black, &CdrawView::OnColorBlack)
	ON_COMMAND(ID_Brush_Solid, &CdrawView::OnBrushSolid)
	ON_COMMAND(ID_Brush_Dash, &CdrawView::OnBrushDash)
	ON_COMMAND(ID_Brush_Dot, &CdrawView::OnBrushDot)
	ON_COMMAND(ID_Brush_Dashdot, &CdrawView::OnBrushDashdot)
	ON_UPDATE_COMMAND_UI(ID_Draw_Rectangle, &CdrawView::OnUpdateDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_Draw_Ellipse, &CdrawView::OnUpdateDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_Draw_Line, &CdrawView::OnUpdateDrawLine)
	ON_UPDATE_COMMAND_UI(ID_Draw_FreeStyle, &CdrawView::OnUpdateDrawFreestyle)
	ON_UPDATE_COMMAND_UI(ID_Draw_Erase, &CdrawView::OnUpdateDrawErase)
	ON_UPDATE_COMMAND_UI(ID_Thick_1px, &CdrawView::OnUpdateThick1px)
	ON_UPDATE_COMMAND_UI(ID_Thick_3px, &CdrawView::OnUpdateThick3px)
	ON_UPDATE_COMMAND_UI(ID_Thick_5px, &CdrawView::OnUpdateThick5px)
	ON_UPDATE_COMMAND_UI(ID_Thick_8px, &CdrawView::OnUpdateThick8px)
	ON_UPDATE_COMMAND_UI(ID_Color_Red, &CdrawView::OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_Color_Yellow, &CdrawView::OnUpdateColorYellow)
	ON_UPDATE_COMMAND_UI(ID_Color_LightGreen, &CdrawView::OnUpdateColorLightgreen)
	ON_UPDATE_COMMAND_UI(ID_Color_Green, &CdrawView::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_Color_Sky, &CdrawView::OnUpdateColorSky)
	ON_UPDATE_COMMAND_UI(ID_Color_Blue, &CdrawView::OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_Color_Navy, &CdrawView::OnUpdateColorNavy)
	ON_UPDATE_COMMAND_UI(ID_Color_Pink, &CdrawView::OnUpdateColorPink)
	ON_UPDATE_COMMAND_UI(ID_Color_Purple, &CdrawView::OnUpdateColorPurple)
	ON_UPDATE_COMMAND_UI(ID_Color_Brown, &CdrawView::OnUpdateColorBrown)
	ON_UPDATE_COMMAND_UI(ID_Color_Grey, &CdrawView::OnUpdateColorGrey)
	ON_UPDATE_COMMAND_UI(ID_Color_Black, &CdrawView::OnUpdateColorBlack)
	ON_UPDATE_COMMAND_UI(ID_Brush_Solid, &CdrawView::OnUpdateBrushSolid)
	ON_UPDATE_COMMAND_UI(ID_Brush_Dash, &CdrawView::OnUpdateBrushDash)
	ON_UPDATE_COMMAND_UI(ID_Brush_Dot, &CdrawView::OnUpdateBrushDot)
	ON_UPDATE_COMMAND_UI(ID_Brush_Dashdot, &CdrawView::OnUpdateBrushDashdot)
	ON_COMMAND(ID_Draw_RoundRect, &CdrawView::OnDrawRoundrect)
	ON_UPDATE_COMMAND_UI(ID_Draw_RoundRect, &CdrawView::OnUpdateDrawRoundrect)
	ON_COMMAND(ID_Draw_Triangle, &CdrawView::OnDrawTriangle)
	ON_COMMAND(ID_Draw_Arrow, &CdrawView::OnDrawArrow)
	ON_COMMAND(ID_Color_Orange, &CdrawView::OnColorOrange)
	ON_COMMAND(ID_Color_Gold1, &CdrawView::OnColorGold1)
	ON_COMMAND(ID_Color_LightPink, &CdrawView::OnColorLightpink)
	ON_COMMAND(ID_Color_LightGrey, &CdrawView::OnColorLightgrey)
	ON_COMMAND(ID_Color_LightBrown, &CdrawView::OnColorLightbrown)
	//ON_COMMAND(IDC_CONNECT, &CdrawView::OnConnect)
	//ON_COMMAND(IDC_DISCONNECT, &CdrawView::OnDisconnect)
	ON_COMMAND(IDC_SEND, &CdrawView::OnSend)
	ON_WM_PAINT()
	ON_COMMAND(ID_TAKE, &CdrawView::OnTake)
	ON_COMMAND(ID_SEGMENT_LABELING, &CdrawView::OnSegmentLabeling)
	ON_COMMAND(ID_ALL_DELETE, &CdrawView::OnAllDelete)
	ON_COMMAND(ID_RESIZE_IMAGE, &CdrawView::OnResizeImage)
END_MESSAGE_MAP()

CdrawView::CdrawView() noexcept
	: m_nZoom(1)
{
	m_Color = RGB(0, 0, 0);
	m_nThick = 1;
	m_nType = 0;
	m_bDrawMode = flag_checkthick = flag_killallcolors = m_bLableCheck = m_bResizeImage = FALSE;
	m_ptStart = m_ptEnd = CPoint(0, 0);
	m_nSXpos = m_nSYpos = m_nEXpos = m_nEYpos = m_nPenType = 0;
	m_cType = m_cThick = m_cColor = m_cPenType =m_cFilter = _T(" ");
	RH = RW = 1;
}
CdrawView::~CdrawView()
{
}

BOOL CdrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CdrawView::OnDraw(CDC* pDC)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CdrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	w = pDoc->m_Dib.GetWidth();
	h = pDoc->m_Dib.GetHeight();

	
	if (pDoc->m_Dib.IsValid()) {
		
		if (RH < 0 && RW < 0) {
			int newRH = RH * -1;
			int newRW = RW * -1;
			pDoc->m_Dib.Draw(pDC->m_hDC, 0, 0, w /= newRW, h /= newRH);
		}
		else if (RH > 0 && RW > 0) {
			pDoc->m_Dib.Draw(pDC->m_hDC, 0, 0, w *= RW, h *= RH);
		
		}
		else {
			pDoc->m_Dib.Draw(pDC->m_hDC, 0, 0, w, h);
		}
	}
	
	for (int i = 0; i < data.GetSize(); i++)
	{
		drawShape(data[i].shapeType, data[i].penType, data[i].penWidth, data[i].rect.left, data[i].rect.top, data[i].rect.right, data[i].rect.bottom, data[i].fgColor);
	}
	for (const CRect& rect : pFrame->m_labelRects)
	{
		CPen pen(PS_SOLID, 4, RGB(255, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		pDC->SelectObject(pOldPen);
	}
}


BOOL CdrawView::OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
void CdrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {}
void CdrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {}
#ifdef _DEBUG
void CdrawView::AssertValid() const { CScrollView::AssertValid(); }
void CdrawView::Dump(CDumpContext& dc) const { CScrollView::Dump(dc); }
CdrawDoc* CdrawView::GetDocument() const {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdrawDoc)));
	return (CdrawDoc*)m_pDocument;
}
#endif


//작동부
void CdrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDrawMode = TRUE;
	m_nSXpos = m_nEXpos = point.x;
	m_nSYpos = m_nEYpos = point.y;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CdrawView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	CPen pen, * poldpen, erase, * p_erase;
	pen.CreatePen(m_nPenType, m_nThick, m_Color);
	erase.CreatePen(PS_SOLID, m_nThick * 2.5, RGB(255, 255, 255)); //지우개와 같은 역할하는 흰색 펜 생성
	poldpen = dc.SelectObject(&pen);

	if (m_bDrawMode) {
		switch (m_nType) {
		case 0: //네모
			dc.SelectStockObject(NULL_BRUSH);
			dc.SetROP2(R2_NOTXORPEN);
			dc.Rectangle(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SetROP2(R2_NOTXORPEN);
			m_nEXpos = point.x;
			m_nEYpos = point.y;
			dc.Rectangle(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SelectObject(poldpen);
			break;

		case 1: //원
			dc.SelectStockObject(NULL_BRUSH);
			dc.SetROP2(R2_NOTXORPEN);
			dc.Ellipse(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SetROP2(R2_NOTXORPEN);
			m_nEXpos = point.x;
			m_nEYpos = point.y;
			dc.Ellipse(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SelectObject(poldpen);
			break;

		case 2: //선
			dc.SetROP2(R2_NOTXORPEN);
			dc.MoveTo(m_nSXpos, m_nSYpos);
			dc.LineTo(m_nEXpos, m_nEYpos);
			dc.SetROP2(R2_NOTXORPEN);
			m_nEXpos = point.x;
			m_nEYpos = point.y;
			dc.MoveTo(m_nSXpos, m_nSYpos);
			dc.LineTo(m_nEXpos, m_nEYpos);
			dc.SelectObject(poldpen);
			break;

		case 3: //자유선
			shape.rect.left = m_nSXpos; // 도형 시작좌표 저장
			shape.rect.top = m_nSYpos;

			dc.MoveTo(m_nSXpos, m_nSYpos);
			dc.LineTo(point.x, point.y);
			dc.SelectObject(poldpen);
			m_nSXpos = m_nEXpos = point.x;
			m_nSYpos = m_nEYpos = point.y;

			shape.shapeType = m_nType;
			shape.penType = m_nPenType;
			shape.penWidth = m_nThick;
			shape.fgColor = m_Color; // 글자색 저장
			shape.rect.right = m_nEXpos; // 도형 끝 좌표 저장
			shape.rect.bottom = m_nEYpos;

			data.Add(shape);
			break;
		case 4: //지우개
			p_erase = dc.SelectObject(&erase);

			shape.rect.left = m_nSXpos; // 도형 시작좌표 저장
			shape.rect.top = m_nSYpos;

			dc.MoveTo(m_nSXpos, m_nSYpos);
			dc.LineTo(point.x, point.y);
			dc.SelectObject(p_erase);
			m_nSXpos = m_nEXpos = point.x;
			m_nSYpos = m_nEYpos = point.y;

			shape.shapeType = m_nType;
			shape.penType = m_nPenType;
			shape.penWidth = m_nThick;
			shape.fgColor = m_Color; // 글자색 저장
			shape.rect.right = m_nEXpos; // 도형 끝 좌표 저장
			shape.rect.bottom = m_nEYpos;

			data.Add(shape);
			break;
		case 5: //둥근 네모
			dc.SelectStockObject(NULL_BRUSH);
			dc.SetROP2(R2_NOTXORPEN);
			dc.RoundRect(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos, 70, 70);
			dc.SetROP2(R2_NOTXORPEN);
			m_nEXpos = point.x;
			m_nEYpos = point.y;
			dc.RoundRect(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos, 70, 70);
			dc.SelectObject(poldpen);
			break;
		case 6: //세모
			dc.SetROP2(R2_NOTXORPEN);
			Draw_Triangle(&dc, m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SetROP2(R2_NOTXORPEN);
			m_nEXpos = point.x;
			m_nEYpos = point.y;
			Draw_Triangle(&dc, m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SelectObject(poldpen);
			break;
		case 7: //세모
			dc.SetROP2(R2_NOTXORPEN);
			Draw_Arrow(&dc, m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SetROP2(R2_NOTXORPEN);
			m_nEXpos = point.x;
			m_nEYpos = point.y;
			Draw_Arrow(&dc, m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
			dc.SelectObject(poldpen);
			break;
		default:
			break;
		}

		pen.DeleteObject();
		erase.DeleteObject();
	}
	CScrollView::OnMouseMove(nFlags, point);
}


void CdrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	CPen pen, * poldpen;
	pen.CreatePen(m_nPenType, m_nThick, m_Color);
	poldpen = dc.SelectObject(&pen);

	switch (m_nType) {
	case 0: //네모
		dc.SelectStockObject(NULL_BRUSH);
		m_nEXpos = point.x;
		m_nEYpos = point.y;
		dc.Rectangle(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
		dc.SelectObject(poldpen);
		break;
	case 1: //원
		dc.SelectStockObject(NULL_BRUSH);
		m_nEXpos = point.x;
		m_nEYpos = point.y;
		dc.Ellipse(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
		dc.SelectObject(poldpen);
		break;
	case 2: //선
		m_nEXpos = point.x;
		m_nEYpos = point.y;
		dc.MoveTo(m_nSXpos, m_nSYpos);
		dc.LineTo(m_nEXpos, m_nEYpos);
		dc.SelectObject(poldpen);
		break;
	case 5: //둥근네모
		dc.SelectStockObject(NULL_BRUSH);
		m_nEXpos = point.x;
		m_nEYpos = point.y;
		dc.RoundRect(m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos, 70, 70);
		dc.SelectObject(poldpen);
		break;
	case 6: //세모
		m_nEXpos = point.x;
		m_nEYpos = point.y;
		Draw_Triangle(&dc, m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
		dc.SelectObject(poldpen);
		break;
	case 7: //화살
		m_nEXpos = point.x;
		m_nEYpos = point.y;
		Draw_Arrow(&dc, m_nSXpos, m_nSYpos, m_nEXpos, m_nEYpos);
		dc.SelectObject(poldpen);
		break;

	default:
		break;
	}
	pen.DeleteObject();
	if (m_bDrawMode) {
		m_nEXpos = point.x;
		m_nEYpos = point.y;

		shape.shapeType = m_nType;
		shape.penType = m_nPenType;
		shape.penWidth = m_nThick;
		shape.fgColor = m_Color; // 글자색 저장	
		shape.rect.left = m_nSXpos; // 도형 시작좌표 저장
		shape.rect.top = m_nSYpos;
		shape.rect.right = m_nEXpos; // 도형 끝 좌표 저장
		shape.rect.bottom = m_nEYpos;

		data.Add(shape);
	}
	m_bDrawMode = FALSE;
	CScrollView::OnLButtonUp(nFlags, point);
}

//도형 메뉴
void CdrawView::OnDrawRectangle()
{
	m_nType = 0;
	flag_killallcolors = FALSE;
	m_cType = _T("사각형");
}
void CdrawView::OnDrawEllipse()
{
	m_nType = 1;
	flag_killallcolors = FALSE;
	m_cType = _T("원");
}
void CdrawView::OnDrawRoundrect()
{
	m_nType = 5;
	flag_killallcolors = FALSE;
	m_cType = _T("둥근 사각형");
}
void CdrawView::OnDrawTriangle()
{
	m_nType = 6;
	flag_killallcolors = FALSE;
	m_cType = _T("세모");
}
void CdrawView::OnDrawArrow()
{
	m_nType = 7;
	flag_killallcolors = FALSE;
	m_cType = _T("화살표");
}

//도구 메뉴
void CdrawView::OnDrawLine()
{
	m_nType = 2;
	flag_killallcolors = FALSE;
	m_cType = _T("직선");
}
void CdrawView::OnDrawFreestyle()
{
	m_nType = 3;
	flag_killallcolors = FALSE;
	flag_checkthick = TRUE;
	if (m_nPenType != PS_SOLID) m_nPenType = PS_SOLID;
	m_cType = _T("자유선");
}
void CdrawView::OnDrawErase()
{
	m_nType = 4;
	m_Color = RGB(0, 0, 0);
	flag_checkthick = flag_killallcolors = TRUE;
	if (m_nPenType != PS_SOLID) m_nPenType = PS_SOLID;
	m_cType = _T("지우개");
}

//굵기 메뉴
void CdrawView::OnThick1px()
{
	m_nThick = 1;
	flag_checkthick = FALSE;
	m_cThick = _T("1픽셀");
}
void CdrawView::OnThick3px()
{
	m_nThick = 3;
	flag_checkthick = TRUE;
	if (m_nPenType != PS_SOLID) m_nPenType = PS_SOLID;
	m_cThick = _T("3픽셀");
}
void CdrawView::OnThick5px()
{
	m_nThick = 5;
	flag_checkthick = TRUE;
	if (m_nPenType != PS_SOLID) m_nPenType = PS_SOLID;
	m_cThick = _T("5픽셀");
}
void CdrawView::OnThick8px()
{
	m_nThick = 8;
	flag_checkthick = TRUE;
	if (m_nPenType != PS_SOLID) m_nPenType = PS_SOLID;
	m_cThick = _T("8픽셀");
}

//색깔 메뉴
void CdrawView::OnColorRed() {
	m_Color = RGB(255, 0, 0);
	m_cColor = _T("빨간색");
}
void CdrawView::OnColorYellow() {
	m_Color = RGB(255, 255, 0);
	m_cColor = _T("노란색");
}
void CdrawView::OnColorLightgreen() {
	m_Color = RGB(50, 255, 50);
	m_cColor = _T("연두색");
}
void CdrawView::OnColorGreen() {
	m_Color = RGB(0, 255, 0);
	m_cColor = _T("초록색");
}
void CdrawView::OnColorSky() {
	m_Color = RGB(115, 255, 255);
	m_cColor = _T("하늘색");
}
void CdrawView::OnColorBlue() {
	m_Color = RGB(0, 0, 255);
	m_cColor = _T("파란색");
}
void CdrawView::OnColorNavy() {
	m_Color = RGB(0, 15, 125);
	m_cColor = _T("남색");
}
void CdrawView::OnColorPink() {
	m_Color = RGB(255, 90, 230);
	m_cColor = _T("핑크색");
}
void CdrawView::OnColorPurple() {
	m_Color = RGB(125, 50, 100);
	m_cColor = _T("보라색");
}
void CdrawView::OnColorBrown() {
	m_Color = RGB(142, 64, 58);
	m_cColor = _T("갈색");
}
void CdrawView::OnColorGrey() {
	m_Color = RGB(128, 128, 128);
	m_cColor = _T("회색");
}
void CdrawView::OnColorBlack() {
	m_Color = RGB(0, 0, 0);
	m_cColor = _T("검은색");
}
void CdrawView::OnColorOrange() {
	m_Color = RGB(240, 106, 47);
	m_cColor = _T("주황색");
}
void CdrawView::OnColorGold1() {
	m_Color = RGB(191, 155, 48);
	m_cColor = _T("금색");
}
void CdrawView::OnColorLightpink() {
	m_Color = RGB(255, 192, 203);
	m_cColor = _T("연한 핑크색");
}
void CdrawView::OnColorLightgrey() {
	m_Color = RGB(192, 192, 192);
	m_cColor = _T("연한 회색");
}
void CdrawView::OnColorLightbrown() {
	m_Color = RGB(141, 111, 90);
	m_cColor = _T("연한 갈색");
}

//브러쉬
void CdrawView::OnBrushSolid() {
	m_nPenType = PS_SOLID;
	m_cPenType = _T("실선");
}
void CdrawView::OnBrushDash() {
	m_nPenType = PS_DASH;
	m_cPenType = _T("대쉬선");
}
void CdrawView::OnBrushDot() {
	m_nPenType = PS_DOT;
	m_cPenType = _T("점선");
}
void CdrawView::OnBrushDashdot() {
	m_nPenType = PS_DASHDOT;
	m_cPenType = _T("대쉬점선");
}

// 메뉴 & 툴바 체킹 ----------------------------------------
// 타입 확인 ----------------------------------------------
void CdrawView::OnUpdateDrawRectangle(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nType == 0);
}
void CdrawView::OnUpdateDrawEllipse(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nType == 1);
}
void CdrawView::OnUpdateDrawRoundrect(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nType == 5);
}
void CdrawView::OnUpdateDrawLine(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nType == 2);
}
void CdrawView::OnUpdateDrawFreestyle(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nType == 3);
}
void CdrawView::OnUpdateDrawErase(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nType == 4);
}
// 굵기 확인 ----------------------------------------------
void CdrawView::OnUpdateThick1px(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nThick == 1);
}
void CdrawView::OnUpdateThick3px(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nThick == 3);
}
void CdrawView::OnUpdateThick5px(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nThick == 5);
}
void CdrawView::OnUpdateThick8px(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nThick == 8);
}
// 색상 확인 ----------------------------------------------
void CdrawView::OnUpdateColorRed(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(255, 0, 0));
}
void CdrawView::OnUpdateColorYellow(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(255, 255, 0));
}
void CdrawView::OnUpdateColorLightgreen(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(50, 255, 50));
}
void CdrawView::OnUpdateColorGreen(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(0, 255, 0));
}
void CdrawView::OnUpdateColorSky(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(115, 255, 255));
}
void CdrawView::OnUpdateColorBlue(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(0, 0, 255));
}
void CdrawView::OnUpdateColorNavy(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(0, 15, 125));
}
void CdrawView::OnUpdateColorPink(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(255, 90, 230));
}
void CdrawView::OnUpdateColorPurple(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(125, 50, 100));
}
void CdrawView::OnUpdateColorBrown(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(142, 64, 58));
}
void CdrawView::OnUpdateColorGrey(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(128, 128, 128));
}
void CdrawView::OnUpdateColorBlack(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_killallcolors);
	pCmdUI->SetCheck(m_Color == RGB(0, 0, 0));
}
// 펜 타입 확인 -------------------------------------------
void CdrawView::OnUpdateBrushSolid(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nPenType == PS_SOLID);
}
void CdrawView::OnUpdateBrushDash(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_checkthick);
	pCmdUI->SetCheck(m_nPenType == PS_DASH);
}
void CdrawView::OnUpdateBrushDot(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_checkthick);
	pCmdUI->SetCheck(m_nPenType == PS_DOT);
}
void CdrawView::OnUpdateBrushDashdot(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!flag_checkthick);
	pCmdUI->SetCheck(m_nPenType == PS_DASHDOT);
}

//세모를 그리기 위해 선언한 함수
void CdrawView::Draw_Triangle(CDC* pDC, int m_nSXpos, int m_nSYpos, int m_nEXpos, int m_nEYpos)
{
	pDC->MoveTo(m_nSXpos, m_nSYpos); //우측선
	pDC->LineTo(m_nSXpos + (m_nEXpos - m_nSXpos), m_nSYpos + (m_nEYpos - m_nSYpos));

	pDC->MoveTo(m_nSXpos, m_nSYpos); //왼쪽선
	pDC->LineTo(m_nSXpos - (m_nEXpos - m_nSXpos), m_nSYpos + (m_nEYpos - m_nSYpos));

	pDC->MoveTo(m_nSXpos - (m_nEXpos - m_nSXpos), m_nSYpos + (m_nEYpos - m_nSYpos)); //아랫선
	pDC->LineTo(m_nSXpos + (m_nEXpos - m_nSXpos), m_nSYpos + (m_nEYpos - m_nSYpos));
}
//화살표를 그리기 위해 선언한 함수
void CdrawView::Draw_Arrow(CDC* pDC, int m_nSXpos, int m_nSYpos, int m_nEXpos, int m_nEYpos)
{
	if (m_nSXpos == m_nEXpos && m_nSYpos == m_nEYpos) // 시작점과 끝점이 동일한 경우 선을 그리지 않음
		return;

	pDC->MoveTo(m_nSXpos, (m_nSYpos + ((m_nEYpos - m_nSYpos) * 1 / 3))); //직사각형 부분 윗변
	pDC->LineTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), (m_nSYpos + ((m_nEYpos - m_nSYpos) * 1 / 3)));

	pDC->MoveTo(m_nSXpos, (m_nSYpos + ((m_nEYpos - m_nSYpos) * 1 / 3))); //직사각형 부분 옆부분
	pDC->LineTo(m_nSXpos, (m_nSYpos + ((m_nEYpos - m_nSYpos) * 2 / 3)));

	pDC->MoveTo(m_nSXpos, (m_nSYpos + ((m_nEYpos - m_nSYpos) * 2 / 3))); //직사각형 부분 아랫변
	pDC->LineTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), (m_nSYpos + ((m_nEYpos - m_nSYpos) * 2 / 3)));

	pDC->MoveTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), (m_nSYpos + ((m_nEYpos - m_nSYpos) * 1 / 3)));
	pDC->LineTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), m_nSYpos);

	pDC->MoveTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), (m_nSYpos + ((m_nEYpos - m_nSYpos) * 2 / 3)));
	pDC->LineTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), m_nEYpos);

	pDC->MoveTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), (m_nSYpos + (m_nEYpos - m_nSYpos))); //세모부분 윗변
	pDC->LineTo((m_nSXpos + (m_nEXpos - m_nSXpos)), (m_nSYpos + ((m_nEYpos - m_nSYpos) / 2)));

	pDC->MoveTo((m_nSXpos + ((m_nEXpos - m_nSXpos) / 2)), m_nSYpos); //세모 부분 아랫변
	pDC->LineTo((m_nSXpos + (m_nEXpos - m_nSXpos)), (m_nSYpos + ((m_nEYpos - m_nSYpos) / 2)));
}

void CdrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	//m_pDataSocket = NULL;
	SetScrollSizeToFit();
}


void CdrawView::SetScrollSizeToFit(void)
{
	CSize sizeTotal;

	CdrawDoc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid())
	{
		int w = pDoc->m_Dib.GetWidth();
		int h = pDoc->m_Dib.GetHeight();

		sizeTotal.cx = w/2 * m_nZoom;
		sizeTotal.cy = h/2 * m_nZoom;
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(TRUE);
}


void CdrawView::OnSend()
{
	CString cuurentDate;
	CTime data = CTime::GetCurrentTime();
	// 구조체에 데이터 저장
	CdrawDoc* pDoc = GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CString sendStr;
	if (pFrame->m_pDataSocket == NULL) {
		MessageBox(_T("서버에 접속 안함!"), _T("알림"), MB_ICONERROR);
	}
	else {
		if (m_cType == _T(" ") && m_cThick == _T(" ") && m_cColor == _T(" ") && m_cPenType == _T(" ") && pFrame->m_cFilter == _T(" "))
		{
			MessageBox(_T("서버로 전송할 정보가 없습니다"), _T("알림"), MB_ICONERROR);
		}
		else if(m_cType == _T(" ")&& m_cThick == _T(" ") && m_cColor == _T(" ") && m_cPenType == _T(" ")){
			sendStr.Format(_T("필터 및 검출 정보 : %s\r\n \r\n 정보 전송 시간 : %d 년 %d 월 %d 일 %d 시 %d 분 %d 초\r\n \r\n"), pFrame->m_cFilter, data.GetYear(),
				data.GetMonth(), data.GetDay(), data.GetHour(), data.GetMinute(), data.GetSecond());
			
			pFrame->m_pDataSocket->Send((LPCTSTR)sendStr, (sendStr.GetLength() + 1) * sizeof(TCHAR));
		
			sendStr = _T(" ");
			pFrame->m_cFilter = _T(" ");
		}
		else{
			sendStr.Format(_T("도형, 형태 : %s \r\n픽셀 : %s \r\n색 : %s \r\n펜 스타일 : %s \r\n \r\n 정보 전송 시간 : %d 년 %d 월 %d 일 %d 시 %d 분 %d 초\r\n \r\n"),
				m_cType, m_cThick, m_cColor, m_cPenType, data.GetYear(),
				data.GetMonth(), data.GetDay(), data.GetHour(), data.GetMinute(), data.GetSecond());
			
			pFrame->m_pDataSocket->Send((LPCTSTR)sendStr, (sendStr.GetLength() + 1) * sizeof(TCHAR));
			sendStr = _T(" ");
			m_cType = m_cThick = m_cColor = m_cPenType = _T(" ");
		}
	}
}


//지워진 이미지 다시 그리기
void CdrawView::drawShape(int shapeNum, int penTy, int penWd, int sx, int sy, int ex, int ey, COLORREF fgcolor)
{
	CClientDC dc(this);
	CPen myPen, * oldPen, erase, * p_erase;

	// 전경색과 선폭 설정
	myPen.CreatePen(penTy, penWd, fgcolor);
	erase.CreatePen(PS_SOLID, penWd * 2.5, RGB(255, 255, 255));
	oldPen = dc.SelectObject(&myPen);
	if (shapeNum == 2) // 콤보상자에서 직선 선택시
	{
		dc.MoveTo(sx, sy); dc.LineTo(ex, ey);
	}
	else if (shapeNum == 0) // 콤보상자에서 사각형 선택시
	{
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(sx, sy, ex, ey);
	}
	else if (shapeNum == 1) // 콤보상자에서 원 선택시
	{
		dc.SelectStockObject(NULL_BRUSH);
		dc.Ellipse(sx, sy, ex, ey);
	}
	else if (shapeNum == 3) // 콤보상자에서 자유선 선택시
	{
		dc.MoveTo(sx, sy);
		dc.LineTo(ex, ey);
	}
	else if (shapeNum == 4) // 콤보상자에서 지우개 선택시
	{
		p_erase = dc.SelectObject(&erase);
		dc.MoveTo(sx, sy);
		dc.LineTo(ex, ey);
		dc.SelectObject(p_erase);
	}
	else if (shapeNum == 5) // 콤보상자에서 둥근사각형 선택시
	{
		dc.SelectStockObject(NULL_BRUSH);
		dc.RoundRect(sx, sy, ex, ey, 70, 70);
	}
	else if (shapeNum == 6) // 콤보상자에서 세모 선택시
	{
		Draw_Triangle(&dc, sx, sy, ex, ey);

	}
	else if (shapeNum == 7) // 콤보상자에서 화살표 선택시
	{
		Draw_Arrow(&dc, sx, sy, ex, ey);
	}

	dc.SelectObject(oldPen); // 이전 팬 선택	
	myPen.DeleteObject();  // 생성한 펜 메모리에서 제거	
}

void CdrawView::OnTake()
{
}

void CdrawView::OnSegmentLabeling()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CdrawDoc* pDoc = GetDocument();
	CClientDC dc(this);

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CONVERT_DIB_TO_BYTEIMAGE(pDoc->m_Dib, img)
	IppIntImage imgLabel;
	std::vector<IppLabelInfo> labels;
	IppLabeling(img, imgLabel, labels);

	CONVERT_IMAGE_TO_DIB(img, dib)
	AfxNewBitmap(dib);
	
	pFrame->m_labelRects.clear(); // 이전에 저장된 좌표를 지웁니다.
	for (IppLabelInfo& info : labels)
	{
		pFrame->m_labelRects.push_back(CRect(info.minx/2, info.miny/2, (info.maxx + 1)/2, (info.maxy + 1)/2));
	}
	for (const CRect& rect : pFrame->m_labelRects)
	{
		CPen pen(PS_SOLID, 4, RGB(255, 0, 0));
		CPen* pOldPen = dc.SelectObject(&pen);
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(rect);
		dc.SelectObject(pOldPen);
	}
	Invalidate();
}

//레이블링 좌표 지우기
void CdrawView::OnAllDelete()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_labelRects.clear(); // 이전에 저장된 좌표를 지웁니다.
}

//이미지 크기 조절 변수 선언
void CdrawView::OnResizeImage()
{
	CResizeImage dlg;
	if (dlg.DoModal() == IDOK) {
		RW = RH = dlg.m_nResizeImage;
		m_bResizeImage = TRUE;
		Invalidate();
	}
}