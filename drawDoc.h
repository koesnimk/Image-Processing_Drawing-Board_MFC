
// drawDoc.h: CdrawDoc 클래스의 인터페이스
//


#pragma once
#include "MainFrm.h"
#include <afxtempl.h>
#include <atlimage.h>
#include ".\IppImage\IppDib.h"
class CdrawDoc : public CDocument
{

protected: // serialization에서만 만들어집니다.
	CdrawDoc() noexcept;
	DECLARE_DYNCREATE(CdrawDoc)

// 특성입니다.
public:

	IppDib m_Dib;
// 작업입니다.
public:
	//CString m_cFilter;
// 재정의입니다.
public:
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CdrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	
	afx_msg void OnWindowDuplicate();
	afx_msg void OnImageInverse();
	afx_msg void OnBrightnessContrast();
	afx_msg void OnUpdateImageInverse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrightnessContrast(CCmdUI* pCmdUI);
	afx_msg void OnGammaCorrection();
	afx_msg void OnViewHistogram();
	afx_msg void OnHistoStretching();
	afx_msg void OnHistoEqualization();
	afx_msg void OnFilterMean();
	afx_msg void OnFilterWeightedMean();
	afx_msg void OnFilterGaussian();
	afx_msg void OnFilterLaplacian();
	afx_msg void OnFilterUnsharpMask();
	afx_msg void OnFilterHighboost();
	afx_msg void OnAddNoise();
	afx_msg void OnFilterMedian();
	afx_msg void OnFilterDiffusion();
	afx_msg void OnEdgeRoberts();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgeCanny();
	afx_msg void OnHoughLine();
	afx_msg void OnHarrisCorner();
	afx_msg void OnColorGrayscale();
	afx_msg void OnUpdateColorGrayscale(CCmdUI* pCmdUI);
	afx_msg void OnColorSplitRgb();
	afx_msg void OnUpdateColorSplitRgb(CCmdUI* pCmdUI);
	afx_msg void OnColorSplitHsi();
	afx_msg void OnUpdateColorSplitHsi(CCmdUI* pCmdUI);
	afx_msg void OnColorSplitYuv();
	afx_msg void OnUpdateColorSplitYuv(CCmdUI* pCmdUI);
	afx_msg void OnColorEdge();
	afx_msg void OnUpdateColorEdge(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGammaCorrection(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterMean(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterWeightedMean(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterGaussian(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterLaplacian(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterUnsharpMask(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterHighboost(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddNoise(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterMedian(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterDiffusion(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdgeRoberts(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdgePrewitt(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdgeSobel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdgeCanny(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHoughLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHarrisCorner(CCmdUI* pCmdUI);
	afx_msg void OnImageResize();
	afx_msg void OnSegmentBinarization();
	afx_msg void OnUpdate32896(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSegmentBinarization(CCmdUI* pCmdUI);
	//afx_msg void OnSegmentLabeling();
	//afx_msg void OnUpdateSegmentLabeling(CCmdUI* pCmdUI);
	afx_msg void OnContourTracing();
	afx_msg void OnUpdateContourTracing(CCmdUI* pCmdUI);
	afx_msg void OnMorphologyErosion();
	afx_msg void OnMorphologyDilarion();
	afx_msg void OnMorphologyOpening();
	afx_msg void OnMorphologyClosing();
	afx_msg void OnUpdateMorphologyErosion(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphologyDilarion(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphologyOpening(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMorphologyClosing(CCmdUI* pCmdUI);
	afx_msg void OnGraymorphErosion();
	afx_msg void OnUpdateGraymorphErosion(CCmdUI* pCmdUI);
	afx_msg void OnGraymorphDilation();
	afx_msg void OnUpdateGraymorphDilation(CCmdUI* pCmdUI);
	afx_msg void OnGraymorphOpening();
	afx_msg void OnUpdateGraymorphOpening(CCmdUI* pCmdUI);
	afx_msg void OnGraymorphClosing();
	afx_msg void OnUpdateGraymorphClosing(CCmdUI* pCmdUI);
	afx_msg void OnHoughCircle();
	afx_msg void OnFreqFiltering();
};
