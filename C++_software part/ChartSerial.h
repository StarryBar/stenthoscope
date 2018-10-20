// ChartSerial.h: interface for the CChartSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTSERIAL_H__516E7F28_DCEA_41CA_B3A5_DE1152F24872__INCLUDED_)
#define AFX_CHARTSERIAL_H__516E7F28_DCEA_41CA_B3A5_DE1152F24872__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxcoll.h >
typedef struct point_struct
{
	double x;
	double y;
	double data;
}POINTDATA, *LPPOINTDATA;
class CChartSerial  
{
public:
	void AddPointEx(double x, double y, double data);
	void SetSize(int max, int nGrow);
	void SetShow(BOOL bShow);
	BOOL bShow;
	void Clear();
	LPPOINTDATA GetPoint(int index);
	int GetPointCount();
	void AddPoint(double x, double y, double data);
	COLORREF crSerial;

	CChartSerial();
	virtual ~CChartSerial();

private:
	CPtrArray DataPairs;//返回空指针，同时加入getat函数
	int pointCount;
	int Upbound;
};

#endif // !defined(AFX_CHARTSERIAL_H__516E7F28_DCEA_41CA_B3A5_DE1152F24872__INCLUDED_)
