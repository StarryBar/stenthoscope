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
	// ����ֻ��ʼ����ͼ�Ļ�������
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

	// ��ǳ�ʼ��ʾ������Init()����Ϊ�õ��˳�Ա�࣬�����Ƴ٣�
	isFirstTime = TRUE; 
	//
	n_x_Format = 0; 
	n_y_Format = 3; 
	x_LabelFormat = "%.0f";		// x���ǩ�ĸ�ʽ���ַ���
	y_LabelFormat = "%.3f";		// y���ǩ�ĸ�ʽ���ַ���
	x_LabelFormatSel = "%.0f";
	y_LabelFormatSel = "%.3f";
	tip_x = "%.3f";				// �������λ����Ϣ�ĸ�ʽ����Ϣ
	tip_y = "%.3f";

	n_x_TickOneLabel = 1;      // ������̶�һ����ǩ
	n_y_TickOneLabel = 1;
	n_x_TickOneLabelSel = 1;   // ������̶�һ����ǩ
	n_y_TickOneLabelSel = 1;

	MajorTick = 4;
	MiniTick = 3;
	x_max = 3000;
	x_min = 0;
	y_max = 200.0;
	y_min = -200.0;

	// ͼ�ϣ����ָ�
	x_Divisions = 10;
	y_Divisions = 10;
	x_miniPerSec = 5;
	y_miniPerSec = 5;

	// �������ߵĻ���
	x_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // x��
	y_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // y��
	h_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // �����
	v_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // ������
	x_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // x���Ͽ̶�
	y_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // y���Ͽ̶�
	uptLine.SetStyle(1, PS_SOLID, RGB(255, 255, 0));   // �û�ָ�����ʮ��
	bHGrig = TRUE;
	bVGrid = TRUE;
	

	// ���ø��������õ�HDC
	userPointsLabel.pChart = this;   // �û���ı�ǩ
	TitleLabel.pChart = this;
	x_AxisTitle.pChart = this;       // x�����
	y_AxisTitle.pChart = this;       // y�����
	x_AxisLabel.pChart = this;       // ���洢�ַ�����ֻ�洢��ɫ������
	y_AxisLabel.pChart = this;
	TipLabel.pChart = this;

	// ����������������ɫ
	TitleLabel.SetColor(RGB(255, 255, 0));
	x_AxisTitle.SetColor(RGB(255, 255, 0)); // 128
	y_AxisTitle.SetColor(RGB(255, 255, 0));
	x_AxisLabel.SetColor(RGB(255, 255, 0));
	y_AxisLabel.SetColor(RGB(255, 255, 0));

	// λ��������ز���
	TitleLabel.SetFontSize(11);
	x_AxisTitle.SetFontSize(9);
	y_AxisTitle.SetFontStyle(9, TRUE);
	TitleLabel.text = "Chart1.0 ��������ͼ";
	x_AxisTitle.text = "X��/ms";
	y_AxisTitle.text = "Y��";

	x_AxisLabel.SetFontSize(9);
	y_AxisLabel.SetFontSize(9);
	x_AxisLabel.text = "1234.567";   // 
	y_AxisLabel.text = "1234.567";   // �������������Ҫ�Ŀ�ȣ�4�����֣��������һ��

	
	TipLabel.text = "λ����Ϣ";
	TipLabel.SetFontSize(9);
	TipLabel.rt = CRect(0, 0, 0, 0);
	TipLabel.SetColor(RGB(255, 255, 255));

	// �û�ָ����ı�ǩ
	userPointsLabel.text = "λ����Ϣ";
	userPointsLabel.SetFontSize(9);
	userPointsLabel.SetColor(RGB(255, 255, 0));
	userPointsLabel.rt = CRect(0, 0, 0, 0);


