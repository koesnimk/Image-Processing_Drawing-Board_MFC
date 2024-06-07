// drawDoc.cpp: CdrawDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "draw.h"
#endif
#include <algorithm>
#include <functional>
#include <propkey.h>

#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"
#include "IppImage\IppColor.h"
#include "IppImage\IppFilter.h"
#include "IppImage\IppFourier.h"
#include "IppImage\IppFeature.h"
#include "IppImage\IppGeometry.h"
#include "IppImage\IppSegment.h"

#include "drawDoc.h"
#include "CFileNewDlg.h"
#include "CBrightnessContrastDlg.h"
#include "CGammaCorrectionDlg.h"
#include "CHistogramDlg.h"
#include "CGaussianDlg.h"
#include "CAddNoiseDlg.h"
#include "CDiffusionDlg.h"
#include "CCannyEdgeDlg.h"
#include "CHarrisCornerDlg.h"
#include "CResizeDlg.h"
#include "CBinarizationDlg.h"
#include "CFreqFilteringDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	IppByteImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_DIB_TO_RGBIMAGE(m_Dib, img) \
	IppRgbImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img, dib) \
	IppDib dib; \
	IppImageToDib(img, dib);
// CdrawDoc
const float  PI_F = 3.14159265358979323846f;
IMPLEMENT_DYNCREATE(CdrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CdrawDoc, CDocument)
	ON_COMMAND(ID_WINDOW_DUPLICATE, &CdrawDoc::OnWindowDuplicate)
	ON_COMMAND(ID_IMAGE_INVERSE, &CdrawDoc::OnImageInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CdrawDoc::OnBrightnessContrast)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERSE, &CdrawDoc::OnUpdateImageInverse)
	ON_UPDATE_COMMAND_UI(ID_BRIGHTNESS_CONTRAST, &CdrawDoc::OnUpdateBrightnessContrast)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CdrawDoc::OnGammaCorrection)
	ON_COMMAND(ID_VIEW_HISTOGRAM, &CdrawDoc::OnViewHistogram)
	ON_COMMAND(ID_HISTO_STRETCHING, &CdrawDoc::OnHistoStretching)
	ON_COMMAND(ID_HISTO_EQUALIZATION, &CdrawDoc::OnHistoEqualization)
	ON_COMMAND(ID_FILTER_MEAN, &CdrawDoc::OnFilterMean)
	ON_COMMAND(ID_FILTER_WEIGHTED_MEAN, &CdrawDoc::OnFilterWeightedMean)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CdrawDoc::OnFilterGaussian)
	ON_COMMAND(ID_FILTER_LAPLACIAN, &CdrawDoc::OnFilterLaplacian)
	ON_COMMAND(ID_FILTER_UNSHARP_MASK, &CdrawDoc::OnFilterUnsharpMask)
	ON_COMMAND(ID_FILTER_HIGHBOOST, &CdrawDoc::OnFilterHighboost)
	ON_COMMAND(ID_ADD_NOISE, &CdrawDoc::OnAddNoise)
	ON_COMMAND(ID_FILTER_MEDIAN, &CdrawDoc::OnFilterMedian)
	ON_COMMAND(ID_FILTER_DIFFUSION, &CdrawDoc::OnFilterDiffusion)
	ON_COMMAND(ID_EDGE_ROBERTS, &CdrawDoc::OnEdgeRoberts)
	ON_COMMAND(ID_EDGE_PREWITT, &CdrawDoc::OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_SOBEL, &CdrawDoc::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_CANNY, &CdrawDoc::OnEdgeCanny)
	ON_COMMAND(ID_HOUGH_LINE, &CdrawDoc::OnHoughLine)
	ON_COMMAND(ID_HARRIS_CORNER, &CdrawDoc::OnHarrisCorner)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CdrawDoc::OnColorGrayscale)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CdrawDoc::OnUpdateColorGrayscale)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CdrawDoc::OnColorSplitRgb)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_RGB, &CdrawDoc::OnUpdateColorSplitRgb)
	ON_COMMAND(ID_COLOR_SPLIT_HSI, &CdrawDoc::OnColorSplitHsi)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_HSI, &CdrawDoc::OnUpdateColorSplitHsi)
	ON_COMMAND(ID_COLOR_SPLIT_YUV, &CdrawDoc::OnColorSplitYuv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_YUV, &CdrawDoc::OnUpdateColorSplitYuv)
	ON_COMMAND(ID_COLOR_EDGE, &CdrawDoc::OnColorEdge)
	ON_UPDATE_COMMAND_UI(ID_COLOR_EDGE, &CdrawDoc::OnUpdateColorEdge)
	ON_UPDATE_COMMAND_UI(ID_GAMMA_CORRECTION, &CdrawDoc::OnUpdateGammaCorrection)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEAN, &CdrawDoc::OnUpdateFilterMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_WEIGHTED_MEAN, &CdrawDoc::OnUpdateFilterWeightedMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GAUSSIAN, &CdrawDoc::OnUpdateFilterGaussian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_LAPLACIAN, &CdrawDoc::OnUpdateFilterLaplacian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_UNSHARP_MASK, &CdrawDoc::OnUpdateFilterUnsharpMask)
	ON_UPDATE_COMMAND_UI(ID_FILTER_HIGHBOOST, &CdrawDoc::OnUpdateFilterHighboost)
	ON_UPDATE_COMMAND_UI(ID_ADD_NOISE, &CdrawDoc::OnUpdateAddNoise)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEDIAN, &CdrawDoc::OnUpdateFilterMedian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_DIFFUSION, &CdrawDoc::OnUpdateFilterDiffusion)
	ON_UPDATE_COMMAND_UI(ID_EDGE_ROBERTS, &CdrawDoc::OnUpdateEdgeRoberts)
	ON_UPDATE_COMMAND_UI(ID_EDGE_PREWITT, &CdrawDoc::OnUpdateEdgePrewitt)
	ON_UPDATE_COMMAND_UI(ID_EDGE_SOBEL, &CdrawDoc::OnUpdateEdgeSobel)
	ON_UPDATE_COMMAND_UI(ID_EDGE_CANNY, &CdrawDoc::OnUpdateEdgeCanny)
	ON_UPDATE_COMMAND_UI(ID_HOUGH_LINE, &CdrawDoc::OnUpdateHoughLine)
	ON_UPDATE_COMMAND_UI(ID_HARRIS_CORNER, &CdrawDoc::OnUpdateHarrisCorner)
	ON_COMMAND(ID_IMAGE_RESIZE, &CdrawDoc::OnImageResize)
	ON_COMMAND(ID_SEGMENT_BINARIZATION, &CdrawDoc::OnSegmentBinarization)
	ON_UPDATE_COMMAND_UI(32896, &CdrawDoc::OnUpdate32896)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_BINARIZATION, &CdrawDoc::OnUpdateSegmentBinarization)
	//ON_COMMAND(ID_SEGMENT_LABELING, &CdrawDoc::OnSegmentLabeling)
	//ON_UPDATE_COMMAND_UI(ID_SEGMENT_LABELING, &CdrawDoc::OnUpdateSegmentLabeling)
	ON_COMMAND(ID_CONTOUR_TRACING, &CdrawDoc::OnContourTracing)
	ON_UPDATE_COMMAND_UI(ID_CONTOUR_TRACING, &CdrawDoc::OnUpdateContourTracing)
	ON_COMMAND(ID_MORPHOLOGY_EROSION, &CdrawDoc::OnMorphologyErosion)
	ON_COMMAND(ID_MORPHOLOGY_DILARION, &CdrawDoc::OnMorphologyDilarion)
	ON_COMMAND(ID_MORPHOLOGY_OPENING, &CdrawDoc::OnMorphologyOpening)
	ON_COMMAND(ID_MORPHOLOGY_CLOSING, &CdrawDoc::OnMorphologyClosing)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_EROSION, &CdrawDoc::OnUpdateMorphologyErosion)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_DILARION, &CdrawDoc::OnUpdateMorphologyDilarion)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_OPENING, &CdrawDoc::OnUpdateMorphologyOpening)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_CLOSING, &CdrawDoc::OnUpdateMorphologyClosing)
	ON_COMMAND(ID_GRAYMORPH_EROSION, &CdrawDoc::OnGraymorphErosion)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_EROSION, &CdrawDoc::OnUpdateGraymorphErosion)
	ON_COMMAND(ID_GRAYMORPH_DILATION, &CdrawDoc::OnGraymorphDilation)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_DILATION, &CdrawDoc::OnUpdateGraymorphDilation)
	ON_COMMAND(ID_GRAYMORPH_OPENING, &CdrawDoc::OnGraymorphOpening)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_OPENING, &CdrawDoc::OnUpdateGraymorphOpening)
	ON_COMMAND(ID_GRAYMORPH_CLOSING, &CdrawDoc::OnGraymorphClosing)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_CLOSING, &CdrawDoc::OnUpdateGraymorphClosing)
	ON_COMMAND(ID_HOUGH_CIRCLE, &CdrawDoc::OnHoughCircle)
	ON_COMMAND(ID_FREQ_FILTERING, &CdrawDoc::OnFreqFiltering)
