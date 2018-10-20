// AudioRecord.cpp: implementation of the CAudioRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "gbl.h"
#include "AudioRecord.h"
#include "soundlist.h"

extern "C"
{
	#include "va_g729a.h"
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAudioRecord::CAudioRecord(CSoundList *_soundList)
{
	m_hInThread		= NULL;
	soundList_		= _soundList;
	allocBuffer_	= false;
	waveHdrIn_		= NULL;
}

CAudioRecord::~CAudioRecord()
{
	if(m_hInThread!=NULL)
	{
		TerminateThread(m_hInThread,0);
	}
}

//初始化结构体WAVEFORMATEX的参数
void CAudioRecord::InitWaveFormat()
{
	waveFormat_.wFormatTag = WAVE_FORMAT_PCM;  //编码格式，包括WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM等 
	waveFormat_.nChannels = 1;//声道数，单声道为1，双声道为2
	waveFormat_.nSamplesPerSec = 8000;  //采样频率
	waveFormat_.wBitsPerSample = BITS_PER_SAMPLE; //WAVE文件的采样大小 
	waveFormat_.nBlockAlign = waveFormat_.nChannels*waveFormat_.wBitsPerSample/8; //块对齐 
	waveFormat_.nAvgBytesPerSec = waveFormat_.nSamplesPerSec*waveFormat_.nBlockAlign;//每秒的数据量 
	waveFormat_.cbSize = 0;
	
	TRACE("初始化完成\n");
}


//开始录音
BOOL CAudioRecord::StartRecord()
{
	if(m_hInThread==NULL)
	{
		m_hInThread = CreateThread(NULL,0,WaveInThreadProc,this,0,&m_idInThread);
	}

	InitWaveFormat();

	MMRESULT mmRet = MMSYSERR_NOERROR;

	mmRet = waveInOpen(0,WAVE_MAPPER,&waveFormat_,0,0,WAVE_FORMAT_QUERY);//确认是否有空闲声卡设备
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CWaveIn::OpenDev: waveInOpen error.\n");
		return FALSE;
	}

	mmRet = waveInOpen(&waveIn_,WAVE_MAPPER,&waveFormat_,(unsigned long)m_idInThread,0,CALLBACK_THREAD);//什么设备，哪个线程m_id回调句柄
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("waveInOpen failed!!");
		return FALSE;
	}

	return true;
}

//结束录音
BOOL CAudioRecord::StopRecord()
{

	MMRESULT mmRet;
	mmRet = waveInReset(waveIn_);
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("waveInReset failed!!\n");
		return false;
	}

	FreeBuffer();

	PostThreadMessage(m_idInThread,WM_QUIT,0,0);
	Sleep(100);
	
	mmRet = waveInClose(waveIn_);
	if(mmRet != MMSYSERR_NOERROR)
	{	
		TRACE("waveInClose failed!!\n");
		return false;
	}
	else
	{
		TRACE("waveInClose success!!\n");
	}		

	return true;
}


bool CAudioRecord::PerPareBuffer()
{
	if (allocBuffer_)
	{
		TRACE("CAudioRecord::PerPareBuffer: Buffer has been alloc.\n");
		return false;
	}

	MMRESULT mmRet;
	mmRet = waveInReset(waveIn_);//1.重置声卡

	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CAudioRecord::PerPareBuffer: waveInReset error.\n");
		return false;
	}

	unsigned int i;

	waveHdrIn_ = new WAVEHDR[NUM_BUF];//2.分配缓存数目
		
	for(i=0;i<NUM_BUF;i++)//创建10个缓冲区，并定义设备缓冲写入方向
	{
		ZeroMemory(&waveHdrIn_[i],sizeof(WAVEHDR));//分配10个缓存，每个缓存类型都是WAVEHDR，每个缓存初始化参数都是0，再在其中lpara参数增加引用地址。
		waveHdrIn_[i].lpData = new char[DATA_SIZE];//设置缓存大小
		waveHdrIn_[i].dwBufferLength = DATA_SIZE;

		mmRet = waveInPrepareHeader(waveIn_,&waveHdrIn_[i],sizeof(WAVEHDR));//3.声卡设备开启wavehdrin_缓存

		if(mmRet != MMSYSERR_NOERROR)
		{
			TRACE("CAudioRecord::PerPareBuffer: waveInPrepareHeader error.\n");
			return false;
		}
		
		mmRet = waveInAddBuffer(waveIn_,&waveHdrIn_[i],sizeof(WAVEHDR));//3.声卡设备将录音写入wavehdrin_缓存

		if(mmRet != MMSYSERR_NOERROR)
		{
			TRACE("CAudioRecord::PerPareBuffer: waveInAddBuffer error.\n");
			return false;
		}
	}

	allocBuffer_ = true;

	return true;
}