//	shouldReDrawChart = FALSE;       // ��y���ǩ����Ԥ�ڿ��ʱ�����»�����
	isMagnified = FALSE;             // �Ѿ��Ŵ��˱��
	isMagnifying = FALSE;
	nChartType = 1;                  // ����ͼ
	AxisSecendVisible = TRUE;        // �Ƿ�Ҫ������������
	
	bCanMagnify = TRUE;              //�Ƿ���ԷŴ�ֱ��������

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
	// ��ʼ����ͼ��Դ������λͼ
	if (pMemDC == NULL)
	{
		pMemDC = new CDC();
		pMemDC->CreateCompatibleDC(NULL);  
	}	
	if (m_pBackGround == NULL)
	{
		::GetClientRect(pDC->GetWindow()->m_hWnd,&rt_Back);//��ȡ����RX��:Static���ͻ����Ĵ�С////��߿�ʼ��������
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
	// ��ʼ����ͼ��Դ��ǰ��λͼ
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

// �������ã����²���Ҳ�������û������ã�Ȼ�����initChart()

	x_minSel = x_min;
	x_maxSel = x_max;
	y_minSel = y_min;
	y_maxSel = y_max;

//	x_LabelFormat = "%.0f";   // x���ǩ�ĸ�ʽ���ַ���
//	y_LabelFormat = "%.3f";   // y���ǩ�ĸ�ʽ���ַ���


	ModifyStyle(wanglifan, NULL, SS_NOTIFY, 0);
	hCurCross = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
	hCurHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));//MAKEINTRESOURCE(IDC_APPSTARTING)

	// ��Ǽ���������ֵ�λ�ã�����InitChart()
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
	// ��ʼ���������ֵ�λ�ã���һ�λ�ͼʱ����ã��Լ����û������ò�������ã��Ա��ض�λ����Ԫ��λ�ã�
	::GetClientRect(wanglifan,&rt_Back);//��Rx���ľ��η���rt_back
	::GetClientRect(wanglifan,&rt_Plot);//��Rx���ľ��η���rt_plot
	rt_Plot.left += 6;//���ñ�����Ĵ�С
	rt_Plot.top += 5;
	rt_Plot.bottom -= 5;
	rt_Plot.right -= 25;
	
	SIZE size;
	// ����Ĳ���
	TitleLabel.GetStringSize(pMemDC->m_hDC, TitleLabel.text, TitleLabel.text.GetLength(), &size);
	TitleLabel.rt = CRect(0, 0, rt_Back.Width(), size.cy + 10);
	rt_Plot.top += TitleLabel.rt.Height();       // �����������ľ���
	
	// X�����Ĳ���
	x_AxisTitle.GetStringSize(pMemDC->m_hDC, x_AxisTitle.text, x_AxisTitle.text.GetLength(), &size);
	x_AxisTitle.rt = CRect(0, rt_Back.Height() - size.cy, rt_Back.Width(), rt_Back.Height());
	rt_Plot.bottom -= x_AxisTitle.rt.Height();  // �����������ľ���
	
	// Y�����Ĳ���
	y_AxisTitle.GetStringSize(pMemDC->m_hDC, y_AxisTitle.text, y_AxisTitle.text.GetLength(), &size);
	y_AxisTitle.rt = CRect(0, 0, size.cy, rt_Back.Height());     // ��ֱ��
	rt_Plot.left += size.cy;             // �����������ľ���
	
	// ͨ��x���ϱ�ǩ�ĸ߶ȣ��õ�plot���յ�λ��
	x_AxisLabel.GetStringSize(pMemDC->m_hDC, x_AxisLabel.text, x_AxisLabel.text.GetLength(), &size);
	rt_Plot.bottom -= size.cy;          // �����������ľ���

	// ͨ��y���ϱ�ǩ�Ŀ�ȣ��õ�plot���յ�λ��
//	y_AxisLabel.SetFontSize(9, pMemDC->m_hDC);
	y_AxisLabel.GetStringSize(pMemDC->m_hDC, y_AxisLabel.text, y_AxisLabel.text.GetLength(), &size);
	rt_Plot.left += size.cx;

	// ��������
	isMagnified = FALSE;             // �Ѿ��Ŵ��˱��
	isDraging = FALSE;
	isMagnify = FALSE;
	x_minSel = x_min;
	x_maxSel = x_max;
	y_minSel = y_min;
	y_maxSel = y_max;

	shouldReDrawChart = TRUE;        // ������»�����
}

