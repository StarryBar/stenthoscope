#if !defined(AFX_CHARTRX_H__93581DF5_5D59_4980_87F6_74C660302015__INCLUDED_)
#define AFX_CHARTRX_H__93581DF5_5D59_4980_87F6_74C660302015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartRx.h : header file
//

#include <afxcoll.h >
/////////////////////////////////////////////////////////////////////////////
// CChartRx window
#include "StdAfx.h"
#include "ChartLine.h"
#include "ChartSerial.h"
#include "ScrollChart.h"// 2



class CChartRx : public CScrollChart// 1
{

public:
	CChartRx();
   UINT   m_nIndex;
   

public:

	BOOL bShowLegend;       // 是否显示图例
	BOOL bShowTickXMajor;
	BOOL bShowTickXMin;
	BOOL bShowTickYMajor;
	BOOL bShowTickYMin;
	BOOL bCanMagnify;
	
	CBitmap *m_pBackGround;  // 背景图
	CBitmap *m_pForeGround;  // 加上线或者图的部分
	CBitmap *m_pScreen;
	CRect rt_Back;           // 整个客户区大小
	CDC  * pMemDC;           // 用来画图
	CDC  * pTempDC; 
	COLORREF crBackGround;   // 画布
	COLORREF crForeGround;
	int Fontsize;
	CPen pen;
	CBrush *pBrush;

	COLORREF crTitle;
	CRect rtTitle;
	
	int MajorTick;
	int MiniTick;
	int x_Divisions;
	int y_Divisions;
	int x_miniPerSec;
	int y_miniPerSec;
	double x_max;             // 画图最关键的值
	double x_min;
	double y_max;
	double y_min;
	double x_maxSel;          // 缩放时候选择的值
	double x_minSel;
	double y_maxSel;
	double y_minSel;
	BOOL isMagnifying;       // 正在放大时候，画线
	BOOL isMagnified;        // 用来标记，是否被放大了
	BOOL isMagnify;          // 放大图的一刻，刷新
	CPoint  ptDown;           // 放大时候使用，
	CPoint ptCur;
	BOOL isDraging;          // 是否在使用右键拖动

	BOOL AxisSecendVisible;
	int  nChartType;         // 图的类型，线:1，直方:2
	CString x_LabelFormat;   // x轴标签的格式化字符串
	CString y_LabelFormat;   // y轴标签的格式化字符串
	CString x_LabelFormatSel;   // x轴标签的格式化字符串
	CString y_LabelFormatSel;   // y轴标签的格式化字符串

	CString tip_x;           // 打标和鼠标位置信息的格式化信息
	CString tip_y;
	int     n_x_Format;      // 用这个值来格式化信息
	int     n_y_Format; 

	int  n_x_TickOneLabel;
	int  n_y_TickOneLabel;
	int  n_x_TickOneLabelSel;  // 旧的
	int  n_y_TickOneLabelSel;
	BOOL bShowTip;

	//CChartRx* this
// 各个单元的存储的类
	CPtrArray Serials;       // 用来保存线条，或者说通道
	int SerialCount;         // 线条数目
//	int chartType;

//	CToolTipCtrl m_Tip;
	HCURSOR hCurCross;
	HCURSOR hCurHand;
	
	CChartSerial userPoints;      // 用户指定要标记的点，在图形大小改变时候消失
	CChartSerial xLabelPoints;    // 当x轴显示值不是均匀的时候，根据这个链表

	CChartLabel userPointsLabel;  // 用户点的标签
	CChartLabel TitleLabel;
	CChartLabel x_AxisTitle;      // x轴标题
	CChartLabel y_AxisTitle;      // y轴标题
	CChartLabel x_AxisLabel;      // 不存储字符串，只存储颜色和字体
	CChartLabel y_AxisLabel;
	CChartLabel TipLabel;
	BOOL isIniting;
	BOOL isFirstTime;
	BOOL shouldReDrawChart;
	BOOL bHGrig;
	BOOL bVGrid;


	CRect rt_Plot;           // 表格矩形区域
	CChartLine x_Axis;       // x轴
	CChartLine y_Axis;       // y轴
	CChartLine h_Grid;       // 横格线
	CChartLine v_Grid;       // 竖格线
	CChartLine x_Degree;     // x轴上刻度
	CChartLine y_Degree;     // y轴上刻度
	CChartLine uptLine;      // 用户指定点的十字
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartRx)
	protected:
	virtual	void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnButtonSaveEnd();
	void OnButtonSaveStart();
	void MMTimer();
	void MouseM(UINT nFlags, CPoint point,CScrollChart *jisuji);
	void RButtonU(UINT nFlags,CPoint point,CScrollChart *jisuji);
	void RButtonD(UINT nFlags,CPoint point,CScrollChart *jisuji);
	void LButtonU(UINT nFlags,CPoint point,CScrollChart *jisuji);
	void LButtonD(UINT nFlags, CPoint point,CScrollChart *jisuji);
	void LButtonDbC(UINT nFlags, CPoint point,CScrollChart *jisuji);
	HWND wanglifan;
	void belief(CDC* pDC);
	CString AddFormat(CString data, double val1, double val2);
	double ABS(double val);
	void SetNTickOneLabel(int x, int y);
	CString AddFormat(CString data);
	HDC GetDC();
	void SetTickFormat(CString x, CString y);
	void SetYDivision(int major, int mini);
	void SetXDivision(int major, int mini);
	void SetYScale(double min, double max);
	void SetXScale(double min, double max);
	void SetChartType(int type);

	CChartSerial * GetSerial(int index);
	void SetSerialColor(int index, COLORREF cr);
	void SetSerialCount(int count);
	
	BOOL SaveBmp(LPCTSTR  Filepath);
	void SaveBmpEx(LPCTSTR lpFileName);

	CString GetFormatFormVal(double val);
	void ClearSerialPoints(int i);

	void SetTitleText(int index, CString str);
	void AddXLabel(double xVal);
	void ClearXLabels();

	void Updata();
	void DrawLineChart();
	void DrawPlot();
private:
	UINT tmpcnt;
	double GetPixFromValue1(int nAxis, double value);
	void DrawChartHistogram();

	void DrawScreen(CDC *pDC);
	void DrawTip(int x, int y);
	double GetValueFormPix(int nAxis, double pix);
	double GetPixFromValue(int nAxis, double value);

	BOOL DrawYLabel(CString str, int x_Mark, int y_Mark);
	void InitChart();
	BOOL DrawXLabel(CString str, int x_Mark, int y_Mark, int index);
	void DrawYTitle();
	void DrawXTitle();
	void DrawTitle();
	BOOL DrawChart();
	BOOL Init(CDC *pDC);
	void ClearPlot();
	void DrawUserPoint();
	
	virtual ~CChartRx();


	//

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartRx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()




	// Overrides
	// ClassWizard generated virtual function overrides

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTRX_H__93581DF5_5D59_4980_87F6_74C660302015__INCLUDED_)
