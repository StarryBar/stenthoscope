// ChartRx.cpp : implementation file
//

#include "stdafx.h"
#include "ChartRx.h"
#include <math.h>
#include "ChartSerial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "ChartWnd.h"
#include "ScrollChart.h"
#include "gbl.h"
//#define DEBUG_OUTPUT
/////////////////////////////////////////////////////////////////////////////
// CChartRx


CChartRx::CChartRx():m_nIndex(0)
{
	// 这里只初始化画图的基本参数
	tmpcnt=0;
	pMemDC = NULL;
	pTempDC = NULL;
	m_pBackGround = NULL;
	m_pForeGround = NULL;
	m_pScreen = NULL;
	pBrush = NULL;
	crBackGround = RGB(0, 60, 0);
	crForeGround = RGB(220, 0, 0);

	SerialCount = 0;

	// 标记初始化示例数据Init()，因为用到了成员类，所以推迟：
	isFirstTime = TRUE; 
	//
	n_x_Format = 0; 
	n_y_Format = 3; 
	x_LabelFormat = "%.0f";		// x轴标签的格式化字符串
	y_LabelFormat = "%.3f";		// y轴标签的格式化字符串
	x_LabelFormatSel = "%.0f";
	y_LabelFormatSel = "%.3f";
	tip_x = "%.3f";				// 打标和鼠标位置信息的格式化信息
	tip_y = "%.3f";

	n_x_TickOneLabel = 1;      // 几个大刻度一个标签
	n_y_TickOneLabel = 1;
	n_x_TickOneLabelSel = 1;   // 几个大刻度一个标签
	n_y_TickOneLabelSel = 1;

	MajorTick = 4;
	MiniTick = 3;
	x_max = 3000;
	x_min = 0;
	y_max = 200.0;
	y_min = -200.0;

	// 图上，表格分割
	x_Divisions = 10;
	y_Divisions = 10;
	x_miniPerSec = 5;
	y_miniPerSec = 5;

	// 表格基本线的画笔
	x_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // x轴
	y_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // y轴
	h_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // 横格线
	v_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // 竖格线
	x_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // x轴上刻度
	y_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // y轴上刻度
	uptLine.SetStyle(1, PS_SOLID, RGB(255, 255, 0));   // 用户指定点的十字
	bHGrig = TRUE;
	bVGrid = TRUE;
	

	// 设置父，用来得到HDC
	userPointsLabel.pChart = this;   // 用户点的标签
	TitleLabel.pChart = this;
	x_AxisTitle.pChart = this;       // x轴标题
	y_AxisTitle.pChart = this;       // y轴标题
	x_AxisLabel.pChart = this;       // 不存储字符串，只存储颜色和字体
	y_AxisLabel.pChart = this;
	TipLabel.pChart = this;

	// 各个标题的字体和颜色
	TitleLabel.SetColor(RGB(255, 255, 0));
	x_AxisTitle.SetColor(RGB(255, 255, 0)); // 128
	y_AxisTitle.SetColor(RGB(255, 255, 0));
	x_AxisLabel.SetColor(RGB(255, 255, 0));
	y_AxisLabel.SetColor(RGB(255, 255, 0));

	// 位置设置相关参数
	TitleLabel.SetFontSize(11);
	x_AxisTitle.SetFontSize(9);
	y_AxisTitle.SetFontStyle(9, TRUE);
	TitleLabel.text = "Chart1.0 心音曲线图";
	x_AxisTitle.text = "X轴/ms";
	y_AxisTitle.text = "Y轴";

	x_AxisLabel.SetFontSize(9);
	y_AxisLabel.SetFontSize(9);
	x_AxisLabel.text = "1234.567";   // 
	y_AxisLabel.text = "1234.567";   // 用来计算左边需要的宽度，4个汉字，保存最大一个

	
	TipLabel.text = "位置信息";
	TipLabel.SetFontSize(9);
	TipLabel.rt = CRect(0, 0, 0, 0);
	TipLabel.SetColor(RGB(255, 255, 255));

	// 用户指定点的标签
	userPointsLabel.text = "位置信息";
	userPointsLabel.SetFontSize(9);
	userPointsLabel.SetColor(RGB(255, 255, 0));
	userPointsLabel.rt = CRect(0, 0, 0, 0);


//	shouldReDrawChart = FALSE;       // 当y轴标签超过预期宽度时候，重新画背景
	isMagnified = FALSE;             // 已经放大了标记
	isMagnifying = FALSE;
	nChartType = 1;                  // 折线图
	AxisSecendVisible = TRUE;        // 是否要画辅助坐标轴
	
	bCanMagnify = TRUE;              //是否可以放大，直方不可以

	bShowTip = TRUE;

}

CChartRx::~CChartRx()
{
	if (m_pBackGround)
		m_pBackGround->DeleteObject();  
	if (pMemDC)
		pMemDC->DeleteDC();
	
	if (m_pBackGround)
		delete m_pBackGround;
	if (m_pScreen)
		delete m_pScreen;
	if (pMemDC)
		delete pMemDC;
	if (pTempDC)
		delete pTempDC;
	if (pBrush)
		delete pBrush;
}


BEGIN_MESSAGE_MAP(CChartRx, CScrollChart)
//{{AFX_MSG_MAP(CChartRx)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartRx message handlers

BOOL CChartRx::Init(CDC * pDC)
{	
	// 初始化绘图资源，背景位图
	if (pMemDC == NULL)
	{
		pMemDC = new CDC();
		pMemDC->CreateCompatibleDC(NULL);  
	}	
	if (m_pBackGround == NULL)
	{
		::GetClientRect(pDC->GetWindow()->m_hWnd,&rt_Back);//获取基类RX（:Static）客户区的大小////这边开始出问题了
		wanglifan=pDC->GetWindow()->m_hWnd;
		m_pBackGround = new CBitmap();
		m_pBackGround->CreateCompatibleBitmap(pDC, rt_Back.Width(), rt_Back.Height()); 
	}
	if (pBrush)
	{
		pBrush = new CBrush();
		pBrush->CreateSolidBrush(crForeGround);
	}
	//////////////////////////////////////////////////////////////////////////
	// 初始化绘图资源，前景位图
	if (pTempDC == NULL)
	{
		pTempDC = new CDC();
		pTempDC->CreateCompatibleDC(NULL);
	}
	if (m_pForeGround == NULL)
	{
		m_pForeGround = new CBitmap();
		m_pForeGround->CreateCompatibleBitmap(pDC, rt_Back.Width(), rt_Back.Height());
	}
	if (m_pScreen == NULL)
	{
		m_pScreen = new CBitmap();
		m_pScreen->CreateCompatibleBitmap(pDC, rt_Back.Width(), rt_Back.Height()); 
	}

// 数据设置，以下部分也可以由用户来设置，然后调用initChart()

	x_minSel = x_min;
	x_maxSel = x_max;
	y_minSel = y_min;
	y_maxSel = y_max;

//	x_LabelFormat = "%.0f";   // x轴标签的格式化字符串
//	y_LabelFormat = "%.3f";   // y轴标签的格式化字符串


	ModifyStyle(wanglifan, NULL, SS_NOTIFY, 0);
	hCurCross = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
	hCurHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));//MAKEINTRESOURCE(IDC_APPSTARTING)

	// 标记计算各个部分的位置，调用InitChart()
	isIniting = TRUE;  
	return TRUE;
}

