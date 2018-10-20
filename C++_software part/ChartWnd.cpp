// ChartWnd.cpp: implementation of the CChartWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "testChartDll.h"
#include "ChartWnd.h"

#include "ChartLine.h"
#include "ChartRx.h"
#include "ChartSerial.h"
#include "ScrollChart.h"
#include "gbl.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartWnd::CChartWnd()
{
	mChart = new CChartRx();
}

CChartWnd::~CChartWnd()
{

	delete mChart;
}

CChartWnd momg;


BOOL CChartWnd::Create(LPCTSTR lpszText, DWORD dwStyle,
				const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	CScrollChart* gChart=(CScrollChart*)mChart->CreateObject();//类似new
	gChart->SetWindowPos(rect.left, rect.top);
	gChart->SetWindowSize(rect.right, rect.bottom);
	gChart->SetCanvasWidth(10000);
	gChart->OnInitialUpdate();
	gChart->Create1(1000, pParentWnd);
//	dwStyle = dwStyle | WS_CHILD | WS_VISIBLE;
	return	TRUE;	//mChart->Create(lpszText, dwStyle, rect, pParentWnd, nID);//static CREATE改为scrollchart create
}

void CChartWnd::SetXScale(double min, double max)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->SetXScale(min, max);
}

void CChartWnd::SetYScale(double min, double max)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->SetYScale(min, max);
}

void CChartWnd::SetTitle(int index, LPCTSTR str)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->SetTitleText(index, str);
}

void CChartWnd::SetChartType(int type)
{
	CChartRx * pChart = (CChartRx *)mChart;
	//pChart->SetChartType(type);
	omg->SetChartType(type);//!!!!!重点，.h+extern定义全局变量，注意用#ifndef #define #endif避免重复定义；+cpp中设置头文件+CScrollChart申明变量和赋值，&关联头文件+调用
}

void CChartWnd::SetYDivision(int major, int mini)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->SetYDivision(major, mini);
}

void CChartWnd::SetXDivision(int major, int mini)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->SetXDivision(major, mini);
}

void CChartWnd::SetTickFormat(CString x, CString y)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->SetTickFormat(x, y);
}

void CChartWnd::SetSerialCount(int count)
{
	CChartRx * pChart = (CChartRx *)mChart;
	//pChart->SetSerialCount(count);
	omg->SetSerialCount(count);
}

void CChartWnd::SetSerialColor(int index, COLORREF cr)
{
	CChartRx * pChart = (CChartRx *)mChart;
	CChartSerial * serial = omg->GetSerial(index);
	serial->crSerial = cr;
}

COLORREF CChartWnd::GetSerialColor(int index)
{
	CChartRx * pChart = (CChartRx *)mChart;
	CChartSerial * serial = omg->GetSerial(index);
	return serial->crSerial;

}

void CChartWnd::SetSerialPoints(int index, int count)
{
	CChartRx * pChart = (CChartRx *)mChart;
	CChartSerial * serial = omg->GetSerial(index);
	serial->SetSize(count, -1);

}

void CChartWnd::AddSerialPoint(int index, double x, double y, double value)
{
	CChartRx * pChart = (CChartRx *)mChart;
	CChartSerial * serial = omg->GetSerial(index);
	serial->AddPoint(x, y, value);
}

void CChartWnd::AddSerialPointEx(int index, double x, double y, double value)
{
	CChartRx * pChart = (CChartRx *)mChart;
	CChartSerial * serial = omg->GetSerial(index);
	serial->AddPointEx(x, y, value);
}

void CChartWnd::ClearSerialPoints(int index)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->ClearSerialPoints(index);
}

void CChartWnd::AddXLabel(double xVal)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->AddXLabel(xVal);
}

void CChartWnd::ClearXLabels()
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->ClearXLabels();
}

void CChartWnd::ShowTip(BOOL bShow)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->bShowTip = bShow;
}

void CChartWnd::CanMagnify(BOOL bShow)
{
	CChartRx * pChart = (CChartRx *)mChart;
	omg->bCanMagnify = bShow;
}

void CChartWnd::SaveBmp(LPCTSTR Filepath, BOOL bWhileBack)
{
	CChartRx * pChart = (CChartRx *)mChart;	
	if (bWhileBack)
		omg->SaveBmpEx(Filepath);
	else
		omg->SaveBmp(Filepath);
}


void CChartWnd::Updata()
{
	CChartRx * pChart = (CChartRx *)mChart;	
	omg->Updata();
}

void CChartWnd::SetBackColor(COLORREF cr)
{
	CChartRx * pChart = (CChartRx *)mChart;	
	omg->crBackGround = cr;
}

void CChartWnd::SetForeColor(COLORREF cr)
{
	CChartRx * pChart = (CChartRx *)mChart;	
	omg->crForeGround = cr;
}

void CChartWnd::SetTitleColor(int index, COLORREF cr)
{
	CChartRx * pChart = (CChartRx *)mChart;	
	if (index == 0)
	{
		omg->TitleLabel.SetColor(cr);
	}
	else if (index == 1)
	{
		omg->x_AxisTitle.SetColor(cr);
	}
	else if (index == 2)
	{
		omg->y_AxisTitle.SetColor(cr);
	}
}

void CChartWnd::SetLableColor(int index, COLORREF cr)
{
	CChartRx * pChart = (CChartRx *)mChart;	

	if (index == 1)
	{
		omg->x_AxisLabel.SetColor(cr);
	}
	else if (index == 2)
	{
		omg->y_AxisLabel.SetColor(cr);
	}
	else if (index == 3)
	{
		omg->TipLabel.SetColor(cr);
	}
	else if (index == 4)
	{
		omg->userPointsLabel.SetColor(cr);
	}
}

void CChartWnd::SetTitleFontSize(int index, int size)
{
	CChartRx * pChart = (CChartRx *)mChart;	
	if (omg->pMemDC == NULL)
		return;

	if (index == 0)
	{
		omg->TitleLabel.SetFontSize(size, omg->pMemDC->m_hDC);
	}
	else if (index == 1)
	{
		omg->x_AxisTitle.SetFontSize(size, omg->pMemDC->m_hDC);
	}
	else if (index == 2)
	{
		omg->y_AxisTitle.SetFontSize(size, omg->pMemDC->m_hDC);
	}
}

void CChartWnd::SetLabelFontSize(int index, int size)
{
	CChartRx * omg = (CChartRx *)mChart;	
	if (omg->pMemDC == NULL)
		return;

	if (index == 1)
	{
		omg->x_AxisLabel.SetFontSize(size, omg->pMemDC->m_hDC);
	}
	else if (index == 2)
	{
		omg->y_AxisLabel.SetFontSize(size, omg->pMemDC->m_hDC);
	}
	else if (index == 3)
	{
		omg->TipLabel.SetFontSize(size, omg->pMemDC->m_hDC);
	}
	else if (index == 4)
	{
		omg->userPointsLabel.SetFontSize(size, omg->pMemDC->m_hDC);
	}
}
