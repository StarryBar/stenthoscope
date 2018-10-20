// AudioPlay.cpp: implementation of the CAudioPlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "gbl.h"//这部分include不能放在stdafx前面

#include "AudioPlay.h"
#include "soundlist.h"


extern "C"
{
	#include "va_g729a.h"
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAudioPlay::CAudioPlay(CSoundList *pQueue)
{
	soundList_ = pQueue;

	m_hOutThread=NULL;

	devOpen_ = false;

}

CAudioPlay::~CAudioPlay()
{
	if(m_hOutThread!=NULL)
	{
		TerminateThread(m_hOutThread,0);
	}
	
}

bool CAudioPlay::IsOpen() const
{
	return devOpen_;
}

//初始化结构体WAVEFORMATEX的参数
void CAudioPlay::InitWaveFormat()
{
	waveFormat_.wFormatTag = WAVE_FORMAT_PCM;  //编码格式，包括WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM等 
	waveFormat_.nChannels = 1;//声道数，单声道为1，双声道为2
	waveFormat_.nSamplesPerSec = 8000;  //采样频率
	waveFormat_.wBitsPerSample = BITS_PER_SAMPLE; //WAVE文件的采样大小 
	waveFormat_.nBlockAlign = waveFormat_.nChannels*waveFormat_.wBitsPerSample/8; //块对齐 
	waveFormat_.nAvgBytesPerSec = waveFormat_.nSamplesPerSec*waveFormat_.nBlockAlign;//每秒的数据量 
	waveFormat_.cbSize = 0;
}

//初始化放音时候音频头结构体
void CAudioPlay::initWaveOutHdr(WAVEHDR *_waveHdr)
{
	if(NULL == _waveHdr)
		return;

	//从声音缓存队列中获取声音数据
	char *pData = soundList_->GetData();
	
	if(NULL != pData)
	{
		_waveHdr->lpData = pData;
		_waveHdr->dwBufferLength = DATA_SIZE;
		_waveHdr->dwFlags = 0;
		_waveHdr->dwLoops = 0;
	}
	
}


//开始放音
BOOL CAudioPlay::StartPlay()
{
	if(m_hOutThread==NULL)//保证不重复创建线程
	{
		m_hOutThread=CreateThread(NULL,0,WaveOutThreadProc,this,0,&m_idOutTread);//调用API创建waveout进程，用来接受缓冲区块写入声卡完毕消息
	}
	
	
	CreateThread(NULL,0,PlayThread,this,0,NULL);//调用API创建playthread线程，用来将缓冲区数据写入声卡

	InitWaveFormat();

	MMRESULT mmRet;

	mmRet = waveOutOpen (0,WAVE_MAPPER,&waveFormat_,0,0,WAVE_FORMAT_QUERY);//咨询有没有空闲的声卡
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CWaveOut::OpenDev: waveOutOpen error.\n");
		return FALSE;
	}


	mmRet = waveOutOpen(&waveOut_,WAVE_MAPPER,&waveFormat_,(unsigned long)m_idOutTread,0,CALLBACK_THREAD);

	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("waveOutOpen failed!!\n");
		return false;
	}

	TRACE("waveOutOpen ok!!!\n");
	return true;
}

//结束放音
BOOL CAudioPlay::StopPlay()
{
	MMRESULT mmRet;
	mmRet=waveOutReset(waveOut_);
	if(mmRet!=MMSYSERR_NOERROR )
	{
		TRACE("waveOutReset failed!\n");
		return false;
	}

	PostThreadMessage(m_idOutTread,WM_QUIT,0,0);
	Sleep(100);

	mmRet=waveOutClose(waveOut_);
	if(mmRet!=MMSYSERR_NOERROR )
	{
		TRACE("waveOutClose failed!\n");
		return false;
	}

	return true;
}


