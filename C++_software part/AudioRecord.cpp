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

//��ʼ���ṹ��WAVEFORMATEX�Ĳ���
void CAudioRecord::InitWaveFormat()
{
	waveFormat_.wFormatTag = WAVE_FORMAT_PCM;  //�����ʽ������WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM�� 
	waveFormat_.nChannels = 1;//��������������Ϊ1��˫����Ϊ2
	waveFormat_.nSamplesPerSec = 8000;  //����Ƶ��
	waveFormat_.wBitsPerSample = BITS_PER_SAMPLE; //WAVE�ļ��Ĳ�����С 
	waveFormat_.nBlockAlign = waveFormat_.nChannels*waveFormat_.wBitsPerSample/8; //����� 
	waveFormat_.nAvgBytesPerSec = waveFormat_.nSamplesPerSec*waveFormat_.nBlockAlign;//ÿ��������� 
	waveFormat_.cbSize = 0;
	
	TRACE("��ʼ�����\n");
}


//��ʼ¼��
BOOL CAudioRecord::StartRecord()
{
	if(m_hInThread==NULL)
	{
		m_hInThread = CreateThread(NULL,0,WaveInThreadProc,this,0,&m_idInThread);
	}

	InitWaveFormat();

	MMRESULT mmRet = MMSYSERR_NOERROR;

	mmRet = waveInOpen(0,WAVE_MAPPER,&waveFormat_,0,0,WAVE_FORMAT_QUERY);//ȷ���Ƿ��п��������豸
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CWaveIn::OpenDev: waveInOpen error.\n");
		return FALSE;
	}

	mmRet = waveInOpen(&waveIn_,WAVE_MAPPER,&waveFormat_,(unsigned long)m_idInThread,0,CALLBACK_THREAD);//ʲô�豸���ĸ��߳�m_id�ص����
	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("waveInOpen failed!!");
		return FALSE;
	}

	return true;
}

//����¼��
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
	mmRet = waveInReset(waveIn_);//1.��������

	if(mmRet != MMSYSERR_NOERROR)
	{
		TRACE("CAudioRecord::PerPareBuffer: waveInReset error.\n");
		return false;
	}

	unsigned int i;

	waveHdrIn_ = new WAVEHDR[NUM_BUF];//2.���仺����Ŀ
		
	for(i=0;i<NUM_BUF;i++)//����10�����������������豸����д�뷽��
	{
		ZeroMemory(&waveHdrIn_[i],sizeof(WAVEHDR));//����10�����棬ÿ���������Ͷ���WAVEHDR��ÿ�������ʼ����������0����������lpara�����������õ�ַ��
		waveHdrIn_[i].lpData = new char[DATA_SIZE];//���û����С
		waveHdrIn_[i].dwBufferLength = DATA_SIZE;

		mmRet = waveInPrepareHeader(waveIn_,&waveHdrIn_[i],sizeof(WAVEHDR));//3.�����豸����wavehdrin_����

		if(mmRet != MMSYSERR_NOERROR)
		{
			TRACE("CAudioRecord::PerPareBuffer: waveInPrepareHeader error.\n");
			return false;
		}
		
		mmRet = waveInAddBuffer(waveIn_,&waveHdrIn_[i],sizeof(WAVEHDR));//3.�����豸��¼��д��wavehdrin_����

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



//��ʼ��¼��ʱ����Ƶͷ�ṹ��
void CAudioRecord::initWaveInHdr(WAVEHDR* _waveHdr)
{	
	if(NULL == _waveHdr)
		return;
	
	_waveHdr->lpData = new char[DATA_SIZE];
	_waveHdr->dwBufferLength = DATA_SIZE;
	_waveHdr->dwFlags = 0;
	_waveHdr->dwLoops = 0;
}

//¼������
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

//¼�����̺߳���
DWORD WINAPI CAudioRecord::WaveInThreadProc(LPVOID lpParameter)
{
	CAudioRecord *p=(CAudioRecord *)lpParameter;

	p->processRecord();

	return 0;
}


void CAudioRecord::processRecord()//�����߳�һֱ�ȴ���Ϣ
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
				
				PerPareBuffer();//����10��buffer����������ض���Ƶ�豸������֮��msg.lParam��ȡֵ

				mmRet  = waveInStart(waveIn_);//ʹ��wavein�豸����¼��
				
				if(mmRet != MMSYSERR_NOERROR)
				{
					TRACE("waveInStart failed!!\n");
	
					m_hInThread=NULL;	

					return ;
				}
			}
			break;

		case MM_WIM_DATA://��������M����������MM_WIM_DATA��Ϣ��M��ΪlParam������Ϣ���д���
			{	

				WAVEHDR* pWH=(WAVEHDR*)msg.lParam;//���ݽ�����������ַ���仺���ַ��lparamȷ��������������Ļ����ַ
				
				waveInUnprepareHeader(waveIn_,pWH,sizeof(WAVEHDR));//�ж������豸��ĳ��160�������ϵ���ύ��װ

				if(pWH->dwBytesRecorded!=DATA_SIZE)
				{
					TRACE("dwBytesRecorded = %d",pWH->dwBytesRecorded);
				}
			
				//�����ݿ���������������
				soundList_->PutData(pWH->lpData,pWH->dwBytesRecorded);//��160��װѹ��ջ��
				

				waveInPrepareHeader((HWAVEIN)msg.wParam,pWH,sizeof(WAVEHDR));//������������pWH��ʼ
				waveInAddBuffer((HWAVEIN)msg.wParam,pWH,sizeof(WAVEHDR));//wparamȷ�����������������豸
				
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