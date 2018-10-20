// testChartDll.h : main header file for the TESTCHARTDLL application
//

#if !defined(AFX_TESTCHARTDLL_H__28815B98_C288_472C_BA86_7EF78558653F__INCLUDED_)
#define AFX_TESTCHARTDLL_H__28815B98_C288_472C_BA86_7EF78558653F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestChartDllApp:
// See testChartDll.cpp for the implementation of this class
//

class CTestChartDllApp : public CWinApp
{
public:
	CTestChartDllApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestChartDllApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestChartDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCHARTDLL_H__28815B98_C288_472C_BA86_7EF78558653F__INCLUDED_)
