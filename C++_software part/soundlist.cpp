#include "StdAfx.h"
#include "soundlist.h"
#include "global.h"

extern "C"
{
	#include "va_g729a.h"//G.729编解码
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

	::WaitForSingleObject(mutex_,INFINITE);//信号量机制


	//将原始数据转为G729编码后的数据
//	char rawData[DATA_SIZE] = {0};
//	memcpy(rawData,_data,DATA_SIZE);
//	char newdata[DATA_DECODE_SIZE] = {0};
	char *newdata = new char[DATA_DECODE_SIZE];//value=10
	memset(newdata,0x00,DATA_DECODE_SIZE);
	try
	{
		
		va_g729a_encoder((short *)_data, (unsigned char*)newdata);//他为什么要encoder呢G.729将10个16bit数据压缩成1个10bit数据
		
	}
	catch(...)
	{
		TRACE("encoder exception ....\n");
	}
	
	
	bufferSound_.push_back(newdata);//将解码后的数据存入到buffersound专用缓冲器的最后

	TRACE("putData in = %d\n",++m_in);//在控制台打印显示第几个采样数值。
	
	::ReleaseMutex(mutex_);
}


char* CSoundList::GetData()
{
	::WaitForSingleObject(mutex_,INFINITE);

	char *RawData = NULL;

	if(!bufferSound_.empty())
	{
		char *pTmp = NULL;
		pTmp = bufferSound_.front();//取出堆栈值
		
		bufferSound_.pop_front();	//修改堆栈指针

		//将取得的数据还原,解码
		RawData = new char[DATA_SIZE]; //解码后的原始数据
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