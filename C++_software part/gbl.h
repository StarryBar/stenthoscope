#ifndef GBL_H
#define GBL_H

#include "ChartRx.h"
#include "ScrollChart.h"
#include "ChartWnd.h"

CChartWnd extern *ALL_mChart;
CChartRx extern *omg;
CScrollChart extern *jisuji1;
BOOL extern TimerOnTag;
UINT extern g_uUpdateRateTime;
short extern *TmpBuffer;
UINT extern TmpCount;
UINT extern high_freq;
UINT extern low_freq;

#define mSTEPS 160
#define mMAXSIZE 22000
#define BUFFER_SIZE 16000

#endif