// testChartDllDlg.cpp : implementation file
//




#include "windows.h"
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")

#include"stdio.h"//
#include"WinDef.h"//
#include "stdafx.h"
#include "testChartDll.h"
#include "testChartDllDlg.h"
#include "ChartSetting.h"
#include "gbl.h"

#include "ChartRx.h"
#include <math.h>
#include "st.h"
#include "cmath"
#include "svd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestChartDllDlg dialog

CTestChartDllDlg::CTestChartDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestChartDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestChartDllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}
	
void CTestChartDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestChartDllDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestChartDllDlg, CDialog)
	//{{AFX_MSG_MAP(CTestChartDllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTONSAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTONSaveEnd, OnBUTTONSaveEnd)
	ON_BN_CLICKED(IDC_BUTTONTmpStop, OnBUTTONTmpStop)
	ON_BN_CLICKED(IDC_BUTTONResume, OnBUTTONResume)
	ON_BN_CLICKED(IDC_BUTTONClear, OnBUTTONClear)
	ON_BN_CLICKED(IDC_BUTTON9, OnButtonSchDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestChartDllDlg message handlers

BOOL CTestChartDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	soundBuffer_ = new CSoundList();
	audioPlay_ = new CAudioPlay(soundBuffer_);
	audioRecord_ = new CAudioRecord(soundBuffer_);
//	SetWindowPos(NULL,0,0,1024,600,SWP_NOMOVE);  
//	lifanwang=this;

	//
	mChart.Create("test chart", WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(220, 0, 813, 409), this, 0);
	srand((unsigned)time(NULL));
	//	mChart.SetXScale(0, 10);
//	mChart.SetTitle(0, "Version1.0 By LF WANG");
//	mChart.SetTitle(1, "x坐标");
//	mChart.SetTitle(2, "y坐标");
//	mChart.SetTickFormat("%.2f", "%.3f");
//	mChart.SetBackColor(RGB(0,0,0));
//	mChart.SetTitleColor(0, RGB(255, 0, 0));
//	mChart.SetTitleColor(1, RGB(0, 255, 255));
//	mChart.SetTitleColor(2, RGB(255, 0, 255));
//
//	mChart.SetLableColor(1, RGB(0, 0 ,0));
//	mChart.SetLableColor(2, RGB(0, 0 ,0));
//	mChart.SetLableColor(3, RGB(0, 0 ,0));
//	mChart.SetLableColor(4, RGB(0, 0 ,0));
// 		m_pCtrlScrollChart = (CScrollChart*)CScrollChart::CreateObject();//类似new
//	m_pCtrlScrollChart->SetWindowPos(0, 0);
//	m_pCtrlScrollChart->SetWindowSize(600, 200);
//	m_pCtrlScrollChart->SetCanvasWidth(10000);
//	m_pCtrlScrollChart->OnInitialUpdate();
//	m_pCtrlScrollChart->Create1(1000, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestChartDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestChartDllDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestChartDllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CTestChartDllDlg::OnButton1() 
{
//	CChartRx * p = (CChartRx *)mChart.mChart;
//	CString str =	p->AddFormat("%.f");
//	MessageBox(str);
//	p->SetNTickOneLabel(3, 1);
//	Wave();
CFile file("process.txt",CFile::modeRead);
DWORD dwFileLen;
dwFileLen=file.GetLength();
pBuf=new char[dwFileLen+1];
pBuf[dwFileLen]=0;
pData=new int[mMAXSIZE+1];
pData[mMAXSIZE]=0;

int j_tmp;
j_tmp=pow(2,ceil(log(mMAXSIZE)/log(2)));
int freq_size;
freq_size=high_freq-low_freq+1;
Process_S = (double *)malloc(sizeof(double)  * j_tmp*freq_size);
memset(Process_S,0,sizeof(double) * j_tmp* freq_size);
file.Read(pBuf,dwFileLen);
file.Close();

char seps   = 10;
char *token=NULL;
char temp[100]="";
int m=0;
   token = strtok( pBuf, &seps ); // C4996   去掉pBuf中的换行符并拆分长数据段CString。
   // Note: strtok is deprecated; consider using strtok_s instead
   while( token != NULL )
   {
      // While there are tokens in "string"
  	   pData[m]=atoi(token);
	   m++;
      // Get next token: 
      token = strtok( NULL, &seps ); // C4996
   }


   ////////////////////////////////////////////////////////
   //	st(mMAXSIZE, low_freq, high_freq, pData, Process_S);//s变换
   //////////////////////////////////////////////////////////////////////////动态分配左右奇异向量内存空间
/*	U_left = (double *)malloc(sizeof(double)  * freq_size * freq_size );
	memset(U_left,0,sizeof(double) * freq_size* freq_size);
	V_right = (double *)malloc(sizeof(double)  * j_tmp * j_tmp);
	memset(V_right,0,sizeof(double) * j_tmp * j_tmp);
	double * c_tmp =(double *)malloc(sizeof(double)  * j_tmp * freq_size);*/
	//////////////////////////////////////////////////////求解奇异值存放在Process_S矩阵中
/*	double eps=0.000001;
	dluav(Process_S,freq_size,j_tmp,U_left,V_right,eps,max(freq_size,j_tmp)+1);
	damul(U_left,Process_S,freq_size,freq_size,j_tmp,c_tmp);
	damul(c_tmp,V_right,freq_size,j_tmp,j_tmp,Process_S);
	free(U_left);free(V_right);free(c_tmp);*/
	/////////////////////////////////////////////////////
/*	vector_tpl=(double*)malloc(sizeof(double)*min(freq_size,j_tmp));
	for(int i=0;i<min(freq_size,j_tmp);i++)
	{
		vector_tpl[i] = Process_S[i*min(freq_size,j_tmp)+i];
	}*/
   /////////////////////////////////////////////////////////////
sprintf(temp,"String:   %d\n\n    mission finished", pData[0]); 
MessageBox(temp,"警告！！！",MB_OK);	

	mChart.SetSerialCount(2);
	mChart.ClearSerialPoints(0);
//动态刷新数据
	for (int i = 0; i < mMAXSIZE; i++)//2000
	{
		
		mChart.AddSerialPoint(0, i*0.125, pData[i], i);
		//CPoint point(i*0.125,pData[i]+200);//(i * 4, rand() % 400);//产生0~400之间一个随机数
		//m_pCtrlScrollChart->AddPoint(point);
	}
	mChart.SetChartType(1);
	*pBuf=NULL;

	jisuji1->SetUpdateRate(g_uUpdateRateTime);//开启定时器
}

void CTestChartDllDlg::OnButton2() 
{
	mChart.SetYScale(10000.111, 20000.111);
	mChart.Updata();
	
}

void CTestChartDllDlg::Wave()
{
	mChart.SetSerialCount(2);
	double sampleRate = 10;
	double secs = 10;
	int count = 1000;
	double t;
	double y;
	mChart.ClearSerialPoints(0);
	for (int i =0; i< count; i++)
	{
		t = (double)i / secs;
		y = 500*sin(2 * 3.14 *t);
		mChart.AddSerialPoint(0, t, y, i);
	}
	mChart.SetChartType(1);
}

void CTestChartDllDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
CChartSetting  Dlg;
Dlg.DoModal();
}

void CTestChartDllDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	// audioout.cpp : Defines the entry point for the console application.
//
LPSTR szFileName="37125.wav";//声音文件名

//LPSTR szFileName="d:\\37125.wav";//声音文件名
	sndPlaySound (szFileName,SND_ASYNC);
}

void CTestChartDllDlg::OnButtonRecord() 
{
	// TODO: Add your control notification handler code here
	memset(TmpBuffer,0,BUFFER_SIZE/2);
	audioRecord_->StartRecord();

}

void CTestChartDllDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here

	audioPlay_->StartPlay();
}

void CTestChartDllDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
omg->OnButtonSaveStart();

}

void CTestChartDllDlg::OnBUTTONSaveEnd() 
{
	// TODO: Add your control notification handler code here
omg->OnButtonSaveEnd();

}

void CTestChartDllDlg::OnBUTTONTmpStop() 
{
	// TODO: Add your control notification handler code here
	timeKillEvent(jisuji1->m_timerId);
}

void CTestChartDllDlg::OnBUTTONResume() 
{
	// TODO: Add your control notification handler code here
	jisuji1->SetUpdateRate(g_uUpdateRateTime);
}

void CTestChartDllDlg::OnBUTTONClear() 
{
	// TODO: Add your control notification handler code here
	CChartSerial *pserial =omg->GetSerial(0);//这里简化了，只取一个通道！！
	pserial->Clear();
	omg->m_nIndex=0;
	omg->DrawPlot();//?????什么时候才能刷新啊！

	CDialog::OnPaint();
	jisuji1->Invalidate();
	jisuji1->SetFocus();
}

void CTestChartDllDlg::OnButtonSchDisplay() 
{
	

	// TODO: Add your control notification handler code here //这些全部放到MMTimer中去，该按钮只负责开启定时器，在声卡采满一轮后进行数制转化存放10个声卡数据、立即将10个数据add serial点，第二轮继续将10个数据add serial点，第20轮时就会有200个serial点，第40轮就会有400个serial点。采满足够多个点（16000）后就可以开始MMTimer绘图了。（我现在主要是担心数据拆分、转化、存取操作会占用很大周期，导致声卡采样不连贯！！！！！→消息处理的时候，声卡消息还是会异步进行，不会互相影响。）绘图线程开启定时器消息处理时，会统一对200个serial点进行刷新绘图，下一个周期对400个serial点进行刷新绘图，（注意：addserial写的时候，不能同时对serial进行读取显示，需要在addserial和serial写之间加一个mutex；也许会影响声卡读取速度的连贯性。为了保证连贯性，可以使用两个缓冲池+mutex，轮流交替读写保证连贯。）
/*
pData=new int[BUFFER_SIZE];
char *token1=NULL;//1.如何拆分长char数据串，2.如何合并小数据串为token并转化为int正数 3.如何动态采集buffer_size——数据输入在消息处理函数里进行
for(int i=0; i<BUFFER_SIZE/10; i++)
	{
	for(int j=0; j<10; j++)
  	   token1[j]=TmpBuffer[i*10+j];
	pData[i]=atoi(token1);
    }

TmpBuffer   TmpCount   BUFFER_SIZE


	sprintf(temp,"String:   %d\n\n    mission finished", pData[0]);
  MessageBox(temp,"前方高能！！！",MB_OK);	
*/
		mChart.SetSerialCount(2);
		mChart.ClearSerialPoints(0);
	//动态刷新数据
	//	for (int i = 0; i < 22000; i++)//2000
	//	{
			
	//		mChart.AddSerialPoint(0, i*0.125, TmpBuffer[i], i);
			//CPoint point(i*0.125,pData[i]+200);//(i * 4, rand() % 400);//产生0~400之间一个随机数
			//m_pCtrlScrollChart->AddPoint(point);
	//	}
	//	mChart.SetChartType(1);


		jisuji1->SetUpdateRate(g_uUpdateRateTime);//开启定时器
}