END_MESSAGE_MAP()

// CdrawDoc 생성/소멸

CdrawDoc::CdrawDoc() noexcept
{
}

CdrawDoc::~CdrawDoc()
{
}

BOOL CdrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	BOOL ret = TRUE;
	if (theApp.m_pNewDib != NULL)
	{
		m_Dib = *(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}
	return ret;
}




// CdrawDoc serialization

void CdrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CdrawDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CdrawDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CdrawDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CdrawDoc 진단

#ifdef _DEBUG
void CdrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CdrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CdrawDoc 명령


BOOL CdrawDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	//BOOL res = m_Dib.Load(CT2A(lpszPathName));

	return m_Dib.Load(CT2A(lpszPathName));
}


BOOL CdrawDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return m_Dib.Save(CT2A(lpszPathName));
}


void CdrawDoc::OnWindowDuplicate()
{
	AfxNewBitmap(m_Dib);
}

//이미지 반전
void CdrawDoc::OnImageInverse()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		//AfxPrintInfo((CString)(_T("[반전] 입력 영상: %s"), GetTitle()));
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		//AfxPrintInfo((CString)(_T("[반전] 입력 영상: %s"), GetTitle()));
		AfxNewBitmap(dib);
		pFrame->m_cFilter = _T("이미지 반전");
	}
}
void CdrawDoc::OnUpdateImageInverse(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


//이미지 밝기 및 명암비 조절
void CdrawDoc::OnBrightnessContrast()
{
	
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
				IppBrightness(img, dlg.m_nBrightness);
			IppContrast(img, dlg.m_nContrast);
			CONVERT_IMAGE_TO_DIB(img, dib)

			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
				IppByteImage imgY, imgU, imgV;
			IppColorSplitYUV(img, imgY, imgU, imgV);
			IppBrightness(imgY, dlg.m_nBrightness);
			IppContrast(imgY, dlg.m_nContrast);

			IppRgbImage imgRes;
			IppColorCombineYUV(imgY, imgU, imgV, imgRes);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

			AfxNewBitmap(dib);
		}
	}
	pFrame->m_cFilter = _T("이미지 밝기 %d ,명암비 %d 조절", pDoc->dlg.m_nBrightness, pDoc->dlg.m_nContrast);

}
void CdrawDoc::OnUpdateBrightnessContrast(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}

