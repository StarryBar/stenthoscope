// ChartSerial.cpp: implementation of the CChartSerial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "RxChart.h"
#include "ChartSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartSerial::CChartSerial()//波形颜色
{
	bShow = TRUE;
	pointCount = 0;
	crSerial = RGB(255, 0, 0);
}

CChartSerial::~CChartSerial()
{
	Clear();
}

//DEL void CChartSerial::SetCount(int count)
//DEL {
//DEL 	this->DataPairs.SetSize(count, 50);
//DEL }

void CChartSerial::AddPoint(double x, double y, double data)
{
	LPPOINTDATA pPoint = new POINTDATA();
	if (pPoint == NULL)
	{
		AfxMessageBox("CChartRx: 建立数据点失败！");
		return;
	}
	pPoint->x = x;
	pPoint->y = y;
	pPoint->data = data;
	this->DataPairs.Add(pPoint);//增加一个member到datapairs最后
	this->pointCount ++;
}

int CChartSerial::GetPointCount()
{
	return this->pointCount;
}

LPPOINTDATA CChartSerial::GetPoint(int index)
{
	return (LPPOINTDATA)(this->DataPairs.GetAt(index));
}

void CChartSerial::Clear()//清除节点
{
	int i;
	LPPOINTDATA pPoint = NULL;
	for (i=0; i<pointCount; i++)
	{
	//	pPoint = NULL;
		pPoint = (LPPOINTDATA)(this->DataPairs.GetAt(i));
		delete pPoint;
	}
	this->DataPairs.RemoveAll();
	this->DataPairs.SetSize(0, 1024);
	this->pointCount = 0;
}

void CChartSerial::SetShow(BOOL bShow)
{
	this->bShow = bShow;
}

void CChartSerial::SetSize(int max, int nGrow)
{
	DataPairs.SetSize(max, nGrow);
	this->pointCount = 0;
	this->Upbound = max;
}
// 使用直接插入的方式设置数据，不重复分配内存
void CChartSerial::AddPointEx(double x, double y, double data)
{
	LPPOINTDATA pPoint = new POINTDATA();
	pPoint->x = x;
	pPoint->y = y;
	pPoint->data = data;
	this->DataPairs.SetAt(this->pointCount, pPoint);
	this->pointCount ++;
}
