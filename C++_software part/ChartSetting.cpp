// ChartSetting.cpp : implementation file
//

#include "stdafx.h"
#include "testchartdll.h"
#include "ChartSetting.h"
#include "gbl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartSetting dialog


CChartSetting::CChartSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CChartSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChartSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChartSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartSetting, CDialog)
	//{{AFX_MSG_MAP(CChartSetting)
	ON_BN_CLICKED(IDOK, OnApplySetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartSetting message handlers

void CChartSetting::OnApplySetting() 
{
	// TODO: Add your control notification handler code here
	CString strText;
	GetDlgItemText(IDC_EDIT_RATE, strText);//可提取编辑框中文字
	float mpp;
	mpp = _ttoi((LPCTSTR)strText);//可转化编辑框中数据
	//mpp/=80;
	g_uUpdateRateTime =	(UINT)mSTEPS/mpp;
	EndDialog(NULL);
}
