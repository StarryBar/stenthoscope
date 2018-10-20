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
	void SetLabelFontSize(int index, int size);                              // �����ı���ǩ�����С
	void SetTitleFontSize(int index, int size);                              // ���ñ��������С
	void SetLableColor(int index, COLORREF cr);                              // �����ı���ǩ������ɫ
	void SetTitleColor(int index, COLORREF cr);                              // ���ñ���������ɫ
	void SetForeColor(COLORREF cr);                                          // ǰ����ɫ
	void SetBackColor(COLORREF cr);                                          // ������ɫ
	void Updata();

	void SaveBmp(LPCTSTR  Filepath, BOOL bWhileBack);                        // ���汳�����ð�ɫ����
	void CanMagnify(BOOL bShow);                                             // �Ƿ���ԷŴ�
	void ShowTip(BOOL bShow);                                                // �Ƿ���ʾ����
	void ClearXLabels();                                                     // ȥ��X��ǩ���˳��Զ���ģʽ
	void AddXLabel(double xVal);                                             // ���һ��X��ǩ���Զ���ģʽ
	void ClearSerialPoints(int index);                                       // ��������������ݵ�
	void AddSerialPointEx(int index, double x, double y, double value);      // ���һ����
	void AddSerialPoint(int index, double x, double y, double value);        // ��ĳ����Χ�ڿ������
	void SetSerialPoints(int index, int count);                              // ���ÿ���ĳ��������ĸ���
	void SetSerialColor(int index, COLORREF cr);                             // �������������ɫ
	COLORREF GetSerialColor(int index);                                      // �õ�ĳ���ߵ���ɫ
	void SetSerialCount(int count);                                          // ������������
	void SetTickFormat(CString x, CString y);                                // �̶��ı���ʽ
	void SetXDivision(int major, int mini);                                  // ����X��̶�
	void SetYDivision(int major, int mini);                                  // ����Y��̶�
	void SetChartType(int type);                                             // ͼ�����ͣ���:1��ֱ��:2
	void SetTitle(int index, LPCTSTR str);                                   // ����
	void SetYScale(double min, double max);                                  // ����Y�᷶Χ
	void SetXScale(double min, double max);                                  // ����X�᷶Χ
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle,                             // ��������
				const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	CChartWnd();
	virtual ~CChartWnd();
	
	CScrollChart * mChart;//3

};

#endif // !defined(AFX_CHARTWND_H__E01A4DF8_6289_4433_B082_DF57F79BD7F9__INCLUDED_)