BOOL CChartRx::DrawChart()
{
	CBitmap   *pOldBit = pMemDC->SelectObject(m_pBackGround); //ѡȡ����bitͼ���������DC��Pmemdc��ֵ
	pMemDC->FillSolidRect(0, 0, rt_Back.Width(), rt_Back.Height(), crBackGround); 
	if (isIniting)
	{
		isIniting = FALSE;
		InitChart();   // ����������ֵ�λ�ã��õ�plotλ��
	}
	// title����
	DrawTitle();
	DrawXTitle();
	DrawYTitle();

	// ����x��y����ʵ����ֵ��������label
	double x_addData = (x_maxSel - x_minSel) / x_Divisions;
	double y_addData = (y_maxSel - y_minSel) / y_Divisions;
	CString LabelText;
	// y ��
	BOOL ret;
	HGDIOBJ pPenOld = pMemDC->SelectObject(y_Axis.pen);
	pMemDC->MoveTo(rt_Plot.left, rt_Plot.bottom + MajorTick);
	pMemDC->LineTo(rt_Plot.left, rt_Plot.top);
	LabelText.Format(y_LabelFormat, y_minSel);
	ret = DrawYLabel(LabelText, rt_Plot.left, rt_Plot.bottom);
	if (!ret)
		return ret;          // ����Y��ǩ�Ŀ�����¼���λ��

	// x ��
	pMemDC->SelectObject(x_Axis.pen);
	pMemDC->MoveTo(rt_Plot.left - MajorTick, rt_Plot.bottom);
	pMemDC->LineTo(rt_Plot.right, rt_Plot.bottom);
	LabelText.Format(x_LabelFormat, x_minSel);
	ret = DrawXLabel(LabelText, rt_Plot.left, rt_Plot.bottom, 0);
	if (!ret)
		return ret;

	// grid ����
	double addPix = (float)rt_Plot.Width() / (float)(x_Divisions);  // x����ͼ�ϵ�����
	int i = 0, j = 0;
	double tempPix = rt_Plot.right;
	double tempData = x_maxSel;
	// ������С�̶ȵı���
	double tempDegreeAdd;
	double tempDegreePix; 
	int lableCount = 0;
	for (i=x_Divisions; i>0; i--)
	{	
		if (bVGrid)   // �Ƿ�������
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
		// ��ϸ�̶���
		tempDegreeAdd = addPix / x_miniPerSec;  // һ���̶ȵ���������
		tempDegreePix = tempPix; 
		//pMemDC->SelectObject(x_Degree.pen);
		for (j=1; j<x_miniPerSec; j++)
		{
			tempDegreePix += tempDegreeAdd;
			pMemDC->MoveTo((int)tempDegreePix , rt_Plot.bottom);
			pMemDC->LineTo((int)tempDegreePix, rt_Plot.bottom + MiniTick);
		}
	}
	// grid ����
	addPix = (float)rt_Plot.Height() / (float)(y_Divisions);  // y����ͼ�ϵ�����
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
		// ��ϸ�̶���
		tempDegreeAdd = addPix / y_miniPerSec;  // һ���̶ȵ���������
		tempDegreePix = tempPix; 
		pMemDC->SelectObject(y_Degree.pen);
		for (j=1; j<y_miniPerSec; j++)
		{
			tempDegreePix -= tempDegreeAdd;
			pMemDC->MoveTo(rt_Plot.left, (int)tempDegreePix);
			pMemDC->LineTo(rt_Plot.left - MiniTick, (int)tempDegreePix);
		}
	}
	// ���������ᣬ���ұߵ�Y��
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
		//temp.Format("��ˢ�µ���Ϣ  %d,  %d", rt_Back.Width(), rt_Back.Height());
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
		
		// ���ڱ��ʱ�����¼���λ�ã����ǲ���Ҫ�ı�ѡ��Χ
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

	static int x_label_left;         // ���ǰһ�λ�ʱ������Ƕ��٣��ж��Ƿ��ص�
	static double x_label_val;       // ǰһ�ε�ֵ

	SIZE size;
	int nLabels = xLabelPoints.GetPointCount();
	// ���û��ָ���Լ��ı�ǩ���͸��ݷ�Χ���㡣
	if (nLabels == 0)
	{	
		x_AxisLabel.GetStringSize(pMemDC->m_hDC, str, str.GetLength(), &size);
		int top = rt_Back.Height() - x_AxisTitle.rt.Height() - size.cy;
		int left = x_Mark - size.cx/2;
		x_AxisLabel.rt = CRect(left, top, left + size.cx, top + size.cy);
		x_AxisLabel.ShowText(pMemDC->m_hDC, str,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// �ж��ص�
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
		// �ж��ص�����
	}
	else
	{
		if ((nLabels - 1) < index)    // �����ǩ����С��Ҫ�������ֵ���������
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
		// Ĭ�ϵģ����Է�"1234.567��ô����8�ֽڣ�������ˣ���Ҫ���¼���λ�ã��ٻ�һ��
		y_AxisLabel.text = str; // �������һ��
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
	if (shouldReDrawChart)         // ������»�������λ�ã�initChart(),
	{
		ret = DrawChart();         // ��plot����ı���
		if (!ret)
		{
			double x1 = x_minSel;
			double x2 = x_maxSel;
			double y1 = y_maxSel;
			double y2 = y_minSel;
			InitChart();           // ���¼����С
			x_minSel = x1;
			x_maxSel = x2;
			y_maxSel = y1;
			y_minSel = y2;
			ret = DrawChart();
		}
		if (nChartType == 1)           // ��plot��������ͼ
			DrawLineChart();
		if (nChartType == 2)           // ��plot����ֱ��ͼ
			DrawChartHistogram();

		//DrawUserPoint();
	}
	shouldReDrawChart = FALSE;
}

// ������ת��Ϊͼ�ϵ�����ƫ�ƣ�0Ϊx�ᣬ1Ϊy��
// ���������ʱ��ʹ��
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

// ��ͼ�ϵ�����ƫ��ת��Ϊ���ݣ�0Ϊx�ᣬ1Ϊy��
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
	CString temp = AddFormat(x_LabelFormat) + "�� " + AddFormat(y_LabelFormat);
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
	// �ӱ���ͼֱ�ӿ������ݣ������˻�������ʱ��
//	pTempDC->SelectObject(m_pScreen);
	pMemDC->SelectObject(m_pBackGround);
//	pTempDC->BitBlt(0, 0, rt_Back.Width(), rt_Back.Height(), pMemDC, 0, 0, SRCCOPY); 

	pDC->BitBlt(0, 0, rt_Back.Width(), rt_Back.Height(), pMemDC, 0, 0, SRCCOPY); 

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, crForeGround);
	if (isMagnifying)  // ������
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

		// �������ߵĻ���
	x_Axis.SetStyle(1, PS_SOLID, RGB(0, 80, 0));         // x��
	y_Axis.SetStyle(1, PS_SOLID, RGB(0, 80, 0));         // y��
	h_Grid.SetStyle(1, PS_DOT, RGB(0, 80, 0));           // �����
	v_Grid.SetStyle(1, PS_DOT, RGB(0, 80, 0));           // ������
	x_Degree.SetStyle(1, PS_SOLID, RGB(0, 80, 0));       // x���Ͽ̶�
	y_Degree.SetStyle(1, PS_SOLID, RGB(0, 80, 0));       // y���Ͽ̶�

	SetRedraw(FALSE);         // ������Ļ��ˢ��
	shouldReDrawChart = TRUE; // ���ڴ������»�һ��
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

	// �������ߵĻ���
	x_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // x��
	y_Axis.SetStyle(1, PS_SOLID, RGB(0, 192, 0));         // y��
	h_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // �����
	v_Grid.SetStyle(1, PS_DOT, RGB(0, 192, 0));           // ������
	x_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // x���Ͽ̶�
	y_Degree.SetStyle(1, PS_SOLID, RGB(0, 192, 0));       // y���Ͽ̶�


	shouldReDrawChart = TRUE;
	DrawPlot();
	SetRedraw(TRUE);         // ������Ļ��ˢ��
	


}

