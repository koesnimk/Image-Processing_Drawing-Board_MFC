#pragma once


// ChildFrame 프레임

class ChildFrame : public CMDIChildWnd
{
public:
	ChildFrame() noexcept;           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ChildFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(ChildFrame)

public:
	//afx_msg void OnPaint();
	//void LoadImage();
};
