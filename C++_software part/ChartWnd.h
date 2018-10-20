// ChartWnd.h: interface for the CChartWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTWND_H__E01A4DF8_6289_4433_B082_DF57F79BD7F9__INCLUDED_)
#define AFX_CHARTWND_H__E01A4DF8_6289_4433_B082_DF57F79BD7F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScrollChart.h"
#include "ChartRx.h"


class CChartWnd  
{
public:
	void SetLabelFontSize(int index, int size);                              // 设置文本标签字体大小
	void SetTitleFontSize(int index, int size);                              // 设置标题字体大小
	void SetLableColor(int index, COLORREF cr);                              // 设置文本标签字体颜色
	void SetTitleColor(int index, COLORREF cr);                              // 设置标题字体颜色
	void SetForeColor(COLORREF cr);                                          // 前景颜色
	void SetBackColor(COLORREF cr);                                          // 背景颜色
	void Updata();

	void SaveBmp(LPCTSTR  Filepath, BOOL bWhileBack);                        // 保存背景，用白色背景
	void CanMagnify(BOOL bShow);                                             // 是否可以放大
	void ShowTip(BOOL bShow);                                                // 是否显示坐标
	void ClearXLabels();                                                     // 去初X标签，退出自定义模式
	void AddXLabel(double xVal);                                             // 添加一个X标签，自定义模式
	void ClearSerialPoints(int index);                                       // 清除线条所有数据点
	void AddSerialPointEx(int index, double x, double y, double value);      // 添加一个点
	void AddSerialPoint(int index, double x, double y, double value);        // 在某个范围内快速添加
	void SetSerialPoints(int index, int count);                              // 设置可以某个线条点的个数
	void SetSerialColor(int index, COLORREF cr);                             // 设置线条点的颜色
	COLORREF GetSerialColor(int index);                                      // 得到某个线的颜色
	void SetSerialCount(int count);                                          // 设置线条个数
	void SetTickFormat(CString x, CString y);                                // 刻度文本格式
	void SetXDivision(int major, int mini);                                  // 设置X轴刻度
	void SetYDivision(int major, int mini);                                  // 设置Y轴刻度
	void SetChartType(int type);                                             // 图的类型，线:1，直方:2
	void SetTitle(int index, LPCTSTR str);                                   // 标题
	void SetYScale(double min, double max);                                  // 设置Y轴范围
	void SetXScale(double min, double max);                                  // 设置X轴范围
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle,                             // 创建窗口
				const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	CChartWnd();
	virtual ~CChartWnd();
	
	CScrollChart * mChart;//3

};

#endif // !defined(AFX_CHARTWND_H__E01A4DF8_6289_4433_B082_DF57F79BD7F9__INCLUDED_)