bool CAudioRecord::FreeBuffer()
{
	if (!allocBuffer_)
	{
		TRACE("CAudioRecord::FreeBuffer: Buffer hasn't been alloc.\n");
		return false;
	}
	
	unsigned int i;
	if(NULL == waveHdrIn_)
	{
		TRACE("CAudioRecord::FreeBuffer: m_pHdr is NULL.\n");
		return false;
	}

	MMRESULT mmRet;
	for(i=0;i<NUM_BUF;i++)
	{
		mmRet = waveInUnprepareHeader(waveIn_,&waveHdrIn_[i],sizeof(WAVEHDR));

		if(mmRet != MMSYSERR_NOERROR)
		{
			Sleep(100);
			TRACE("CAudioRecord::FreeBuffer: waveInUnprepareHeader error.\n");
			continue;
		}

		TRACE("CAudioRecord::FreeBuffer: %d.\n",i);
		
		if(waveHdrIn_[i].lpData)
			delete []waveHdrIn_[i].lpData;
	
	}

	delete []waveHdrIn_;
	waveHdrIn_ = NULL;

	allocBuffer_ = false;

	return true;
}



//初始化录音时候音频头结构体
void CAudioRecord::initWaveInHdr(WAVEHDR* _waveHdr)
{	
	if(NULL == _waveHdr)
		return;
	
	_waveHdr->lpData = new char[DATA_SIZE];
	_waveHdr->dwBufferLength = DATA_SIZE;
	_waveHdr->dwFlags = 0;
	_waveHdr->dwLoops = 0;
}

//录音过程
/*
BOOL CAudioRecord::RecordProcess()
{
//	initWaveInHdr(&m_waveHdrIn);
				
	MMRESULT mmRet;
	mmRet = waveInPrepareHeader(waveIn_,&m_waveHdrIn,sizeof(WAVEHDR));
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("waveInPrepareHeader failed!!\n");
		return FALSE;
	}
	
	mmRet = waveInAddBuffer(waveIn_,&m_waveHdrIn,sizeof(WAVEHDR));
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("waveInAddBuffer failed!!\n");
		return FALSE;
	}

	return TRUE;
}
*/

//录音的线程函数
DWORD WINAPI CAudioRecord::WaveInThreadProc(LPVOID lpParameter)
{
	CAudioRecord *p=(CAudioRecord *)lpParameter;

	p->processRecord();

	return 0;
}


void CAudioRecord::processRecord()//建立线程一直等待消息
{
	va_g729a_init_encoder();

	MSG msg;
	while(GetMessage(&msg,NULL,NULL,NULL))
	{
		switch(msg.message)
		{
		case MM_WIM_OPEN:
			{				
				Sleep(100);
				
				MMRESULT mmRet;
				
				PerPareBuffer();//定义10个buffer，并分配给特定音频设备，便于之后msg.lParam读取值

				mmRet  = waveInStart(waveIn_);//使用wavein设备启动录音
				
				if(mmRet != MMSYSERR_NOERROR)
				{
					TRACE("waveInStart failed!!\n");
	
					m_hInThread=NULL;	

					return ;
				}
			}
			break;

		case MM_WIM_DATA://若缓冲区M存满，发送MM_WIM_DATA消息，M作为lParam传入消息进行处理
			{	

				WAVEHDR* pWH=(WAVEHDR*)msg.lParam;//根据进来的声音地址分配缓存地址，lparam确定的是声卡输入的缓存地址
				
				waveInUnprepareHeader(waveIn_,pWH,sizeof(WAVEHDR));//切断声卡设备和某个160缓存的联系，提交封装

				if(pWH->dwBytesRecorded!=DATA_SIZE)
				{
					TRACE("dwBytesRecorded = %d",pWH->dwBytesRecorded);
				}
			
				//把数据拷贝到声音缓存中
				soundList_->PutData(pWH->lpData,pWH->dwBytesRecorded);//将160封装压入栈底
				

				waveInPrepareHeader((HWAVEIN)msg.wParam,pWH,sizeof(WAVEHDR));//声卡缓冲区从pWH开始
				waveInAddBuffer((HWAVEIN)msg.wParam,pWH,sizeof(WAVEHDR));//wparam确定的是声卡的输入设备
				
			}
			break;
			
		case MM_WIM_CLOSE:
			{
				m_hInThread=NULL;
				return ;
			}
			break;
			
		case WM_QUIT:
			{
				m_hInThread=NULL;
				return;
			}
			break;
		}
	}
	
}