void CChartRx::OnDraw(CDC* pDC) 
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here

}

void CChartRx::InitChart()
{
	// 初始化各个部分的位置：第一次画图时候调用，以及由用户在设置参数后调用，以便重定位各个元素位置，
	::GetClientRect(wanglifan,&rt_Back);//将Rx区的矩形放入rt_back
	::GetClientRect(wanglifan,&rt_Plot);//将Rx区的矩形放入rt_plot
	rt_Plot.left += 6;//设置表格区的大小
	rt_Plot.top += 5;
	rt_Plot.bottom -= 5;
	rt_Plot.right -= 25;
	
	SIZE size;
	// 标题的部分
	TitleLabel.GetStringSize(pMemDC->m_hDC, TitleLabel.text, TitleLabel.text.GetLength(), &size);
	TitleLabel.rt = CRect(0, 0, rt_Back.Width(), size.cy + 10);
	rt_Plot.top += TitleLabel.rt.Height();       // 调整表格区域的矩形
	
	// X轴标题的部分
	x_AxisTitle.GetStringSize(pMemDC->m_hDC, x_AxisTitle.text, x_AxisTitle.text.GetLength(), &size);
	x_AxisTitle.rt = CRect(0, rt_Back.Height() - size.cy, rt_Back.Width(), rt_Back.Height());
	rt_Plot.bottom -= x_AxisTitle.rt.Height();  // 调整表格区域的矩形
	
	// Y轴标题的部分
	y_AxisTitle.GetStringSize(pMemDC->m_hDC, y_AxisTitle.text, y_AxisTitle.text.GetLength(), &size);
	y_AxisTitle.rt = CRect(0, 0, size.cy, rt_Back.Height());     // 垂直画
	rt_Plot.left += size.cy;             // 调整表格区域的矩形
	
	// 通过x轴上标签的高度，得到plot最终的位置
	x_AxisLabel.GetStringSize(pMemDC->m_hDC, x_AxisLabel.text, x_AxisLabel.text.GetLength(), &size);
	rt_Plot.bottom -= size.cy;          // 调整表格区域的矩形

	// 通过y轴上标签的宽度，得到plot最终的位置
//	y_AxisLabel.SetFontSize(9, pMemDC->m_hDC);
	y_AxisLabel.GetStringSize(pMemDC->m_hDC, y_AxisLabel.text, y_AxisLabel.text.GetLength(), &size);
	rt_Plot.left += size.cx;

	// 调整参数
	isMagnified = FALSE;             // 已经放大了标记
	isDraging = FALSE;
	isMagnify = FALSE;
	x_minSel = x_min;
	x_maxSel = x_max;
	y_minSel = y_min;
	y_maxSel = y_max;

	shouldReDrawChart = TRUE;        // 标记重新画背景
}

BOOL CChartRx::DrawChart()
{
	CBitmap   *pOldBit = pMemDC->SelectObject(m_pBackGround); //选取背景bit图填充的区域的DC对Pmemdc赋值
	pMemDC->FillSolidRect(0, 0, rt_Back.Width(), rt_Back.Height(), crBackGround); 
	if (isIniting)
	{
		isIniting = FALSE;
		InitChart();   // 计算各个部分的位置，得到plot位置
	}
	// title部分
	DrawTitle();
	DrawXTitle();
	DrawYTitle();

	// 计算x，y轴真实增加值，用来画label
	double x_addData = (x_maxSel - x_minSel) / x_Divisions;
	double y_addData = (y_maxSel - y_minSel) / y_Divisions;
	CString LabelText;
	// y 轴
	BOOL ret;
	HGDIOBJ pPenOld = pMemDC->SelectObject(y_Axis.pen);
	pMemDC->MoveTo(rt_Plot.left, rt_Plot.bottom + MajorTick);
	pMemDC->LineTo(rt_Plot.left, rt_Plot.top);
	LabelText.Format(y_LabelFormat, y_minSel);
	ret = DrawYLabel(LabelText, rt_Plot.left, rt_Plot.bottom);
	if (!ret)
		return ret;          // 根据Y标签的宽度重新计算位置

	// x 轴
	pMemDC->SelectObject(x_Axis.pen);
	pMemDC->MoveTo(rt_Plot.left - MajorTick, rt_Plot.bottom);
	pMemDC->LineTo(rt_Plot.right, rt_Plot.bottom);
	LabelText.Format(x_LabelFormat, x_minSel);
	ret = DrawXLabel(LabelText, rt_Plot.left, rt_Plot.bottom, 0);
	if (!ret)
		return ret;

	// grid 竖线
	double addPix = (float)rt_Plot.Width() / (float)(x_Divisions);  // x方向，图上的增量
	int i = 0, j = 0;
	double tempPix = rt_Plot.right;
	double tempData = x_maxSel;
	// 用来画小刻度的变量
	double tempDegreeAdd;
	double tempDegreePix; 
	int lableCount = 0;
	for (i=x_Divisions; i>0; i--)
	{	
		if (bVGrid)   // 是否画竖格线
		{
			pMemDC->SelectObject(v_Grid.pen);
			pMemDC->MoveTo((int)tempPix, rt_Plot.top);
			pMemDC->LineTo((int)tempPix, rt_Plot.bottom);
		}
		pMemDC->SelectObject(x_Degree.pen);
		pMemDC->MoveTo((int)tempPix, rt_Plot.bottom + MajorTick);
		pMemDC->LineTo((int)tempPix, rt_Plot.bottom);
		
		LabelText.Format(x_LabelFormat, tempData);
		ret = DrawXLabel(LabelText, (int)tempPix, rt_Plot.bottom, i);
		if (!ret)
			return ret; 

		tempData -= x_addData;
		tempPix -= addPix;
		// 画细刻度线
		tempDegreeAdd = addPix / x_miniPerSec;  // 一个刻度的象素增长
		tempDegreePix = tempPix; 
		//pMemDC->SelectObject(x_Degree.pen);
		for (j=1; j<x_miniPerSec; j++)
		{
			tempDegreePix += tempDegreeAdd;
			pMemDC->MoveTo((int)tempDegreePix , rt_Plot.bottom);
			pMemDC->LineTo((int)tempDegreePix, rt_Plot.bottom + MiniTick);
		}
	}
	// grid 横线
	addPix = (float)rt_Plot.Height() / (float)(y_Divisions);  // y方向，图上的增量
	tempPix = rt_Plot.top;
	tempData = y_maxSel;
	for (i=y_Divisions; i>0; i--)
	{	
		pMemDC->SelectObject(h_Grid.pen);
		pMemDC->MoveTo(rt_Plot.right, (int)tempPix);
		pMemDC->LineTo(rt_Plot.left, (int)tempPix);	
		
		pMemDC->SelectObject(y_Degree.pen);   //  - MajorTick
		pMemDC->MoveTo(rt_Plot.left - MajorTick, (int)tempPix);	
		pMemDC->LineTo(rt_Plot.left, (int)tempPix);

		LabelText.Format(y_LabelFormat, tempData);
		ret = DrawYLabel(LabelText, rt_Plot.left, (int)tempPix);
		if (!ret)
			return ret; 

		tempData -= y_addData;
		tempPix += addPix;
		// 画细刻度线
		tempDegreeAdd = addPix / y_miniPerSec;  // 一个刻度的象素增长
		tempDegreePix = tempPix; 
		pMemDC->SelectObject(y_Degree.pen);
		for (j=1; j<y_miniPerSec; j++)
		{
			tempDegreePix -= tempDegreeAdd;
			pMemDC->MoveTo(rt_Plot.left, (int)tempDegreePix);
			pMemDC->LineTo(rt_Plot.left - MiniTick, (int)tempDegreePix);
		}
	}
	// 辅助坐标轴，画右边的Y轴
	if ((nChartType == 2) || (AxisSecendVisible))
	{
		pMemDC->MoveTo(rt_Plot.right, rt_Plot.bottom);
		pMemDC->SelectObject(y_Axis.pen);
		pMemDC->LineTo(rt_Plot.right, rt_Plot.top);
		pMemDC->SelectObject(x_Axis.pen);
		pMemDC->LineTo(rt_Plot.left, rt_Plot.top);
	}
	pMemDC->SelectObject(pPenOld);
	return TRUE;
}

