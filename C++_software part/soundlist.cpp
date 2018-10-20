#include "StdAfx.h"
#include "soundlist.h"
#include "global.h"

extern "C"
{
	#include "va_g729a.h"//G.729�����
}

CSoundList::CSoundList()
{
	mutex_ = CreateMutex(NULL,FALSE,NULL);
	m_in = m_out = 0;
}

CSoundList::~CSoundList()
{
	if(NULL != mutex_)
	{
		CloseHandle(mutex_);
	}
}

void CSoundList::PutData(const char *_data,const int _len)
{
	if(NULL == _data)
		return;

	::WaitForSingleObject(mutex_,INFINITE);//�ź�������


	//��ԭʼ����תΪG729����������
//	char rawData[DATA_SIZE] = {0};
//	memcpy(rawData,_data,DATA_SIZE);
//	char newdata[DATA_DECODE_SIZE] = {0};
	char *newdata = new char[DATA_DECODE_SIZE];//value=10
	memset(newdata,0x00,DATA_DECODE_SIZE);
	try
	{
		
		va_g729a_encoder((short *)_data, (unsigned char*)newdata);//��ΪʲôҪencoder��G.729��10��16bit����ѹ����1��10bit����
		
	}
	catch(...)
	{
		TRACE("encoder exception ....\n");
	}
	
	
	bufferSound_.push_back(newdata);//�����������ݴ��뵽buffersoundר�û����������

	TRACE("putData in = %d\n",++m_in);//�ڿ���̨��ӡ��ʾ�ڼ���������ֵ��
	
	::ReleaseMutex(mutex_);
}


char* CSoundList::GetData()
{
	::WaitForSingleObject(mutex_,INFINITE);

	char *RawData = NULL;

	if(!bufferSound_.empty())
	{
		char *pTmp = NULL;
		pTmp = bufferSound_.front();//ȡ����ջֵ
		
		bufferSound_.pop_front();	//�޸Ķ�ջָ��

		//��ȡ�õ����ݻ�ԭ,����
		RawData = new char[DATA_SIZE]; //������ԭʼ����
		memset(RawData,0xab,DATA_SIZE);
 		try
 		{
 			
 			va_g729a_decoder((unsigned char*)pTmp,(short *)RawData,0);
 			
 		}
 		catch(...)
 		{
 			TRACE("decoder exception!!!\n");
 		}

		delete[] pTmp;
		TRACE("GetData out = %d\n",++m_out);
	}

	::ReleaseMutex(mutex_);

	return RawData;
}


bool CSoundList::IsEmpty()
{
	::WaitForSingleObject(mutex_,INFINITE);

	bool ret = bufferSound_.empty();

	::ReleaseMutex(mutex_);

	return ret;
}