//감마 보정
void CdrawDoc::OnGammaCorrection()
{
	CGammaCorrectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
				IppGammaCorrection(img, dlg.m_fGamma);
			CONVERT_IMAGE_TO_DIB(img, dib)

			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
				IppByteImage imgY, imgU, imgV;
			IppColorSplitYUV(img, imgY, imgU, imgV);
			IppGammaCorrection(imgY, dlg.m_fGamma);

			IppRgbImage imgRes;
			IppColorCombineYUV(imgY, imgU, imgV, imgRes);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

			AfxNewBitmap(dib);

		}
	}
	pFrame->m_cFilter = _T("감마 %f 조절", dlg.m_fGamma);
	
}
void CdrawDoc::OnUpdateGammaCorrection(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


//히스토그램
void CdrawDoc::OnViewHistogram()
{
	CHistogramDlg dlg;
	dlg.SetImage(&m_Dib);
	dlg.DoModal();
	pFrame->m_cFilter = _T("히스토그램");
	
}
//스트레칭
void CdrawDoc::OnHistoStretching()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppHistogramStretching(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppByteImage imgY, imgU, imgV;
		IppColorSplitYUV(img, imgY, imgU, imgV);
		IppHistogramStretching(imgY);

		IppRgbImage imgRes;
		IppColorCombineYUV(imgY, imgU, imgV, imgRes);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxNewBitmap(dib);

	}
	pFrame->m_cFilter = _T("히스토그램 스트레칭");
	
}
//균등화
void CdrawDoc::OnHistoEqualization()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppByteImage imgY, imgU, imgV;
		IppColorSplitYUV(img, imgY, imgU, imgV);
		IppHistogramEqualization(imgY);

		IppRgbImage imgRes;
		IppColorCombineYUV(imgY, imgU, imgV, imgRes);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxNewBitmap(dib);
	}
	pFrame->m_cFilter = _T("히스토그램 균등화");
}