void CAudioPlay::play()
{

	va_g729a_init_decoder();

	while(1)
	{
		while(!IsOpen())
		{
			Sleep(10);
			continue;
		}

		
		//判断缓存队列是否为空
		if(!soundList_->IsEmpty())
		{
			//从声音缓存队列中获取声音数据
			char *pData = soundList_->GetData();//从和声卡关联的堆栈中读取最顶上的值（G.729编码）同时完成解码。这里得到的是10个16bit数据，设置一个全局变量存储
	//		for(int i=0; i<_size; i++)
	//			TmpBuffer[ TmpCount++ % BUFFER_SIZE ]=_data[i];


			if(NULL != pData)
			{
				short *token1=new short;
				for(int i=0; i<DATA_SIZE/2; i++)//拆分，ai转化，+-转化
				{
  					   *token1=*((short *)pData+i);
					TmpBuffer[TmpCount % (BUFFER_SIZE/2)]=*token1;//注意，这里还有一个无符号数和有符号数进制转化没有做。
					if (TmpBuffer[TmpCount % (BUFFER_SIZE/2)] > 32767) 
					{
						TmpBuffer[TmpCount % (BUFFER_SIZE/2)] -= 65536;
					}
					TmpCount++;
				}
				PlayProcess(pData,DATA_SIZE);//将160大小的缓冲池中的数据作为一个快放入声卡
				delete[] pData;
			}
		}
		else
		{
			Sleep(10);//等待其他线程处理：将数据从缓冲区送入声卡
		}

	}
}


//放音音过程
bool CAudioPlay::PlayProcess(char *_data, const unsigned int _size)
{
//



	char* p;
	WAVEHDR *pwh = (WAVEHDR*)malloc(sizeof(WAVEHDR));

	if(!pwh)
	{
		TRACE("CAudioPlay::PlayProcess: alloc WAVEHDR memoyr error.\n");
		return false;
	}
	
	p = new char[_size];

	if(!p)
	{
		TRACE("CAudioPlay::PlayProcess: alloc data memoyr error.\n");
		return false;
	}

	CopyMemory(p,_data,_size);
	ZeroMemory(pwh,sizeof(WAVEHDR));

	pwh->dwBufferLength = _size;
	pwh->lpData = p;//新建一个缓存区存放传入的数据

	MMRESULT mmRet;
	mmRet = waveOutPrepareHeader(waveOut_,pwh,sizeof(WAVEHDR));//缓存预申请声卡设备
  	if (mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CAudioPlay::PlayProcess: waveOutPrepareHeader error.\n");
		return false;
	}

	mmRet = waveOutWrite(waveOut_,pwh,sizeof(WAVEHDR));//缓存写入声卡设备
  	if (mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CAudioPlay::PlayProcess: waveOutWrite error.\n");
		return false;
	}

	return true;
}


void CAudioPlay::ProcessPlay()//创建一个线程来接受消息
{
	MSG msg;
	while(GetMessage(&msg,NULL,NULL,NULL))
	{
		switch(msg.message)
		{
		case MM_WOM_OPEN:
			{
				devOpen_ = true;
			}
			break;

		case MM_WOM_DONE://缓冲数据准备好了了就会发送DONE信号，在unprepare中进行声音的播放
			{
				//删除从队列中获取的字符串指针
				WAVEHDR* pwh=(WAVEHDR*)msg.lParam;
				
				waveOutUnprepareHeader((HWAVEOUT)msg.wParam,pwh,sizeof(WAVEHDR));//pwh块直接提交到声卡播放。				

				if(NULL != pwh)
				{
					delete[] pwh->lpData;//读取完就删除缓冲区中的数据，和pwh指针，但不删除缓冲区，
					free(pwh);
				}
			}
			break;

		case MM_WOM_CLOSE:
			{
				m_hOutThread = NULL;
			}
			break;

		default :
			break;
		}
	}
}


//放音的线程函数
DWORD WINAPI CAudioPlay::WaveOutThreadProc(LPVOID lpParameter)
{
	CAudioPlay *p=(CAudioPlay *)lpParameter;

	p->ProcessPlay();

	return 0;
}


DWORD WINAPI CAudioPlay::PlayThread(LPVOID param)
{
	CAudioPlay *p = (CAudioPlay*)param;

	p->play();


	return 0;
}