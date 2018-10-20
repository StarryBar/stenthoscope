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

	BOOL bShowLegend;       // �Ƿ���ʾͼ��
	BOOL bShowTickXMajor;
	BOOL bShowTickXMin;
	BOOL bShowTickYMajor;
	BOOL bShowTickYMin;
	BOOL bCanMagnify;
	
	CBitmap *m_pBackGround;  // ����ͼ
	CBitmap *m_pForeGround;  // �����߻���ͼ�Ĳ���
	CBitmap *m_pScreen;
	CRect rt_Back;           // �����ͻ�����С
	CDC  * pMemDC;           // ������ͼ
	CDC  * pTempDC; 
	COLORREF crBackGround;   // ����
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
	double x_max;             // ��ͼ��ؼ���ֵ
	double x_min;
	double y_max;
	double y_min;
	double x_maxSel;          // ����ʱ��ѡ���ֵ
	double x_minSel;
	double y_maxSel;
	double y_minSel;
	BOOL isMagnifying;       // ���ڷŴ�ʱ�򣬻���
	BOOL isMagnified;        // ������ǣ��Ƿ񱻷Ŵ���
	BOOL isMagnify;          // �Ŵ�ͼ��һ�̣�ˢ��
	CPoint  ptDown;           // �Ŵ�ʱ��ʹ�ã�
	CPoint ptCur;
	BOOL isDraging;          // �Ƿ���ʹ���Ҽ��϶�

	BOOL AxisSecendVisible;
	int  nChartType;         // ͼ�����ͣ���:1��ֱ��:2
	CString x_LabelFormat;   // x���ǩ�ĸ�ʽ���ַ���
	CString y_LabelFormat;   // y���ǩ�ĸ�ʽ���ַ���
	CString x_LabelFormatSel;   // x���ǩ�ĸ�ʽ���ַ���
	CString y_LabelFormatSel;   // y���ǩ�ĸ�ʽ���ַ���

	CString tip_x;           // �������λ����Ϣ�ĸ�ʽ����Ϣ
	CString tip_y;
	int     n_x_Format;      // �����ֵ����ʽ����Ϣ
	int     n_y_Format; 

	int  n_x_TickOneLabel;
	int  n_y_TickOneLabel;
	int  n_x_TickOneLabelSel;  // �ɵ�
	int  n_y_TickOneLabelSel;
	BOOL bShowTip;

	//CChartRx* this
// ������Ԫ�Ĵ洢����
	CPtrArray Serials;       // ������������������˵ͨ��
	int SerialCount;         // ������Ŀ
//	int chartType;

//	CToolTipCtrl m_Tip;
	HCURSOR hCurCross;
	HCURSOR hCurHand;
	
	CChartSerial userPoints;      // �û�ָ��Ҫ��ǵĵ㣬��ͼ�δ�С�ı�ʱ����ʧ
	CChartSerial xLabelPoints;    // ��x����ʾֵ���Ǿ��ȵ�ʱ�򣬸����������

	CChartLabel userPointsLabel;  // �û���ı�ǩ
	CChartLabel TitleLabel;
	CChartLabel x_AxisTitle;      // x�����
	CChartLabel y_AxisTitle;      // y�����
	CChartLabel x_AxisLabel;      // ���洢�ַ�����ֻ�洢��ɫ������
	CChartLabel y_AxisLabel;
	CChartLabel TipLabel;
	BOOL isIniting;
	BOOL isFirstTime;
	BOOL shouldReDrawChart;
	BOOL bHGrig;
	BOOL bVGrid;


	CRect rt_Plot;           // ����������
	CChartLine x_Axis;       // x��
	CChartLine y_Axis;       // y��
	CChartLine h_Grid;       // �����
	CChartLine v_Grid;       // ������
	CChartLine x_Degree;     // x���Ͽ̶�
	CChartLine y_Degree;     // y���Ͽ̶�
	CChartLine uptLine;      // �û�ָ�����ʮ��
	
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