//필터-------------------------------------------------------------------
//평균값 필터
void CdrawDoc::OnFilterMean()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
	IppFilterMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("평균값 필터");
}
void CdrawDoc::OnUpdateFilterMean(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//가중치 필터
void CdrawDoc::OnFilterWeightedMean()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
	IppFilterWeightedMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("가중 평균 값 필터");
}
void CdrawDoc::OnUpdateFilterWeightedMean(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//가우시안 필터
void CdrawDoc::OnFilterGaussian()
{
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			IppFloatImage imgDst;
		IppFilterGaussian(imgSrc, imgDst, dlg.m_fSigma);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxNewBitmap(dib);
	}
	pFrame->m_cFilter = _T("가우시안 필터");
}
void CdrawDoc::OnUpdateFilterGaussian(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//라플라시안 필터
void CdrawDoc::OnFilterLaplacian()
{
	pFrame->m_cFilter = _T("라플라시안 필터");
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
	IppByteImage imgDst;
	IppFilterLaplacian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxNewBitmap(dib);
	
	
}
void CdrawDoc::OnUpdateFilterLaplacian(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//언샤프 마스크
void CdrawDoc::OnFilterUnsharpMask()
{
	pFrame->m_cFilter = _T("언샤프 마스크 필터");
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
	IppByteImage imgDst;
	IppFilterUnsharpMask(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxNewBitmap(dib);

}
void CdrawDoc::OnUpdateFilterUnsharpMask(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//하이부스트 필터
void CdrawDoc::OnFilterHighboost()
{
	pFrame->m_cFilter = _T("하이부스트 필터");
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
	IppByteImage imgDst;
	float alpha = 1.2f;
	IppFilterHighboost(imgSrc, imgDst, alpha);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateFilterHighboost(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//잡음추가
void CdrawDoc::OnAddNoise()
{
	CAddNoiseDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			IppByteImage imgDst;

		if (dlg.m_nNoiseType == 0)
			IppNoiseGaussian(imgSrc, imgDst, dlg.m_nAmount);
		else
			IppNoiseSaltNPepper(imgSrc, imgDst, dlg.m_nAmount);

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

			TCHAR* noise[] = { _T("가우시안"), _T("소금&후추") };
		AfxNewBitmap(dib);
	}
	if (dlg.m_nNoiseType == 0)
		pFrame->m_cFilter = _T("가우시안 잡음 추가");


	else
		pFrame->m_cFilter = _T("소금 & 후추 잡음 추가");
}
void CdrawDoc::OnUpdateAddNoise(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//미디언 필터
void CdrawDoc::OnFilterMedian()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
	IppByteImage imgDst;
	IppFilterMedian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib);

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("미디언 필터");
}
void CdrawDoc::OnUpdateFilterMedian(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//비등확산 필터
void CdrawDoc::OnFilterDiffusion()
{
	CDiffusionDlg dlg;
	if (dlg.DoModal() == IDOK) {
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc);
		IppFloatImage imgDst;
		IppFilterDiffusion(imgSrc, imgDst, dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		CONVERT_IMAGE_TO_DIB(imgDst, dib);

		AfxNewBitmap(dib);
	}
	pFrame->m_cFilter = _T("비등방성 확산 필터");
}
void CdrawDoc::OnUpdateFilterDiffusion(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//마스크 기반 엣지-----
//로버츠
void CdrawDoc::OnEdgeRoberts()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	IppByteImage imgEdge;
	IppEdgeRoberts(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("로버츠 엣지");
}
void CdrawDoc::OnUpdateEdgeRoberts(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//프리윗
void CdrawDoc::OnEdgePrewitt()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	IppByteImage imgEdge;
	IppEdgePrewitt(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("프리윗 엣지");
}
void CdrawDoc::OnUpdateEdgePrewitt(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//소벨
void CdrawDoc::OnEdgeSobel()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	IppByteImage imgEdge;
	IppEdgeSobel(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("소벨 엣지");
}
void CdrawDoc::OnUpdateEdgeSobel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//캐니엣지 검출
void CdrawDoc::OnEdgeCanny()
{
	CCannyEdgeDlg dlg;
	if (dlg.DoModal() == IDOK) {
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
		IppByteImage imgEdge;
		IppEdgeCanny(img, imgEdge, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib);

		AfxNewBitmap(dib);
	}
	pFrame->m_cFilter = _T("캐니 엣지 검출");
}
void CdrawDoc::OnUpdateEdgeCanny(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

//허프 직선 검출
void CdrawDoc::OnHoughLine()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	IppByteImage imgEdge;
	IppEdgeCanny(img, imgEdge, 1.4f, 30.f, 60.f);

	std::vector<IppLineParam> lines;
	IppHoughLine(imgEdge, lines);

	if (lines.size() == 0) {
		AfxMessageBox(_T("검출된 직선이 없습니다."));
		return;
	}

	std::sort(lines.begin(), lines.end());

	int cnt = min(10, (int)lines.size());
	for (int i = 0; i < cnt; i++)
		IppDrawLine(img, lines[i], 255);

	CONVERT_IMAGE_TO_DIB(img, dib);

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("허프 직선 검출");
}
void CdrawDoc::OnUpdateHoughLine(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8); 
}

//해리코너 검출
void CdrawDoc::OnHarrisCorner()
{
	CHarrisCornerDlg dlg;
	if (dlg.DoModal() == IDOK) {
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			std::vector<IppPoint> corners;
		IppHarrisCorner(img, corners, dlg.m_nHarrisTh);

		BYTE** ptr = img.GetPixels2D();

		int x, y;
		for (IppPoint cp : corners) {
			x = cp.x;
			y = cp.y;

			ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
			ptr[y][x - 1] = ptr[y][x] = ptr[y][x + 1] = 0;
			ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;

		}
		CONVERT_IMAGE_TO_DIB(img, dib);

		AfxNewBitmap(dib);
		pFrame->m_cFilter = _T("해리 코너 검출");
	}
}
void CdrawDoc::OnUpdateHarrisCorner(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


//그레이 스케일 변환
void CdrawDoc::OnColorGrayscale()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgGrey;
	imgGrey.Convert(imgColor);
	CONVERT_IMAGE_TO_DIB(imgGrey, dib);
	UpdateAllViews(NULL);
	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("그레이스케일 변환");
	
}
void CdrawDoc::OnUpdateColorGrayscale(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}

//색상 평면 나누기
//RGB
void CdrawDoc::OnColorSplitRgb()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor);
	IppByteImage imgR, imgG, imgB;
	IppColorSplitRGB(imgColor, imgR, imgG, imgB);
	CONVERT_IMAGE_TO_DIB(imgR, dibR);
	CONVERT_IMAGE_TO_DIB(imgG, dibG);
	CONVERT_IMAGE_TO_DIB(imgB, dibB);

	AfxNewBitmap(dibR);
	AfxNewBitmap(dibG);
	AfxNewBitmap(dibB);
	pFrame->m_cFilter = _T("색상 평면 나누기 : RGB");
}
void CdrawDoc::OnUpdateColorSplitRgb(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}

//HSI
void CdrawDoc::OnColorSplitHsi()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor);
	IppByteImage imgH, imgS, imgI;
	IppColorSplitRGB(imgColor, imgH, imgS, imgI);
	CONVERT_IMAGE_TO_DIB(imgH, dibH);
	CONVERT_IMAGE_TO_DIB(imgS, dibS);
	CONVERT_IMAGE_TO_DIB(imgI, dibI);

	AfxNewBitmap(dibH);
	AfxNewBitmap(dibS);
	AfxNewBitmap(dibI);
	pFrame->m_cFilter = _T("색상 평면 나누기 : HSI");
}
void CdrawDoc::OnUpdateColorSplitHsi(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}

//YUV
void CdrawDoc::OnColorSplitYuv()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor);
	IppByteImage imgY, imgU, imgV;
	IppColorSplitRGB(imgColor, imgY, imgU, imgV);
	CONVERT_IMAGE_TO_DIB(imgY, dibY);
	CONVERT_IMAGE_TO_DIB(imgU, dibU);
	CONVERT_IMAGE_TO_DIB(imgV, dibV);

	AfxNewBitmap(dibY);
	AfxNewBitmap(dibU);
	AfxNewBitmap(dibV);
	pFrame->m_cFilter = _T("색상 평면 나누기 : YUV");
}
void CdrawDoc::OnUpdateColorSplitYuv(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}

//컬러 엣지 검출
void CdrawDoc::OnColorEdge()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img);
	IppByteImage imgEdge;
	IppColorEdge(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib);

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("컬러 엣지 검출");
}
void CdrawDoc::OnUpdateColorEdge(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}

