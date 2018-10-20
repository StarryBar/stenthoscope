// testChartDllDlg.h : header file
//

#if !defined(AFX_TESTCHARTDLLDLG_H__85D0EB19_6CF5_4F41_BA9E_EECFA231B703__INCLUDED_)
#define AFX_TESTCHARTDLLDLG_H__85D0EB19_6CF5_4F41_BA9E_EECFA231B703__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "scrollchart.h"
#include "ChartWnd.h"
#include "ChartRx.h"
#include "StdAfx.h"

#include "soundlist.h"
#include "audiorecord.h"
#include "audioplay.h"
#include "ChartSerial.h"

/////////////////////////////////////////////////////////////////////////////
// CTestChartDllDlg dialog

class CTestChartDllDlg : public CDialog
{
// Construction
public:
	//父函数调用子函数特例
public:
	int* pData;
	char* pBuf;
	double* Process_S;
	double* U_left;
	double* V_right;
	double* vector_tpl;

	void Wave();
	CChartWnd mChart;
	CTestChartDllDlg(CWnd* pParent = NULL);	// standard constructor
	CWnd* lifanwang;
	CChartRx* mygod;
	
// Dialog Data
	//{{AFX_DATA(CTestChartDllDlg)
	enum { IDD = IDD_TESTCHARTDLL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestChartDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	private:
			CScrollChart* m_pCtrlScrollChart;
// Implementation
protected:
	HICON m_hIcon;
	CAudioPlay		*audioPlay_;
	CAudioRecord	*audioRecord_;
	CSoundList		*soundBuffer_;
	// Generated message map functions
	//{{AFX_MSG(CTestChartDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonSave();
	afx_msg void OnBUTTONSaveEnd();
	afx_msg void OnBUTTONTmpStop();
	afx_msg void OnBUTTONResume();
	afx_msg void OnBUTTONClear();
	afx_msg void OnButtonSchDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCHARTDLLDLG_H__85D0EB19_6CF5_4F41_BA9E_EECFA231B703__INCLUDED_)
