// ScrollChart.cpp : implementation file
//

#include "stdafx.h"

#include "ScrollChart.h"
#include "ChartRx.h"
//#include "stdafx.h"
#include "ChartSerial.h"
#include "ChartWnd.h"
#include "gbl.h"
//#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




//	CChartRx extern	*omg=new CChartRx();
//多媒体↓
/*
MMRESULT TimerId;
volatile int i=0;
void CALLBACK TimeProc(
       UINT uID,      //
       UINT uMsg,     
       DWORD dwUser,  
       DWORD dw1,     
       DWORD dw2      
       )
{
 if (uID==TimerId)//
 {
  cout<<++i<<endl;
  if (i==100)
  {
   timeKillEvent(TimerId);
  }
 }
}*/
/////////////////////////////////////////////////////////////////////////////
// CScrollChart

IMPLEMENT_DYNCREATE(CScrollChart, CScrollView)

CScrollChart::CScrollChart()
: m_rcWindowRect(0, 0, 100, 50) 
, m_szCanvasSize(100, 50)
, jisuji(NULL)
{

}

CScrollChart::~CScrollChart()
{

}


BEGIN_MESSAGE_MAP(CScrollChart, CScrollView)
	//{{AFX_MSG_MAP(CScrollChart)
		ON_WM_ERASEBKGND()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
		ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollChart drawing

void CScrollChart::OnInitialUpdate()
{
	//CScrollView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	CSize szTotal(m_rcWindowRect.Width() - 4, m_szCanvasSize.cy);
	SetScrollSizes(MM_TEXT, szTotal);
}

BOOL CScrollChart::OnScrollBy(CSize sizeScroll, BOOL bDoScroll )
{
	if(!CScrollView::OnScrollBy(sizeScroll, bDoScroll))
		return FALSE;

	Invalidate(FALSE);

	return TRUE;
}

void CScrollChart::OnDraw(CDC* pDC)
{
	
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
/*	CRect rcClient;  
	::GetClientRect(pDC->GetWindow()->m_hWnd,rcClient);
	CDC drawDC;  
	drawDC.CreateCompatibleDC(pDC);  
	CBitmap bitmap;  
	bitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());  
	drawDC.SelectObject(bitmap);  
	drawDC.FillSolidRect(0, 0, rcClient.Width(), rcClient.Height(), RGB(255, 255, 255));  
	// Draw grid.
	CPen penGrid(PS_SOLID, 1, RGB(0, 255, 0));
	CPen* pOldPen = drawDC.SelectObject(&penGrid);
	for (int r = 0; r < rcClient.Height(); r++)
	{
		drawDC.MoveTo(0, r * 15);
		drawDC.LineTo(rcClient.Width(), r * 15);
		if(r * 15 > rcClient.Height())
			break;
	}
	for (int c = 0; c < rcClient.Width(); c++)
	{
		drawDC.MoveTo(c * 15, 0);
		drawDC.LineTo(c * 15, rcClient.Height());
		if(c * 15 > rcClient.Width())
			break;
	}


	drawDC.SelectObject(pOldPen);
	CPoint ptViewportOrg = pDC->GetViewportOrg();  
	drawDC.OffsetViewportOrg(ptViewportOrg.x, ptViewportOrg.y);  
	CPen penLine(PS_SOLID, 1, RGB(255, 0, 0));
	pOldPen = drawDC.SelectObject(&penLine);
	CPoint ptLast;
	for (int i = 0; i < m_nIndex; i++)//动态刷新图像点
	{
		if(i == 0)
			ptLast = m_pArrPoints.GetAt(0);
		drawDC.MoveTo(ptLast);
		CPoint point = m_pArrPoints.GetAt(i);
		drawDC.LineTo(point);
		ptLast = point;//以下为添加
	
	
	} 
	if(m_nIndex>mMAXSIZE-mSTEPS)////////这里22000注意定义
	{
		 timeKillEvent(m_timerId);
	}
	drawDC.SelectObject(pOldPen);
	
	drawDC.SetViewportOrg(0, 0);  
	CPoint ptWindowOrg(0, 0); 
	pDC->DPtoLP(&ptWindowOrg);  
	pDC->BitBlt(ptWindowOrg.x, ptWindowOrg.y, rcClient.Width(), rcClient.Height(), &drawDC, 0, 0, SRCCOPY);  
	//mScroll->belief();
 */
	//CChartRx *omg=new CChartRx();//父函数调用子函数特例


	jisuji1=this;//全局变量超级指针extern  
	omg->belief(pDC);
	jisuji=this;//（超级指针）存储绘图窗的this指针

}

/////////////////////////////////////////////////////////////////////////////
// CScrollChart diagnostics

