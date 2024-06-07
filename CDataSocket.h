#pragma once

// CDataSocket 명령 대상
class CMainFrame;
class CDataSocket : public CSocket
{
public:
	CDataSocket(CMainFrame* pMainFrame);
	virtual ~CDataSocket();
	CMainFrame* m_pdFrame;
};


