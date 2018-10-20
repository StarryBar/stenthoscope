#if !defined(AFX_SCROLLCHART_H__24A34CE6_116A_4D6C_A674_5D9422245A4B__INCLUDED_)
#define AFX_SCROLLCHART_H__24A34CE6_116A_4D6C_A674_5D9422245A4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollChart.h : header file
//
#include "windows.h"
#include "mmsystem.h"
#include "iostream"
#pragma comment(lib,"winmm.lib")

/////////////////////////////////////////////////////////////////////////////
// CScrollChart view
#include <afxtempl.h>
#include "testChartDll.h"





//#include "ChartRx.h"


class CScrollChart : public CScrollView 
{
	DECLARE_DYNCREATE(CScrollChart)

// Constructors.
public:
	CScrollChart();
	virtual ~CScrollChart();

// Operations.
public:
	CScrollChart *jisuji;
	void SetWindowSize(int width, int height);
	void SetCanvasWidth(int width);
	void SetWindowPos(int x, int y);
	BOOL Create1(UINT nID, CWnd* pParent);
	void AddPoint(CPoint& point);
	void SetUpdateRate(UINT uRate);
	MMRESULT m_timerId;
void OnMMTimer();


	virtual void belief(CDC* pDC){};

// Attributes.

protected:
//	     // overridden to draw this view
	
	CRect  m_rcWindowRect;
	CSize  m_szCanvasSize;
	UINT   m_uUpdateRate;
	CArray<CPoint, CPoint> m_pArrPoints;
//	UINT   m_nIndex;
	static void CALLBACK TimerCallBack(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
	{
	CScrollChart * pThis=(CScrollChart*)dwUser;
	pThis->OnMMTimer();//
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollChart)
	protected:
	virtual void OnDraw(CDC* pDC); 
	
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll /* = TRUE */);
public:
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScrollChart)
	afx_msg BOOL OnEraseBkgnd();
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLCHART_H__24A34CE6_116A_4D6C_A674_5D9422245A4B__INCLUDED_)