BOOL CChartRx::SaveBmp(LPCTSTR lpFileName)
{
	int iBits;                 // ��ǰ�ֱ�����ÿ������ռ�ֽ���
	WORD wBitCount;            // λͼ��ÿ������ռ�ֽ���
	DWORD dwPaletteSize = 0;   // �����ɫ���С�� 
	DWORD dwBmBitsSize = 0;    // λͼ�������ֽڴ�С  
	DWORD dwDIBSize = 0;       // λͼ�ļ���С
	DWORD dwWritten = 0;       // д���ļ��ֽ���
	BITMAP Bitmap;             // λͼ���Խṹ
	BITMAPFILEHEADER bmfHdr;   // λͼ�ļ�ͷ�ṹ 
	BITMAPINFOHEADER bi;       // λͼ��Ϣͷ�ṹ
	
	LPBITMAPINFOHEADER lpbi;   //ָ��λͼ��Ϣͷ�ṹ 
	
	HANDLE fh = NULL;          // �����ļ��� 
	HANDLE hDib = NULL;        //�����ڴ�����
	HANDLE hPal = NULL;        //��ɫ����
	HANDLE hOldPal = NULL; 
	
	//����λͼ�ļ�ÿ��������ռ�ֽ��� 
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
	
	//Ϊλͼ���ݷ����ڴ� 
	hDib = ::GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	*lpbi = bi; 
	
	// �����ɫ�� 
	hPal = ::GetStockObject(DEFAULT_PALETTE); 
	if (hPal) 
	{ 
		hOldPal = ::SelectPalette(pMemDC->m_hDC, (HPALETTE)hPal, FALSE); 
	}
	
	// ��ȡ�õ�ɫ�����µ�����ֵ 
	::GetDIBits(pMemDC->m_hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 
	
	//�ָ���ɫ�� 
	if (hOldPal) 
	{ 
		::SelectPalette(pMemDC->m_hDC, (HPALETTE)hOldPal, TRUE); 
		::RealizePalette(pMemDC->m_hDC); 
	} 
	
	//����λͼ�ļ� 
	fh = ::CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
	
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE; 
	
	// ����λͼ�ļ�ͷ 
	bmfHdr.bfType = 0x4D42; // "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize; 
	bmfHdr.bfSize = dwDIBSize; 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 
	// д��λͼ�ļ�ͷ 
	::WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
	// д��λͼ�ļ��������� 
	::WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 
	//��� 
	::GlobalUnlock(hDib); // ����
	::GlobalFree(hDib);   // �ͷ��ļ����ݴ洢����
	::CloseHandle(fh);    // �ر��ļ�
	
	return TRUE;
}

void CChartRx::SetSerialCount(int count)
{
	// һ���ߣ�����˵һ��ͨ����һ�����ԣ�һ������Ϊһ�������꣬
	CChartSerial * serial;
	if (this->SerialCount > 0)
	{
		for (int i=0; i<SerialCount; i++)
		{
			serial = GetSerial(i);//��ȡSerials�е�ֵ
			delete serial;
		}
		this->Serials.RemoveAll();//�Ƴ�Serials
	}
//	this->Serials.SetSize(0, 5);
	this->SerialCount = count;
	int index;
	for (int j=0; j<SerialCount; j++)
	{
		serial = new CChartSerial();//��̬����ͨ���洢�ռ�
		index = this->Serials.Add(serial);//�˴���ͨ˫ͨ�� �����µ�Serials
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

CChartSerial * CChartRx::GetSerial(int index)//ʵ�ֵ��ǽ�CChartRx��ֵ���ص�CChartSerial��
{
	if (index > (SerialCount-1))
	{
		return NULL;
	}
	return (CChartSerial *)(this->Serials.GetAt(index));
}

// ����ͼ����ǰ��ͼ�ϻ��ú�����������ȥ��
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
		pSerial = this->GetSerial(i);	//	����ͨ��1�ĵ�ַ
		if (pSerial == NULL)     // �����ˣ�����
		{	
			MessageBox("�õ�������Ϣ");
			return;          
		}

		CPen * pPenSerial = new CPen();
		pPenSerial->CreatePen(PS_SOLID, 1, pSerial->crSerial);

		HGDIOBJ pObj = pTempDC->SelectObject(pPenSerial);

		if (pSerial->GetPointCount() > 0)  //��ȡͨ��1�е����ݵĳ���
		{

#ifdef DEBUG_OUTPUT
				text.Format("### ͨ��%2d ###\r\n", i);
				mFile.WriteString(text);
#endif
				
			datapoint = pSerial->GetPoint(0);//��ȡͨ��1��O�ڵ㴦������
			x = GetPixFromValue1(0, datapoint->x);          // ����ʹ���µĺ�����������Խ��ʱ��ʧ��
			y = GetPixFromValue1(1, datapoint->y);
			pTempDC->MoveTo(x, y);
			for (int j=1; j<this->m_nIndex; j++)//pSerial->GetPointCount()����m_nindex
			{
				datapoint = pSerial->GetPoint(j);
				x = GetPixFromValue1(0, datapoint->x);//��Ҫ��̬ˢ�²��Σ������Ǳ߲ɼ���ɺ�����߶�̬ˢ�¡����߶�ʱ�����ʲ�һ��һ������ɿ��ǡ��ּٶ����߶�ʱ������һ������߰�һ������m_index+m_step��ʾ���Ǳ߰�һ�����ʴ������2.����˵����Ҫ��ʱ����ֻ��Ҫһ��PV�������߻��棬�Ǳ�160���������˺�ͷ����źŽ���ˢ�¡���������Ϣ��������ͼ�̲߳�������while���ȴ��Ǻ�P�������������ͷ��ͼǺ�P
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

// ��ֱ��ͼ����ǰ��ͼ�ϻ��ú�����������ȥ��
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
		if (pSerial == NULL)     // �����ˣ�����
		{	
			MessageBox("�����ڲ����󣺵õ�������Ϣ����");
			return;          
		}
		CPen * pPenSerial = new CPen();
		pPenSerial->CreatePen(PS_SOLID, 1, pSerial->crSerial);

		HGDIOBJ pObj = pTempDC->SelectObject(pPenSerial);

		if (pSerial->GetPointCount() > 0)
		{
			double addPix = (float)rt_Plot.Width() / (float)(x_Divisions);  // x����ͼ�ϵ�����
			double addPix1;
			CRect rt;

			for (int j=0; j<pSerial->GetPointCount(); j++)
			{
				datapoint = pSerial->GetPoint(j);
				x = GetPixFromValue1(0, datapoint->x);   // ����
				y = GetPixFromValue1(1, datapoint->y);   // �߶�
				y0 = GetPixFromValue1(1, 0);             // ��
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

// ���㣬�ߵ�ʱ��ʹ��
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

// �������ͣ���ˢ��
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
		bCanMagnify = FALSE;      // ���÷Ŵ�
	}
	else
	{
	}
	
	this->nChartType = type;
	userPoints.Clear();   // ����û���ǵĵ�
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
	x_LabelFormatSel = x_LabelFormat = x;   // x���ǩ�ĸ�ʽ���ַ��� 
	y_LabelFormatSel = y_LabelFormat = y;   // y���ǩ�ĸ�ʽ���ַ���
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
	// ���λ�ò��ڷ�Χ�ھͷ��أ�
	if ((point.x <= rt_Plot.left) || (point.y <= rt_Plot.top) || (point.x >= rt_Plot.right) || (point.y >= rt_Plot.bottom))
		return;

	double x1, y1;
	x1 = GetValueFormPix(0, point.x);
	y1 = GetValueFormPix(1, point.y);

	// ��һ���㣬���£�������Ǹ����꣬���ƶ�ʱ��������⣬���Խ�������Ǵ��ڱ��ξͺ��鷳
	this->userPoints.AddPoint(x1, y1, 0);
	shouldReDrawChart = TRUE;
	Invalidate();
	return;

	// ����Ϊ����ʹ��


	CString temp;
	//temp.Format("%d, %d", point.x, point.y);
	CString format = x_LabelFormat + ", " + y_LabelFormat;
	temp.Format(format, x1, y1);

	MessageBox(temp);

	
//	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChartRx::DrawUserPoint()//��̬ˢ�������
{
	CBitmap   *pOldBit = pMemDC->SelectObject(m_pBackGround); 

	CString strFormat = AddFormat(x_LabelFormat) + "�� " + AddFormat(y_LabelFormat);//CString �������ַ�������
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

			
	 // ��ʱ����һ�������
		userPointsLabel.GetStringSize(pMemDC->m_hDC, str, str.GetLength(), &size);
		userPointsLabel.rt = CRect(x1, y1, x1 + size.cx, y1 + size.cy);
		userPointsLabel.ShowText(pMemDC->m_hDC, str,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
	}
}

// ����һ��ͼ��ʾ��ʱ��������Դ
void CChartRx::ClearPlot()
{

}

// ����x���ָ����־�������ȵ����
void CChartRx::ClearXLabels()
{
	// ����󣬸��������Сֵ�����ǩ
	xLabelPoints.Clear();
	bShowTip = TRUE;

}

// ���һ��x���ϵ�ָ����ǩ
void CChartRx::AddXLabel(double xVal)
{
	// ��Ϊ��ʱ��ʹ����������ǩ�������ң�
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
		x_AxisTitle.SetText(str);      // x�����
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

// ��������еĵ�
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

	if (this->SerialCount > i)      // i>=0, ������һ��ͨ�� 
	{
		serial = GetSerial(i);
		serial->Clear();
	}
}
// ������ֵ�Ĵ�Сȷ���ֱ���
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

// �������е������������С
HDC CChartRx::GetDC()
{
	if (pMemDC)
	return pMemDC->m_hDC;

	return NULL;
}

void CChartRx::SetNTickOneLabel(int x, int y)
{
	n_x_TickOneLabel = x;      // ������̶�һ����ǩ
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


void CChartRx::belief(CDC* pDC)//��ֲondraw�Ĺ���
{
//pDC->MoveTo(15,15);
//pDC->LineTo(200,200);
//	CPaintDC *dc; // device context for painting
	if (isFirstTime)     // �Ƴٵĳ�ʼ����ֻ��һ�Σ�
	{
		Init(pDC);          // Ĭ�ϵĳ�ʼ����������
		isFirstTime = FALSE;
		shouldReDrawChart = TRUE;
	}
	
	if ((pMemDC != NULL) && (m_pBackGround != NULL))
	{	
		DrawPlot();
		DrawScreen(pDC);               // д��Ļ
	}	
//	if (TimerOnTag==TRUE)
//		jisuji1->SetUpdateRate(50);
}

void CChartRx::LButtonDbC(UINT nFlags, CPoint point,CScrollChart *jisuji)
{
// MessageBox("�ҽ�EDword..");
	// TODO: Add your message handler code here and/or call default
 	x_minSel = x_min;
 	x_maxSel = x_max;
 	y_maxSel = y_max;
	y_minSel = y_min;
	isMagnified = FALSE;
 
	x_LabelFormat = x_LabelFormatSel;
	y_LabelFormat = y_LabelFormatSel;

 	n_x_TickOneLabel = n_x_TickOneLabelSel;   // ������̶�һ����ǩ
 	n_y_TickOneLabel = n_y_TickOneLabelSel;
 		
 	userPoints.Clear();  // ����û���ǵĵ�
 	shouldReDrawChart = TRUE;
 	jisuji->Invalidate();
 	jisuji->SetFocus();
 
}

void CChartRx::LButtonD(UINT nFlags, CPoint point,CScrollChart *jisuji)
{
//	MessageBox("���");
	jisuji->SetFocus();
 	if (!bCanMagnify)   // �����ԷŴ󣬲�����
 		return;

	isMagnifying = TRUE;
 	ptDown = point;
 	shouldReDrawChart = TRUE;
 //	CStatic::OnLButtonDown(nFlags, point);
}



void CChartRx::LButtonU(UINT nFlags, CPoint point,CScrollChart *jisuji)
{
 //	MessageBox("tanqi");
	if (!bCanMagnify)   // �����ԷŴ󣬲�����
 		return;
 
	if (((point.x - ptDown.x) < 5) || ((point.y - ptDown.y)) < 5)
	{
 		isMagnifying = FALSE;//����ʧ�����
		jisuji->Invalidate();
 //		CStatic::OnLButtonUp(nFlags, point);
		return;
 	}
 	double x0 = GetValueFormPix(0, ptDown.x);
 	double x1 = GetValueFormPix(0, point.x);
 	double y1 = GetValueFormPix(1, ptDown.y);
 	double y0 = GetValueFormPix(1, point.y);
 //	CString temp;
//	temp.Format(x_LabelFormat +"��" + y_LabelFormat, GetValueFormPix(0, point.x), GetValueFormPix(1, point.y));
 //	temp.Format("%d  %d", point.x, point.y);
 //	MessageBox(temp);
 	x_minSel = x0;
	x_maxSel = x1;
 	y_minSel = y0;
 	y_maxSel = y1;
 
 	userPoints.Clear();  // ����û���ǵĵ�
	isMagnify = TRUE;
 	isMagnifying = FALSE;
	shouldReDrawChart = TRUE;
 
	x_LabelFormat = AddFormat(x_LabelFormat, x_max - x_min, x_maxSel-x_minSel);
 	jisuji->Invalidate();
 //	CStatic::OnLButtonUp(nFlags, point);
 
	isMagnify = FALSE;
	isMagnified = TRUE;
 	// ������Ҫע�͵�������Ŵ��������!
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
	// ���ڷŴ�Ĳ���
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
		::ClipCursor(&rt);  //ǿ��λ�����
		
	}
	else
		ClipCursor(NULL);
	ptCur = point;

	// �����϶��Ĳ���
	if (isDraging)
	{
		::SetCursor(hCurHand);
//		int x_off = point.x - ptDown.x;
//		int y_off = point.y - ptDown.y;
//
//		// �ı��û�ָ����ǩ�����꣬���ƶ���;
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
		// ֻҪ�������϶�����������ͼ��Ͳ���Ҫ�ػ������Ǵ��ڴ�С�ı�ʱ�������⣬
		// ��������䣬ˢ��ʧ�ܣ�
		shouldReDrawChart = FALSE;
		ClipCursor(NULL);
	}

	// �������ڻ滭�����ͻ���ʾ
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
//////////////////////////////////////ͬ�����ݶ���////////////////////////////////////////////////////////////////
	for (int k=0; k < mSTEPS; k++)//2000   ����������/10=���õĶ�ʱ��������*mSTEP
	{
		ALL_mChart->AddSerialPoint(0, tmpcnt*0.125, (float)(TmpBuffer[tmpcnt%BUFFER_SIZE/2]/10), tmpcnt);
		tmpcnt++;
	}
		//	ALL_mChart->SetChartType(1);
///////////////////////////////////////////////////////////////////////////////////////////////////////
CChartSerial *pserial =this->GetSerial(0);//������ˣ�ֻȡһ��ͨ������

if(m_nIndex < mMAXSIZE- mSTEPS)//if(m_nIndex < pserial->GetPointCount() - mSTEPS)//������1
	{
		m_nIndex+=mSTEPS;//������++
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