#ifdef _DEBUG
void CScrollChart::AssertValid() const
{
	CScrollView::AssertValid();
}

void CScrollChart::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScrollChart message handlers
void CScrollChart::OnMMTimer()//OnTimer(UINT nIDEvent)
{
//	if(TimerOnTag==TRUE)
//	{

	timeKillEvent(m_timerId);

	//TimerOnTag=!TimerOnTag;
	//}
	//CSize szCurrent(m_pArrPoints.GetAt(m_nIndex).x, m_szCanvasSize.cy);
	//SetScrollSizes(MM_TEXT, szCurrent);

	//static int nScroll = 0;

/*	int nMin, nMax;
	GetScrollRange(SB_HORZ, &nMin, &nMax);

	if(nMax > 0)
	{
		SetScrollPos(SB_HORZ, nMax);
	}
*/
	omg->MMTimer();//执行m_nIndex的累加
	

/*	if(m_nIndex < m_pArrPoints.GetSize() - mSTEPS)//本来是1
	{
		m_nIndex+=mSTEPS;//本来是++
	}
*/

	if((omg->m_nIndex>=mMAXSIZE-mSTEPS) && (TimerOnTag==TRUE))////////这里22000注意定义
	{
		 timeKillEvent(m_timerId);
		 TimerOnTag=FALSE;
		 MessageBox("绘图结束!");
		 return;
	}
	if(omg->m_nIndex<mMAXSIZE-mSTEPS)//
	{
		omg->SetChartType(1);
		jisuji1->SetUpdateRate(g_uUpdateRateTime);
	}
		
}

BOOL CScrollChart::OnEraseBkgnd()
{
	return FALSE;
}

void CScrollChart::SetWindowSize(int width, int height)
{
	m_rcWindowRect.right = m_rcWindowRect.left + width;
	m_rcWindowRect.bottom = m_rcWindowRect.top + height + GetSystemMetrics(SM_CYHSCROLL) + 1;
	m_szCanvasSize.cy = height - 4;
}

void CScrollChart::SetCanvasWidth(int width)
{
	if (width < m_rcWindowRect.Width())
	{
		m_szCanvasSize.cx = m_rcWindowRect.Width();
	}
	else
	{
		m_szCanvasSize.cx = width;
	}
}

void CScrollChart::SetWindowPos(int x, int y)
{
	m_rcWindowRect.left = 25+x;//
	m_rcWindowRect.top =25+ y;//
}

BOOL CScrollChart::Create1(UINT nID, CWnd* pParent)
{
	return CScrollView::CreateEx(WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 
		m_rcWindowRect, pParent, nID, NULL);
}

void CScrollChart::AddPoint(CPoint& point)
{
	m_pArrPoints.Add(point);
}

void CScrollChart::SetUpdateRate(UINT uRate)
{
	m_uUpdateRate = uRate;
	//TimerId=::timeSetEvent(m_uUpdateRate,0,TimeProc,NULL,TIME_PERIODIC);//
	//if(TimerOnTag==FALSE)
//	{
	m_timerId=timeSetEvent(m_uUpdateRate,0,&TimerCallBack,(DWORD)this,TIME_PERIODIC);
	TimerOnTag=TRUE;
//	}
	//::KillTimer(m_hWnd, 1);
	//::SetTimer(m_hWnd, 1, m_uUpdateRate, NULL);
}





void CScrollChart::OnCancelMode() 
{
	CScrollView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


//CChartRx *omg=new CChartRx();
void CScrollChart::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//父函数调用子函数特例
	omg->LButtonDbC(nFlags, point, jisuji) ;
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CScrollChart::OnLButtonDown(UINT nFlags, CPoint point) 
{

	// TODO: Add your message handler code here and/or call default
	//CChartRx *omg=new CChartRx();//父函数调用子函数特例
	omg->LButtonD(nFlags, point, jisuji) ;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CScrollChart::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//CChartRx *omg=new CChartRx();//父函数调用子函数特例
	omg->LButtonU(nFlags, point, jisuji) ;	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CScrollChart::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//CChartRx *omg=new CChartRx();//父函数调用子函数特例
	omg->RButtonD(nFlags, point, jisuji) ;	
	CScrollView::OnRButtonDown(nFlags, point);
}

void CScrollChart::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CChartRx *omg=new CChartRx();//父函数调用子函数特例
	omg->RButtonU(nFlags, point, jisuji) ;
	CScrollView::OnRButtonUp(nFlags, point);
}

void CScrollChart::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//CChartRx *omg=new CChartRx();//父函数调用子函数特例
if(TimerOnTag == FALSE)///!!!!!!!!!!!!!!!!!!!!!!!
	{
		omg->MouseM(nFlags, point, jisuji) ;
		CScrollView::OnMouseMove(nFlags, point);
	}
}
