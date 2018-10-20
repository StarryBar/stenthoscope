// ChartLine.cpp: implementation of the CChartLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartLine.h"
#include "ChartRx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartLine::CChartLine()
{
	this->crLine = RGB(0, 80, 80);
	this->Width = 1;
	this->penType = PS_SOLID;   // PS_SOLID	, PS_DOT
	pen.CreatePen(penType, Width, crLine);
}

CChartLine::~CChartLine()
{
	pen.DeleteObject();
}

void CChartLine::SetStyle(int width, int type, COLORREF cr)
{
	if((crLine != cr) || (Width != width) || (penType != type))
	{
		this->crLine = cr;
		this->Width = width;
		this->penType = type;
		pen.DeleteObject();
		pen.CreatePen(penType, Width, crLine);
	}
}

/************ 文字块类 *******************************************/

CChartLabel::CChartLabel()
{
	text = "label";
	crBack = RGB(255, 255, 255);
	crText = RGB(0, 0, 0);
	Point = 9;
	Vector = 0;

	Font.CreateFont(36,  // 大小
		0,
		0,
		0,
		FW_NORMAL,   // 
		0,  // 斜
		0,  // 下划
		0,  // 描边
		ANSI_CHARSET, // 字符集合
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"arial"); 
	bNull = FALSE;
	pChart = NULL;

}
CChartLabel::~CChartLabel()
{
	Font.DeleteObject();
}

BOOL CChartLabel::GetStringSize(HDC hdc, LPCTSTR lpString, int cbString, LPSIZE lpSize)
{
	if (bNull)
	{
		if (pChart == NULL)//有设置好chart属性么？
			return FALSE;
		SetFontSize(Point, pChart->GetDC());
	}
	HGDIOBJ pFont = ::SelectObject(hdc, Font.m_hObject);
	BOOL ret = ::GetTextExtentPoint32(hdc, lpString, cbString, lpSize);
	::SelectObject(hdc, pFont);
    return ret;
}

BOOL CChartLabel::GetStringSize(HDC hdc, LPSIZE lpSize)
{
	if (bNull)
	{
		if (pChart == NULL)
			return FALSE;
		SetFontSize(Point, pChart->GetDC());
	}
	BOOL ret = GetStringSize(hdc, text, text.GetLength(), lpSize);
    return ret;
}

void CChartLabel::SetFontSize(int pointSize, HDC hDC)
{
	Point = pointSize;
	int nHeight = -MulDiv(Point, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	Font.DeleteObject();
	Font.CreateFont(nHeight,  // 大小
		0,
		Vector,
		0,
		FW_NORMAL,   // 
		0,  // 斜
		0,  // 下划
		0,  // 描边
		ANSI_CHARSET, // 字符集合
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"arial"); 
	bNull = FALSE;
}

void CChartLabel::SetFontSize(int pointSize)
{
	Point = pointSize;
	bNull = TRUE;
}

void CChartLabel::SetFontStyle(HDC hDC, int size, BOOL isVertical)
{
	Vector = 0;
	if (isVertical)
		Vector = 900;
	SetFontSize(size, hDC);
}

void CChartLabel::SetFontStyle(int size, BOOL isVertical)
{
	Point = size;
	Vector = 0;
	if (isVertical)
		Vector = 900;
	bNull = TRUE;
}

void CChartLabel::SetText(CString str)
{
	this->text = str;
}

void CChartLabel::SetColor(COLORREF foreColor, COLORREF backColor)
{
	crText = foreColor;
	crBack = backColor;
}

void CChartLabel::ShowText(HDC hDC, CString str, UINT uFormat)
{
	if (bNull)
	{
		if (pChart == NULL)
			return;
		SetFontSize(Point, pChart->GetDC());
	}

//	COLORREF crBackOld = ::SetBkColor(hDC, crBack);
	int modeOld = SetBkMode(hDC, TRANSPARENT);
	COLORREF crTextOld = ::SetTextColor(hDC, crText);
	HGDIOBJ pFont = ::SelectObject(hDC, Font.m_hObject);

	::DrawText(hDC, str, str.GetLength(), &rt, uFormat);
	
	::SelectObject(hDC, pFont);
	::SetTextColor(hDC, crTextOld);
	SetBkMode(hDC, modeOld);
//	::SetBkColor(hDC, crBackOld);
}



