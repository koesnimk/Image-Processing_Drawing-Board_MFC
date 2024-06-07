// ChildFrame.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"

#include "draw.h"
#include "ChildFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(ChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(ChildFrame, CMDIChildWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()

ChildFrame::ChildFrame() noexcept
{
}

ChildFrame::~ChildFrame()
{
}

BOOL ChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CMDIChildWnd::PreCreateWindow(cs))
        return FALSE;

    // 필요한 경우 여기에서 cs 구조체를 수정합니다.

    return TRUE;
}

#ifdef _DEBUG
void ChildFrame::AssertValid() const
{
    CMDIChildWnd::AssertValid();
}

void ChildFrame::Dump(CDumpContext& dc) const
{
    CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

