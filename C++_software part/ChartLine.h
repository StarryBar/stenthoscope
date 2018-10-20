// ChartLine.h: interface for the CChartLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTLINE_H__EBCB1DCA_B1DF_4FD4_9FB3_ECE3E3692BFF__INCLUDED_)
#define AFX_CHARTLINE_H__EBCB1DCA_B1DF_4FD4_9FB3_ECE3E3692BFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StdAfx.h"

class CChartRx;

class CChartLine  
{
public:
	void SetStyle(int width, int type, COLORREF cr);
	COLORREF crLine;
	int Width;
	int penType;
	CPen pen;

	CChartLine();
	virtual ~CChartLine();

};

//struct ChartLabelList
//{
//	CChartLabel * pLabel;
//	struct ChartLabelList *next;
//};

// 用来显示文字块
class CChartLabel
{
public:
	void SetFontStyle(int size, BOOL isVertical);
	void SetFontSize(int pointSize);
	void SetFontStyle(HDC hDC, int size, BOOL isVertical);
	void ShowText(HDC hDC, CString str, UINT uFormat);
	void SetColor(COLORREF foreColor, COLORREF backColor=RGB(255, 255, 255));
	void SetText(CString str);
	void SetFontSize(int pointSize, HDC hDC);
	BOOL GetStringSize(HDC  hdc, LPCTSTR  lpString, int  cbString,LPSIZE  lpSize );
	BOOL GetStringSize(HDC  hdc, LPSIZE  lpSize);
	CString text;
	COLORREF crBack;
	COLORREF crText;
	CRect rt;
	CFont Font;           // 字体 
	BOOL bNull;           // 是否是空字体，需要创建
	int Point;            // 字体大小，9pt
	int Vector;           // 角度

	CChartRx * pChart;
	CChartLabel();
	virtual ~CChartLabel();
};

#endif // !defined(AFX_CHARTLINE_H__EBCB1DCA_B1DF_4FD4_9FB3_ECE3E3692BFF__INCLUDED_)