void CChartRx::OnSize(UINT nType, int cx, int cy) 
{

	//CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ((nType == SIZE_MAXHIDE) || (nType == SIZE_MAXSHOW) || (nType == SIZE_MINIMIZED))
	{
		//CString temp;
		//temp.Format("不刷新的消息  %d,  %d", rt_Back.Width(), rt_Back.Height());
		//MessageBox(temp);
		return;
	}

	CRect rt;
	GetClientRect(&rt);
	//CString temp;
	//temp.Format(" %d,  %d", rt.Width(), rt.Height());
	//MessageBox(temp);
	if ((rt.Width() == 0) || (rt.Height() == 0))
		return;
	if ((rt.Width() == rt_Back.Width()) && (rt.Height() == rt_Back.Height()))
		return;
	rt_Back = rt;

	shouldReDrawChart = TRUE;
	if (m_pBackGround != NULL)
	{
		GetClientRect(&rt_Back);
		m_pBackGround->DeleteObject();
		m_pBackGround->CreateCompatibleBitmap(pMemDC, rt_Back.Width(), rt_Back.Height()); 
	}

	if (m_pForeGround != NULL)
	{
		m_pForeGround->DeleteObject();
		m_pForeGround->CreateCompatibleBitmap(pMemDC, rt_Back.Width(), rt_Back.Height());
	}
	if (m_pScreen != NULL)
	{
		m_pScreen->DeleteObject();
		m_pScreen->CreateCompatibleBitmap(pMemDC, rt_Back.Width(), rt_Back.Height()); 
		
		// 窗口变大时候，重新计算位置，但是不需要改变选择范围
		double x1, x2, y1, y2;
		x1 = x_minSel;
		x2 = x_maxSel;
		y1 = y_minSel;
		y2 = y_maxSel;
		InitChart();
		x_minSel = x1;
		x_maxSel = x2;
		y_minSel = y1;
		y_maxSel = y2;

		DrawPlot();
		Invalidate();
		shouldReDrawChart = TRUE;
	}
	
}

