
#include"stdafx.h"
#include "gbl.h"


CChartWnd *ALL_mChart= new CChartWnd();
CScrollChart *jisuji1;
CChartRx *omg=new CChartRx();//��ʱ�Ȳ���CScrollChart���Ϲ�ϵ
BOOL TimerOnTag=FALSE;
UINT g_uUpdateRateTime=50;
short *TmpBuffer= new short[BUFFER_SIZE/2];
UINT TmpCount=0;
UINT high_freq=300;
UINT low_freq=1;


