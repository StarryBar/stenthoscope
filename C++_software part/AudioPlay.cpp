// AudioPlay.cpp: implementation of the CAudioPlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "gbl.h"//�ⲿ��include���ܷ���stdafxǰ��

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

//��ʼ���ṹ��WAVEFORMATEX�Ĳ���
void CAudioPlay::InitWaveFormat()
{
	waveFormat_.wFormatTag = WAVE_FORMAT_PCM;  //�����ʽ������WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM�� 
	waveFormat_.nChannels = 1;//��������������Ϊ1��˫����Ϊ2
	waveFormat_.nSamplesPerSec = 8000;  //����Ƶ��
	waveFormat_.wBitsPerSample = BITS_PER_SAMPLE; //WAVE�ļ��Ĳ�����С 
	waveFormat_.nBlockAlign = waveFormat_.nChannels*waveFormat_.wBitsPerSample/8; //����� 
	waveFormat_.nAvgBytesPerSec = waveFormat_.nSamplesPerSec*waveFormat_.nBlockAlign;//ÿ��������� 
	waveFormat_.cbSize = 0;
}

//��ʼ������ʱ����Ƶͷ�ṹ��
void CAudioPlay::initWaveOutHdr(WAVEHDR *_waveHdr)
{
	if(NULL == _waveHdr)
		return;

	//��������������л�ȡ��������
	char *pData = soundList_->GetData();
	
	if(NULL != pData)
	{
		_waveHdr->lpData = pData;
		_waveHdr->dwBufferLength = DATA_SIZE;
		_waveHdr->dwFlags = 0;
		_waveHdr->dwLoops = 0;
	}
	
}


//��ʼ����
BOOL CAudioPlay::StartPlay()
{
	if(m_hOutThread==NULL)//��֤���ظ������߳�
	{
		m_hOutThread=CreateThread(NULL,0,WaveOutThreadProc,this,0,&m_idOutTread);//����API����waveout���̣��������ܻ�������д�����������Ϣ
	}
	
	
	CreateThread(NULL,0,PlayThread,this,0,NULL);//����API����playthread�̣߳�����������������д������

	InitWaveFormat();

	MMRESULT mmRet;

	mmRet = waveOutOpen (0,WAVE_MAPPER,&waveFormat_,0,0,WAVE_FORMAT_QUERY);//��ѯ��û�п��е�����
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

//��������
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

		
		//�жϻ�������Ƿ�Ϊ��
		if(!soundList_->IsEmpty())
		{
			//��������������л�ȡ��������
			char *pData = soundList_->GetData();//�Ӻ����������Ķ�ջ�ж�ȡ��ϵ�ֵ��G.729���룩ͬʱ��ɽ��롣����õ�����10��16bit���ݣ�����һ��ȫ�ֱ����洢
	//		for(int i=0; i<_size; i++)
	//			TmpBuffer[ TmpCount++ % BUFFER_SIZE ]=_data[i];


			if(NULL != pData)
			{
				short *token1=new short;
				for(int i=0; i<DATA_SIZE/2; i++)//��֣�aiת����+-ת��
				{
  					   *token1=*((short *)pData+i);
					TmpBuffer[TmpCount % (BUFFER_SIZE/2)]=*token1;//ע�⣬���ﻹ��һ���޷��������з���������ת��û������
					if (TmpBuffer[TmpCount % (BUFFER_SIZE/2)] > 32767) 
					{
						TmpBuffer[TmpCount % (BUFFER_SIZE/2)] -= 65536;
					}
					TmpCount++;
				}
				PlayProcess(pData,DATA_SIZE);//��160��С�Ļ�����е�������Ϊһ�����������
				delete[] pData;
			}
		}
		else
		{
			Sleep(10);//�ȴ������̴߳��������ݴӻ�������������
		}

	}
}


//����������
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
	pwh->lpData = p;//�½�һ����������Ŵ��������

	MMRESULT mmRet;
	mmRet = waveOutPrepareHeader(waveOut_,pwh,sizeof(WAVEHDR));//����Ԥ���������豸
  	if (mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CAudioPlay::PlayProcess: waveOutPrepareHeader error.\n");
		return false;
	}

	mmRet = waveOutWrite(waveOut_,pwh,sizeof(WAVEHDR));//����д�������豸
  	if (mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CAudioPlay::PlayProcess: waveOutWrite error.\n");
		return false;
	}

	return true;
}


void CAudioPlay::ProcessPlay()//����һ���߳���������Ϣ
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

		case MM_WOM_DONE://��������׼�������˾ͻᷢ��DONE�źţ���unprepare�н��������Ĳ���
			{
				//ɾ���Ӷ����л�ȡ���ַ���ָ��
				WAVEHDR* pwh=(WAVEHDR*)msg.lParam;
				
				waveOutUnprepareHeader((HWAVEOUT)msg.wParam,pwh,sizeof(WAVEHDR));//pwh��ֱ���ύ���������š�				

				if(NULL != pwh)
				{
					delete[] pwh->lpData;//��ȡ���ɾ���������е����ݣ���pwhָ�룬����ɾ����������
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


//�������̺߳���
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