void CChartRx::DrawTitle()
{
	// title
	TitleLabel.ShowText(pMemDC->m_hDC, TitleLabel.text, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CChartRx::DrawXTitle()
{
	x_AxisTitle.ShowText(pMemDC->m_hDC, x_AxisTitle.text, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CChartRx::DrawYTitle()
{
	y_AxisTitle.ShowText(pMemDC->m_hDC, y_AxisTitle.text, DT_VCENTER | DT_SINGLELINE);
}

BOOL CChartRx::DrawXLabel(CString str, int x_Mark, int y_Mark, int index)
{	
	if ((index % n_x_TickOneLabel) != 0)
		return TRUE;

	static int x_label_left;         // 标记前一次画时候左边是多少，判断是否重叠
	static double x_label_val;       // 前一次的值

	SIZE size;
	int nLabels = xLabelPoints.GetPointCount();
	// 如果没有指定自己的标签，就根据范围计算。
	if (nLabels == 0)
	{	
		x_AxisLabel.GetStringSize(pMemDC->m_hDC, str, str.GetLength(), &size);
		int top = rt_Back.Height() - x_AxisTitle.rt.Height() - size.cy;
		int left = x_Mark - size.cx/2;
		x_AxisLabel.rt = CRect(left, top, left + size.cx, top + size.cy);
		x_AxisLabel.ShowText(pMemDC->m_hDC, str,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 判断重叠
		if (index == 0)
		{
			x_label_left = 12800;
		}
		else
		{
			if (x_AxisLabel.rt.right > x_label_left)
			{
				n_x_TickOneLabel ++;
				return FALSE;
			}
			x_label_left = x_AxisLabel.rt.left;
		}
		// 判断重叠结束
	}
	else
	{
		if ((nLabels - 1) < index)    // 如果标签个数小于要求的索引值，不画这个
			return TRUE;
		
		CString str1;
		LPPOINTDATA pPoint = xLabelPoints.GetPoint(index);
		str1.Format(x_LabelFormat, pPoint->x);
		x_AxisLabel.GetStringSize(pMemDC->m_hDC, str1, str1.GetLength(), &size);
		int top = rt_Back.Height() - x_AxisTitle.rt.Height() - size.cy;
		int left = x_Mark - size.cx/2;
		x_AxisLabel.rt = CRect(left, top, left + size.cx, top + size.cy);
		x_AxisLabel.ShowText(pMemDC->m_hDC, str1,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	}

	return TRUE;
}

BOOL CChartRx::DrawYLabel(CString str, int x_Mark, int y_Mark)
{
	if (str.GetLength() > y_AxisLabel.text.GetLength())
	{
		// 默认的，可以放"1234.567这么长，8字节，如果长了，需要重新计算位置，再画一次
		y_AxisLabel.text = str; // 保存最长的一个
		return FALSE;
	}
	SIZE size;

	y_AxisLabel.GetStringSize(pMemDC->m_hDC, y_AxisLabel.text, y_AxisLabel.text.GetLength(), &size);
	int top = y_Mark - size.cy/2;
	int left = y_AxisTitle.rt.Width();
	y_AxisLabel.rt = CRect(left, top, left + size.cx, top + size.cy);
	y_AxisLabel.ShowText(pMemDC->m_hDC, str,  DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	return TRUE;
}

void CChartRx::DrawPlot()
{
	BOOL ret;
	if (shouldReDrawChart)         // 标记重新画背景的位置：initChart(),
	{
		ret = DrawChart();         // 画plot以外的背景
		if (!ret)
		{
			double x1 = x_minSel;
			double x2 = x_maxSel;
			double y1 = y_maxSel;
			double y2 = y_minSel;
			InitChart();           // 重新计算大小
			x_minSel = x1;
			x_maxSel = x2;
			y_maxSel = y1;
			y_minSel = y2;
			ret = DrawChart();
		}
		if (nChartType == 1)           // 画plot区域，折线图
			DrawLineChart();
		if (nChartType == 2)           // 画plot区域，直方图
			DrawChartHistogram();

		//DrawUserPoint();
	}
	shouldReDrawChart = FALSE;
}

// 从数据转换为图上的象素偏移，0为x轴，1为y轴
// 画坐标轴的时候使用
double CChartRx::GetPixFromValue(int nAxis, double value)
{
	double pix = 0;
	if (nAxis == 0)
	{
		if (value > x_maxSel)
			value = x_maxSel;
		if (value < x_minSel)
			value = x_minSel;
		pix = rt_Plot.Width() * (value - x_minSel) / (x_maxSel - x_minSel) + rt_Plot.left;
	}
	else
	{
		if (value > y_maxSel)
			value = y_maxSel;
		if (value < y_minSel)
			value = y_minSel;
		pix = rt_Plot.top + rt_Plot.Height() * (y_maxSel - value) / (y_maxSel - y_minSel);
	}
	return pix;
}

// 从图上的象素偏移转换为数据，0为x轴，1为y轴
double CChartRx::GetValueFormPix(int nAxis, double pix)
{
	double value = 0;
	if (nAxis == 0)
	{
		if (pix < rt_Plot.left)
			pix = rt_Plot.left;
		if (pix > rt_Plot.right)
			pix = rt_Plot.right;
		value = (x_maxSel - x_minSel) * (pix - rt_Plot.left) / rt_Plot.Width() + x_minSel;
	}
	else
	{
		if (pix < rt_Plot.top)
			pix = rt_Plot.top;
		if (pix > rt_Plot.bottom)
			pix = rt_Plot.bottom;
		value = y_maxSel - (y_maxSel - y_minSel) * (pix - rt_Plot.top) / rt_Plot.Height();
	}
	return value;
}



void CChartRx::DrawTip(int x, int y)
{
	CRect rt = TipLabel.rt;
	CString temp = AddFormat(x_LabelFormat) + "， " + AddFormat(y_LabelFormat);
	TipLabel.text.Format(temp, GetValueFormPix(0, x), GetValueFormPix(1, y));
	SIZE size;
	TipLabel.GetStringSize(pMemDC->m_hDC, &size);
	if (x > rt_Back.Width()/2)
	{
		TipLabel.rt.right = x - 5;
		TipLabel.rt.left = TipLabel.rt.right - size.cx;
	}
	else
	{
		TipLabel.rt.left = x + 5;
		TipLabel.rt.right = TipLabel.rt.left + size.cx;
	}

	if (y > rt_Back.Height()/2)
	{
		TipLabel.rt.bottom = y - 5;
		TipLabel.rt.top = TipLabel.rt.bottom - size.cy;
	}
	else
	{
		TipLabel.rt.top = y + 5;
		TipLabel.rt.bottom = TipLabel.rt.top + size.cy;
	}

	if (rt.left > TipLabel.rt.left)
		rt.left = TipLabel.rt.left;
	if (rt.top > TipLabel.rt.top)
		rt.top = TipLabel.rt.top;
	if (rt.bottom < TipLabel.rt.bottom)
		rt.bottom = TipLabel.rt.bottom;
	if (rt.right < TipLabel.rt.right)
		rt.right = TipLabel.rt.right;
	
	if (isMagnifying)
		::InvalidateRect(wanglifan,rt_Back,TRUE);//InvalidateRect();
	else
		::InvalidateRect(wanglifan,rt,TRUE);//jisuji->InvalidateRect(rt); jisuji=this;wanglifan=hwnd
}

void CChartRx::DrawScreen(CDC *pDC)
{
	// 从背景图直接拷贝数据，减少了画背景的时间
//	pTempDC->SelectObject(m_pScreen);
	pMemDC->SelectObject(m_pBackGround);
//	pTempDC->BitBlt(0, 0, rt_Back.Width(), rt_Back.Height(), pMemDC, 0, 0, SRCCOPY); 

	pDC->BitBlt(0, 0, rt_Back.Width(), rt_Back.Height(), pMemDC, 0, 0, SRCCOPY); 

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, crForeGround);
	if (isMagnifying)  // 画矩形
	{
		pDC->SelectObject(pen);
		CRect rt(ptDown, ptCur);
		pDC->MoveTo(ptDown);
		pDC->LineTo(ptDown.x, ptCur.y);
		pDC->LineTo(ptCur);
		pDC->LineTo(ptCur.x, ptDown.y);
		pDC->LineTo(ptDown);
	}
	else if(isDraging)
	{
	}
	else
	{
		if (bShowTip)
			TipLabel.ShowText(pDC->m_hDC, TipLabel.text, DT_CENTER | DT_SINGLELINE | DT_VCENTER); //
	}

}

void CChartRx::SaveBmpEx(LPCTSTR lpFileName)
{
	crBackGround = RGB(250, 250, 250);
	TitleLabel.SetColor(RGB(0, 0, 0));
	x_AxisTitle.SetColor(RGB(0, 0, 0)); // 128
	y_AxisTitle.SetColor(RGB(0, 0, 0));
	x_AxisLabel.SetColor(RGB(0, 0, 0));
	y_AxisLabel.SetColor(RGB(0, 0, 0));
	uptLine.SetStyle(1, PS_SOLID, RGB(0, 0, 0));
	userPointsLabel.SetColor(RGB(0, 0, 0));

		// 表格基本线的画笔
	x_Axis.SetStyle(1, PS_SOLID, RGB(0, 80, 0));         // x轴
	y_Axis.SetStyle(1, PS_SOLID, RGB(0, 80, 0));         // y轴
	h_Grid.SetStyle(1, PS_DOT, RGB(0, 80, 0));           // 横格线
	v_Grid.SetStyle(1, PS_DOT, RGB(0, 80, 0));           // 竖格线
	x_Degree.SetStyle(1, PS_SOLID, RGB(0, 80, 0));       // x轴上刻度
	y_Degree.SetStyle(1, PS_SOLID, RGB(0, 80, 0));       // y轴上刻度

	SetRedraw(FALSE);         // 控制屏幕不刷新
	shouldReDrawChart = TRUE; // 在内存中重新画一遍
	DrawPlot();

	SaveBmp(lpFileName);

	crBackGround = RGB(0, 60, 0);
	TitleLabel.SetColor(RGB(255, 255, 0));
	x_AxisTitle.SetColor(RGB(255, 255, 0)); // 128
	y_AxisTitle.SetColor(RGB(255, 255, 0));
	x_AxisLabel.SetColor(RGB(255, 255, 0));
	y_AxisLabel.SetColor(RGB(255, 255, 0));
	uptLine.SetStyle(1, PS_SOLID, RGB(255, 255, 0));
	userPointsLabel.SetColor(RGB(255, 255, 0));

	// 表格基本线的画笔
	x_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // x轴
	y_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // y轴
	h_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // 横格线
	v_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // 竖格线
	x_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // x轴上刻度
	y_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // y轴上刻度


	shouldReDrawChart = TRUE;
	DrawPlot();
	SetRedraw(TRUE);         // 控制屏幕不刷新
	


}

BOOL CChartRx::SaveBmp(LPCTSTR lpFileName)
{
	int iBits;                 // 当前分辨率下每象素所占字节数
	WORD wBitCount;            // 位图中每象素所占字节数
	DWORD dwPaletteSize = 0;   // 定义调色板大小， 
	DWORD dwBmBitsSize = 0;    // 位图中像素字节大小  
	DWORD dwDIBSize = 0;       // 位图文件大小
	DWORD dwWritten = 0;       // 写入文件字节数
	BITMAP Bitmap;             // 位图属性结构
	BITMAPFILEHEADER bmfHdr;   // 位图文件头结构 
	BITMAPINFOHEADER bi;       // 位图信息头结构
	
	LPBITMAPINFOHEADER lpbi;   //指向位图信息头结构 
	
	HANDLE fh = NULL;          // 定义文件， 
	HANDLE hDib = NULL;        //分配内存句柄，
	HANDLE hPal = NULL;        //调色板句柄
	HANDLE hOldPal = NULL; 
	
	//计算位图文件每个像素所占字节数 
	pMemDC->SelectObject(m_pBackGround);
	iBits = GetDeviceCaps(pMemDC->m_hDC, BITSPIXEL) * GetDeviceCaps(pMemDC->m_hDC, PLANES); 
	
	if (iBits <= 1) 
		wBitCount = 1; 
	else if (iBits <= 4) 
		wBitCount = 4; 
	else if (iBits <= 8)
		wBitCount = 8; 
	else 
		wBitCount = 24; 
	
	//wBitCount = 4;
	HBITMAP hBitmap = HBITMAP(*m_pBackGround);
	::GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	
	//为位图内容分配内存 
	hDib = ::GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	*lpbi = bi; 
	
	// 处理调色板 
	hPal = ::GetStockObject(DEFAULT_PALETTE); 
	if (hPal) 
	{ 
		hOldPal = ::SelectPalette(pMemDC->m_hDC, (HPALETTE)hPal, FALSE); 
	}
	
	// 获取该调色板下新的像素值 
	::GetDIBits(pMemDC->m_hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 
	
	//恢复调色板 
	if (hOldPal) 
	{ 
		::SelectPalette(pMemDC->m_hDC, (HPALETTE)hOldPal, TRUE); 
		::RealizePalette(pMemDC->m_hDC); 
	} 
	
	//创建位图文件 
	fh = ::CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
	
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE; 
	
	// 设置位图文件头 
	bmfHdr.bfType = 0x4D42; // "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize; 
	bmfHdr.bfSize = dwDIBSize; 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 
	// 写入位图文件头 
	::WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
	// 写入位图文件其余内容 
	::WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 
	//清除 
	::GlobalUnlock(hDib); // 解锁
	::GlobalFree(hDib);   // 释放文件数据存储区域
	::CloseHandle(fh);    // 关闭文件
	
	return TRUE;
}

void CChartRx::SetSerialCount(int count)
{
	// 一个线，或者说一个通道是一组数对，一个数对为一个点坐标，
	CChartSerial * serial;
	if (this->SerialCount > 0)
	{
		for (int i=0; i<SerialCount; i++)
		{
			serial = GetSerial(i);//提取Serials中的值
			delete serial;
		}
		this->Serials.RemoveAll();//移除Serials
	}
//	this->Serials.SetSize(0, 5);
	this->SerialCount = count;
	int index;
	for (int j=0; j<SerialCount; j++)
	{
		serial = new CChartSerial();//动态分配通道存储空间
		index = this->Serials.Add(serial);//此处开通双通道 放入新的Serials
	}
}


void CChartRx::SetSerialColor(int index, COLORREF cr)
{
	CChartSerial * serial = GetSerial(index);
	if (serial)
	{
		serial->crSerial = cr;
	}
}

CChartSerial * CChartRx::GetSerial(int index)//实现的是将CChartRx的值下载到CChartSerial中
{
	if (index > (SerialCount-1))
	{
		return NULL;
	}
	return (CChartSerial *)(this->Serials.GetAt(index));
}

// 画线图，在前景图上画好后，贴到背景上去，
void CChartRx::DrawLineChart()
{
	CBitmap   *pOldBit = pMemDC->SelectObject(m_pBackGround); 
	pTempDC->SelectObject(m_pForeGround);
	pTempDC->FillSolidRect(rt_Back, RGB(255, 255, 255));
	pTempDC->BitBlt(rt_Plot.left, rt_Plot.top, rt_Plot.Width(), rt_Plot.Height(),
		pMemDC, rt_Plot.left, rt_Plot.top, SRCCOPY);

	CChartSerial *pSerial = NULL;
	double x, y;
	LPPOINTDATA datapoint;

#ifdef DEBUG_OUTPUT
		///
		CStdioFile mFile;
		//mFile.Open("f:\\rx.txt", CFile::modeCreate | CFile::modeWrite, NULL);
		mFile.Open("rx.txt", CFile::modeCreate | CFile::modeWrite, NULL);
		char buffer[1024];
		CTime t = CTime::GetCurrentTime();
		CString text = t.Format( "%y-%m-%d %H:%M:%S\r\n");
		mFile.WriteString(text);
#endif

	for (int i=0; i<this->SerialCount; i++)
	{
		pSerial = this->GetSerial(i);	//	分配通道1的地址
		if (pSerial == NULL)     // 出错了，返回
		{	
			MessageBox("得到线条信息");
			return;          
		}

		CPen * pPenSerial = new CPen();
		pPenSerial->CreatePen(PS_SOLID, 1, pSerial->crSerial);

		HGDIOBJ pObj = pTempDC->SelectObject(pPenSerial);

		if (pSerial->GetPointCount() > 0)  //求取通道1中的数据的长度
		{

#ifdef DEBUG_OUTPUT
				text.Format("### 通道%2d ###\r\n", i);
				mFile.WriteString(text);
#endif
				
			datapoint = pSerial->GetPoint(0);//抽取通道1中O节点处的数据
			x = GetPixFromValue1(0, datapoint->x);          // 这里使用新的函数，否则，在越界时候失真
			y = GetPixFromValue1(1, datapoint->y);
			pTempDC->MoveTo(x, y);
			for (int j=1; j<this->m_nIndex; j++)//pSerial->GetPointCount()换成m_nindex
			{
				datapoint = pSerial->GetPoint(j);
				x = GetPixFromValue1(0, datapoint->x);//想要动态刷新波形，必须那边采集完成后再这边动态刷新。两边定时器速率不一样一定会造成卡壳。现假定两边定时器速率一样，这边按一定速率m_index+m_step显示，那边按一定速率打包采样2.或者说不需要定时器，只需要一个PV操作或者缓存，那边160缓冲器满了后就发送信号进行刷新。核心是消息操作。绘图线程操作挂起（while）等待记号P，声卡缓冲慢就发送记号P
				y = GetPixFromValue1(1, datapoint->y);
				pTempDC->LineTo(x, y);
			
#ifdef DEBUG_OUTPUT
				text.Format("%5d: %f %f\r\n", j,datapoint->x, datapoint->y);
				mFile.WriteString(text);
#endif
			}
		}

		pMemDC->BitBlt(rt_Plot.left, rt_Plot.top, rt_Plot.Width(), rt_Plot.Height(),
			pTempDC, rt_Plot.left, rt_Plot.top, SRCCOPY);

		pTempDC->SelectObject(pObj);
		pPenSerial->DeleteObject();
		delete pPenSerial;

	}

#ifdef DEBUG_OUTPUT
	mFile.Close();
#endif

}

// 画直方图，在前景图上画好后，贴到背景上去，
void CChartRx::DrawChartHistogram()
{
	CBitmap   *pOldBit = pMemDC->SelectObject(m_pBackGround); 
	pTempDC->SelectObject(m_pForeGround);
	pTempDC->FillSolidRect(rt_Back, RGB(255, 255, 255));
	pTempDC->BitBlt(rt_Plot.left, rt_Plot.top, rt_Plot.Width(), rt_Plot.Height(),
		pMemDC, rt_Plot.left, rt_Plot.top, SRCCOPY);

	CChartSerial *pSerial = NULL;
	double x, y, y0;
	LPPOINTDATA datapoint;
	for (int i=0; i<this->SerialCount; i++)
	{
		pSerial = this->GetSerial(i);
		if (pSerial == NULL)     // 出错了，返回
		{	
			MessageBox("程序内部错误：得到线条信息出错");
			return;          
		}
		CPen * pPenSerial = new CPen();
		pPenSerial->CreatePen(PS_SOLID, 1, pSerial->crSerial);

		HGDIOBJ pObj = pTempDC->SelectObject(pPenSerial);

		if (pSerial->GetPointCount() > 0)
		{
			double addPix = (float)rt_Plot.Width() / (float)(x_Divisions);  // x方向，图上的增量
			double addPix1;
			CRect rt;

			for (int j=0; j<pSerial->GetPointCount(); j++)
			{
				datapoint = pSerial->GetPoint(j);
				x = GetPixFromValue1(0, datapoint->x);   // 中轴
				y = GetPixFromValue1(1, datapoint->y);   // 高度
				y0 = GetPixFromValue1(1, 0);             // 底
				addPix1 = addPix * (datapoint->data) / 2;
				rt.left = x - addPix1;
				rt.right = x + addPix1;
				rt.top = y;
				rt.bottom = y0;
				pTempDC->FillSolidRect(rt, pSerial->crSerial);
			}
		}
		pMemDC->BitBlt(rt_Plot.left, rt_Plot.top, rt_Plot.Width(), rt_Plot.Height(),
			pTempDC, rt_Plot.left, rt_Plot.top, SRCCOPY);

		pTempDC->SelectObject(pObj);
		pPenSerial->DeleteObject();
		delete pPenSerial;

	}
}

// 画点，线的时候使用
double CChartRx::GetPixFromValue1(int nAxis, double value)
{	
	double pix = 0;
	if (nAxis == 0)
	{
		pix = rt_Plot.Width() * (value - x_minSel) / (x_maxSel - x_minSel) + rt_Plot.left;
	}
	else
	{
		pix = rt_Plot.top + rt_Plot.Height() * (y_maxSel - value) / (y_maxSel - y_minSel);
	}
	return pix;

}

// 设置类型，并刷新
void CChartRx::SetChartType(int type)
{
	if (type == 1)
	{
		bVGrid = TRUE;
		bCanMagnify = TRUE;
	}
	if (type == 2)
	{
		bVGrid = FALSE;
		bCanMagnify = FALSE;      // 不让放大
	}
	else
	{
	}
	
	this->nChartType = type;
	userPoints.Clear();   // 清除用户标记的点
	shouldReDrawChart = TRUE;
	InitChart();
	y_AxisLabel.text = "1234.678";
	DrawPlot();
//CScrollChart *p =(CScrollChart*)omg;
//	p->Invalidate();
//	p->SetFocus();
	
	jisuji1->Invalidate();
	jisuji1->SetFocus();
	
	//jisuji1->SetFocus();
}

void CChartRx::SetXScale(double min, double max)
{
	this->x_max = this->x_maxSel = max;
	this->x_min = this->x_minSel = min;
}

void CChartRx::SetYScale(double min, double max)
{
	this->y_max = this->y_maxSel = max;
	this->y_min = this->y_minSel = min;
}

void CChartRx::SetXDivision(int major, int mini)
{
	x_Divisions = major;
	x_miniPerSec = mini;

}

void CChartRx::SetYDivision(int major, int mini)
{
	y_Divisions = major;
	y_miniPerSec = mini;
}

void CChartRx::SetTickFormat(CString x, CString y)
{
	x_LabelFormatSel = x_LabelFormat = x;   // x轴标签的格式化字符串 
	y_LabelFormatSel = y_LabelFormat = y;   // y轴标签的格式化字符串
}




void CChartRx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (!bShowTip)
		return;
	if (nChar != VK_SPACE)
		return;
	POINT point;
	BOOL ret = GetCursorPos(&point);
	::ScreenToClient(this->m_hWnd, &point);
	// 如果位置不在范围内就返回，
	if ((point.x <= rt_Plot.left) || (point.y <= rt_Plot.top) || (point.x >= rt_Plot.right) || (point.y >= rt_Plot.bottom))
		return;

	double x1, y1;
	x1 = GetValueFormPix(0, point.x);
	y1 = GetValueFormPix(1, point.y);

	// 加一个点，重新；如果加那个坐标，当移动时候会有问题，可以解决，但是窗口变形就很麻烦
	this->userPoints.AddPoint(x1, y1, 0);
	shouldReDrawChart = TRUE;
	Invalidate();
	return;

	// 以下为测试使用


	CString temp;
	//temp.Format("%d, %d", point.x, point.y);
	CString format = x_LabelFormat + ", " + y_LabelFormat;
	temp.Format(format, x1, y1);

	MessageBox(temp);

	
//	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChartRx::DrawUserPoint()//动态刷新坐标点
{
	CBitmap   *pOldBit = pMemDC->SelectObject(m_pBackGround); 

	CString strFormat = AddFormat(x_LabelFormat) + "， " + AddFormat(y_LabelFormat);//CString 重载了字符函数。
	CString str;
	SIZE size;

	int x1, y1;
	LPPOINTDATA   pPoint;

	for (int i=0; i<userPoints.GetPointCount(); i++)
	{
		pPoint = userPoints.GetPoint(i);
		x1 = (int)GetPixFromValue1(0, pPoint->x);
		y1 = (int)GetPixFromValue1(1, pPoint->y);
		str.Format(strFormat, pPoint->x, pPoint->y);
		pMemDC->SelectObject(uptLine.pen);
		pMemDC->MoveTo(x1-4, y1);
		pMemDC->LineTo(x1+5, y1);
		pMemDC->MoveTo(x1, y1-4);
		pMemDC->LineTo(x1, y1+5);

			
	 // 临时借用一下这个类
		userPointsLabel.GetStringSize(pMemDC->m_hDC, str, str.GetLength(), &size);
		userPointsLabel.rt = CRect(x1, y1, x1 + size.cx, y1 + size.cy);
		userPointsLabel.ShowText(pMemDC->m_hDC, str,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
	}
}

// 更换一个图显示的时候，清理资源
void CChartRx::ClearPlot()
{

}

// 清理x轴的指定标志，不均匀的情况
void CChartRx::ClearXLabels()
{
	// 清理后，根据最大最小值计算标签
	xLabelPoints.Clear();
	bShowTip = TRUE;

}

// 添加一个x轴上的指定标签
void CChartRx::AddXLabel(double xVal)
{
	// 不为空时候，使用这个计算标签，从左到右，
	xLabelPoints.AddPoint(xVal, 0, 0);
	bShowTip = FALSE;
}

void CChartRx::SetTitleText(int index, CString str)
{
	if (index == 0)
	{
		TitleLabel.SetText(str);
	
	}
	else if (index == 1)
	{
		x_AxisTitle.SetText(str);      // x轴标题
	}
	else
	{
		y_AxisTitle.SetText(str); 
	}
}

void CChartRx::Updata()
{
	SetChartType(this->nChartType);
}

// 清除线条中的点
void CChartRx::ClearSerialPoints(int i)
{
	CChartSerial * serial;
	if (i < 0)
	{
		if (this->SerialCount > 0)
		{
			for (int ch=0; ch<SerialCount; ch++)
			{
				serial = GetSerial(ch);
				if (serial)
					serial->Clear();
			}	
		}
		return;
	}

	if (this->SerialCount > i)      // i>=0, 至少有一个通道 
	{
		serial = GetSerial(i);
		serial->Clear();
	}
}
// 根据数值的大小确定分辨率
CString CChartRx::GetFormatFormVal(double val)
{
	CString data;
	data.Format("%f", val);
	int i = data.Find('.', 0);
	int len = data.GetLength();
	LPCTSTR p = (LPCTSTR)data;
	for (int j=i; j<len; j++)
	{
		if (p[j] == '0')
			break;
	}
	if (j == len)
		j = 6;
	else
		j = j - i - 1;
	n_x_Format = j;
	data.Format(".%df", j);
	data = "%" + data;
	return data;
}

CString CChartRx::AddFormat(CString data)
{
	int len = data.GetLength();
	int i = data.Find('.', 0);
	LPCTSTR p = (LPCTSTR)data;

	int k = 0;
	if ((-1 < i) && (i < len))
	{
		if ((p[i + 1] == 0) || (p[i + 1] == 'f'))
		{
			k = 1;
		}
		else
		{
			k = p[i + 1] - '0';
			k ++;
		}	
	}
	else
	{
		k = 1;
	}
	
	if (k > 6)
		k = 6;

	CString str;
	str.Format(".%df", k);
	str = "%" + str;
	return str;
}

CString CChartRx::AddFormat(CString data, double val1, double val2)
{
	CString str;
	double ret = val1 / val2;
	str.Format("%.0f", ret);
	int t = str.GetLength();

	int k = 0;
//	int len = data.GetLength();
//	int i = data.Find('.', 0);
//	LPCTSTR p = (LPCTSTR)data;
//
//
//	if ((-1 < i) && (i < len))
//	{
//		if ((p[i + 1] == 0) || (p[i + 1] == 'f'))
//		{
//			k = t;
//		}
//		else
//		{
//			k = p[i + 1] - '0';
//			k += t;
//		}	
//	}
//	else
//	{
//		k = t;
//	}
	k += t;

	if (k > 6)
		k = 6;

	str.Format(".%df", k);
	str = "%" + str;
	return str;
}

// 用于其中的类设置字体大小
HDC CChartRx::GetDC()
{
	if (pMemDC)
	return pMemDC->m_hDC;

	return NULL;
}

void CChartRx::SetNTickOneLabel(int x, int y)
{
	n_x_TickOneLabel = x;      // 几个大刻度一个标签
	n_y_TickOneLabel = y;
	if (n_x_TickOneLabel > x_Divisions)
		n_x_TickOneLabel = x_Divisions;
	if (n_y_TickOneLabel > y_Divisions)
		n_y_TickOneLabel = y_Divisions;
	Updata();
}

double CChartRx::ABS(double val)
{
	if (val < 0)
		return -val;

	return val;
}


void CChartRx::belief(CDC* pDC)//移植ondraw的功能
{
//pDC->MoveTo(15,15);
//pDC->LineTo(200,200);
//	CPaintDC *dc; // device context for painting
	if (isFirstTime)     // 推迟的初始化，只做一次！
	{
		Init(pDC);          // 默认的初始化参数设置
		isFirstTime = FALSE;
		shouldReDrawChart = TRUE;
	}
	
	if ((pMemDC != NULL) && (m_pBackGround != NULL))
	{	
		DrawPlot();
		DrawScreen(pDC);               // 写屏幕
	}	
//	if (TimerOnTag==TRUE)
//		jisuji1->SetUpdateRate(50);
}

void CChartRx::LButtonDbC(UINT nFlags, CPoint point,CScrollChart *jisuji)
{
// MessageBox("我叫EDword..");
	// TODO: Add your message handler code here and/or call default
 	x_minSel = x_min;
 	x_maxSel = x_max;
 	y_maxSel = y_max;
	y_minSel = y_min;
	isMagnified = FALSE;
 
	x_LabelFormat = x_LabelFormatSel;
	y_LabelFormat = y_LabelFormatSel;

 	n_x_TickOneLabel = n_x_TickOneLabelSel;   // 几个大刻度一个标签
 	n_y_TickOneLabel = n_y_TickOneLabelSel;
 		
 	userPoints.Clear();  // 清除用户标记的点
 	shouldReDrawChart = TRUE;
 	jisuji->Invalidate();
 	jisuji->SetFocus();
 
}

void CChartRx::LButtonD(UINT nFlags, CPoint point,CScrollChart *jisuji)
{
//	MessageBox("左键");
	jisuji->SetFocus();
 	if (!bCanMagnify)   // 不可以放大，不处理
 		return;

	isMagnifying = TRUE;
 	ptDown = point;
 	shouldReDrawChart = TRUE;
 //	CStatic::OnLButtonDown(nFlags, point);
}



void CChartRx::LButtonU(UINT nFlags, CPoint point,CScrollChart *jisuji)
{
 //	MessageBox("tanqi");
	if (!bCanMagnify)   // 不可以放大，不处理
 		return;
 
	if (((point.x - ptDown.x) < 5) || ((point.y - ptDown.y)) < 5)
	{
 		isMagnifying = FALSE;//容许失误操作
		jisuji->Invalidate();
 //		CStatic::OnLButtonUp(nFlags, point);
		return;
 	}
 	double x0 = GetValueFormPix(0, ptDown.x);
 	double x1 = GetValueFormPix(0, point.x);
 	double y1 = GetValueFormPix(1, ptDown.y);
 	double y0 = GetValueFormPix(1, point.y);
 //	CString temp;
//	temp.Format(x_LabelFormat +"，" + y_LabelFormat, GetValueFormPix(0, point.x), GetValueFormPix(1, point.y));
 //	temp.Format("%d  %d", point.x, point.y);
 //	MessageBox(temp);
 	x_minSel = x0;
	x_maxSel = x1;
 	y_minSel = y0;
 	y_maxSel = y1;
 
 	userPoints.Clear();  // 清除用户标记的点
	isMagnify = TRUE;
 	isMagnifying = FALSE;
	shouldReDrawChart = TRUE;
 
	x_LabelFormat = AddFormat(x_LabelFormat, x_max - x_min, x_maxSel-x_minSel);
 	jisuji->Invalidate();
 //	CStatic::OnLButtonUp(nFlags, point);
 
	isMagnify = FALSE;
	isMagnified = TRUE;
 	// 这行需要注释掉，否则放大有问题的!
	//	shouldReDrawChart = FALSE;
}

void CChartRx::RButtonD(UINT nFlags, CPoint point, CScrollChart *jisuji)
{

	ptDown = point;
	isDraging = TRUE;
	::SetCursor(hCurHand);

	shouldReDrawChart = TRUE;	
}

void CChartRx::RButtonU(UINT nFlags, CPoint point,CScrollChart *jisuji)
{
	//MessageBox("hi");
	isDraging = FALSE;
 //	shouldReDrawChart = FALSE;
}

void CChartRx::MouseM(UINT nFlags, CPoint point, CScrollChart *jisuji)
{
	shouldReDrawChart = TRUE;
	// 关于放大的部分
	if (isMagnifying)
	{
		shouldReDrawChart = FALSE;
		CRect rt = rt_Back;
		InflateRect(&rt, -2, -2);
		jisuji->ClientToScreen(&rt);
	//	rt.top +=78;
	//	rt.left +=390;
	//	rt.bottom +=78;
	//	rt.right +=390;
		::ClipCursor(&rt);  //强制位移鼠标
		
	}
	else
		ClipCursor(NULL);
	ptCur = point;

	// 关于拖动的部分
	if (isDraging)
	{
		::SetCursor(hCurHand);
//		int x_off = point.x - ptDown.x;
//		int y_off = point.y - ptDown.y;
//
//		// 改变用户指点点标签的坐标，即移动点;
//		LPPOINTDATA   pPoint;
//		for (int i=0; i<userPoints.GetPointCount(); i++)
//		{
//			pPoint = userPoints.GetPoint(i);
//			pPoint->x =  (int)pPoint->x + x_off;
//			pPoint->y = (int)pPoint->y + y_off;
//		}

		double x_OffData = (x_maxSel - x_minSel) * (point.x - ptDown.x) / rt_Plot.Width();
		double y_OffData = (y_maxSel - y_minSel) * (point.y - ptDown.y) / rt_Plot.Height();
		x_minSel -= x_OffData;
		x_maxSel -= x_OffData;
		y_minSel += y_OffData;
		y_maxSel += y_OffData;
		ptDown = point;

		// 12-16 add
		CRect rt = rt_Back;
		InflateRect(&rt, -2, -2);
		jisuji->ClientToScreen(&rt);
		::ClipCursor(&rt);
		//
		
		::InvalidateRect(wanglifan,rt_Back,TRUE);//InvalidateRect(rt_Back);
		return;
	}
	else
	{
		// 只要不是在拖动，或者缩放图表就不需要重画，但是窗口大小改变时候，有问题，
		// 会引发这句，刷新失败，
		shouldReDrawChart = FALSE;
		ClipCursor(NULL);
	}

	// 如果光标在绘画区，就画提示
	if (rt_Plot.PtInRect(point))
		DrawTip(point.x, point.y);
	else
	{
		TipLabel.rt.bottom = TipLabel.rt.top;
		TipLabel.rt.right = TipLabel.rt.left;
		::InvalidateRect(wanglifan,rt_Back,TRUE);//InvalidateRect(rt_Back);
	}
	if (bShowTip)
		HCURSOR old = ::SetCursor(hCurCross);

//MessageBox("J");
//	CWnd::OnMouseMove(nFlags, point);
}

void CChartRx::MMTimer()
{
//////////////////////////////////////同步数据读入////////////////////////////////////////////////////////////////
	for (int k=0; k < mSTEPS; k++)//2000   声卡采样率/10=设置的定时器采样率*mSTEP
	{
		ALL_mChart->AddSerialPoint(0, tmpcnt*0.125, (float)(TmpBuffer[tmpcnt%BUFFER_SIZE/2]/10), tmpcnt);
		tmpcnt++;
	}
		//	ALL_mChart->SetChartType(1);
///////////////////////////////////////////////////////////////////////////////////////////////////////
CChartSerial *pserial =this->GetSerial(0);//这里简化了，只取一个通道！！

if(m_nIndex < mMAXSIZE- mSTEPS)//if(m_nIndex < pserial->GetPointCount() - mSTEPS)//本来是1
	{
		m_nIndex+=mSTEPS;//本来是++
	}
/*	if(omg->m_nIndex<mMAXSIZE-mSTEPS)//
	{
		::InvalidateRect(wanglifan,rt_Back,TRUE);
	}*/

}

void CChartRx::OnButtonSaveStart()
{
#ifndef DEBUG_OUTPUT	
#define DEBUG_OUTPUT
#endif
}

void CChartRx::OnButtonSaveEnd()
{
#ifdef DEBUG_OUTPUT	
#undef DEBUG_OUTPUT
#endif
}