//이미지 크기 조정
void CdrawDoc::OnImageResize()
{
	CResizeDlg dlg;
	dlg.m_nOldWidth = m_Dib.GetWidth();
	dlg.m_nOldHeight = m_Dib.GetHeight();
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			IppByteImage imgDst;
		switch (dlg.m_nInterpolation)
		{
		case 0: IppResizeNearest(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			pFrame->m_cFilter = _T("이미지 크기 조절 : 최근방 이웃 보간법");
		case 1: IppResizeBilinear(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			pFrame->m_cFilter = _T("이미지 크기 조절 : 양선형 보간법");
		case 2: IppResizeCubic(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			pFrame->m_cFilter = _T("이미지 크기 조절 : 3차 회선 보간법");
		}

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

			TCHAR* interpolation[] = { _T("최근방 이웃 보간법"), _T("양선형 보간법"), _T("3차 회선 보간법") };
		
		AfxNewBitmap(dib);
	}
}
void CdrawDoc::OnUpdate32896(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnSegmentBinarization()
{
	CBinarizationDlg dlg;
	dlg.SetImage(m_Dib);
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppByteImage imgRes;
		IppBinarization(img, imgRes, dlg.m_nThreshold);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxNewBitmap(dib);
	}
	pFrame->m_cFilter = _T("이진화");
}
void CdrawDoc::OnUpdateSegmentBinarization(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


//void CdrawDoc::OnSegmentLabeling()
//{
//	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
//		IppIntImage imgLabel;
//	std::vector<IppLabelInfo> labels;
//	int label_cnt = IppLabeling(img, imgLabel, labels);
//
//	 //객체를 감싸는 사각형 그리기
//	BYTE** ptr = img.GetPixels2D();
//
//	for (IppLabelInfo& info : labels)
//	{
//		for (int j = info.miny; j <= info.maxy; j++)
//			ptr[j][info.minx] = ptr[j][info.maxx] = 128;
//
//		for (int i = info.minx; i <= info.maxx; i++)
//			ptr[info.miny][i] = ptr[info.maxy][i] = 128;
//	}
//
//	CONVERT_IMAGE_TO_DIB(img, dib)
//	AfxNewBitmap(dib);
//}
//void CdrawDoc::OnUpdateSegmentLabeling(CCmdUI* pCmdUI)
//{
//	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
//}


void CdrawDoc::OnContourTracing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppIntImage imgLabel;
	std::vector<IppLabelInfo> labels;
	int label_cnt = IppLabeling(img, imgLabel, labels);

	IppByteImage imgContour(img.GetWidth(), img.GetHeight());
	BYTE** ptr = imgContour.GetPixels2D();
	for (IppLabelInfo& info : labels)
	{
		std::vector<IppPoint> cp;
		IppContourTracing(img, info.pixels[0].x, info.pixels[0].y, cp);

		for (IppPoint& pt : cp)
			ptr[pt.y][pt.x] = 255;
	}

	CONVERT_IMAGE_TO_DIB(imgContour, dib)

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("외곽선 추적");
}
void CdrawDoc::OnUpdateContourTracing(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnMorphologyErosion()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("이진 모폴로지 : 침식");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateMorphologyErosion(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnMorphologyDilarion()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("이진 모폴로지 : 팽창");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateMorphologyDilarion(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnMorphologyOpening()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("이진 모폴로지 : 열림");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateMorphologyOpening(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnMorphologyClosing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("이진 모폴로지 : 닫힘");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateMorphologyClosing(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnGraymorphErosion()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyGrayErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("그레이스케일 모폴로지 : 침식");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateGraymorphErosion(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnGraymorphDilation()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyGrayDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("그레이스케일 모폴로지 : 팽창");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateGraymorphDilation(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnGraymorphOpening()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyGrayOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("그레이스케일 모폴로지 : 열림");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateGraymorphOpening(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnGraymorphClosing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
	IppMorphologyGrayClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
		pFrame->m_cFilter = _T("그레이스케일 모폴로지 : 닫힘");
	AfxNewBitmap(dib);
}
void CdrawDoc::OnUpdateGraymorphClosing(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CdrawDoc::OnHoughCircle()
{

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img);
	IppByteImage imgEdge;
	IppEdgeCanny(img, imgEdge, 1.4f, 30.f, 60.f);

	std::vector<IppCircleParam> circles;
	IppHoughCircle(imgEdge, circles);

	if (circles.size() == 0) {
		AfxMessageBox(_T("검출된 원이 없습니다."));
		return;
	}

	std::sort(circles.begin(), circles.end());

	int cnt = min(10, static_cast<int>(circles.size()));
	for (int i = 0; i < cnt; i++)
	{
		int centerX = static_cast<int>(circles[i].radius * cos(circles[i].theta * PI_F / 180.0)); // 중심 x 좌표
		int centerY = static_cast<int>(circles[i].radius * sin(circles[i].theta * PI_F / 180.0)); // 중심 y 좌
		int radius = circles[i].radius; // 반지름 추출
		IppDrawCircle(img, centerX, centerY, radius, 255);
	}

	CONVERT_IMAGE_TO_DIB(img, dib);

	AfxNewBitmap(dib);
	pFrame->m_cFilter = _T("허프 원 검출");
}




void CdrawDoc::OnFreqFiltering()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CFreqFilteringDlg dlg;
	dlg.m_strRange.Format(_T("(0 ~ %d)"), min(w / 2, h / 2));
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

			IppFourier fourier;
		fourier.SetImage(img);
		fourier.FFT(1);

		if (dlg.m_nFilterType == 0)
		{
			if (dlg.m_nFilterShape == 0)
				fourier.LowPassIdeal(dlg.m_nCutoff);
			else
				fourier.LowPassGaussian(dlg.m_nCutoff);
		}
		else
		{
			if (dlg.m_nFilterShape == 0)
				fourier.HighPassIdeal(dlg.m_nCutoff);
			else
				fourier.HighPassGaussian(dlg.m_nCutoff);
		}

		fourier.FFT(-1);

		IppByteImage img2;
		fourier.GetImage(img2);
		CONVERT_IMAGE_TO_DIB(img2, dib)

			TCHAR* type[] = { _T("저역 통과 필터"), _T("고역 통과 필터") };
		TCHAR* shape[] = { _T("이상적(Ideal)"), _T("가우시안(Gaussian)") };

		AfxNewBitmap(dib);
	}
}
