// AudioRecord.h: interface for the CAudioRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIORECORD_H__59D76219_FB89_46B0_914E_47D1AB08F057__INCLUDED_)
#define AFX_AUDIORECORD_H__59D76219_FB89_46B0_914E_47D1AB08F057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "global.h"
#include <Mmsystem.h>
//#pragma comment (lib, "winmm.lib")
class CSoundList;


class CAudioRecord 
{
public:

	CAudioRecord(CSoundList *_soundList);

	virtual ~CAudioRecord();

	//初始化结构体WAVEFORMATEX的参数
	void InitWaveFormat();
	//开始录音
	BOOL StartRecord();
	//结束录音
	BOOL StopRecord();

	//线程处理函数
	void processRecord();

protected:

	WAVEFORMATEX waveFormat_;

	HWAVEIN	waveIn_;
	char	dataRecord_[DATA_SIZE];
	bool	allocBuffer_;

	WAVEHDR *waveHdrIn_;

	HANDLE m_hInThread;
	DWORD m_idInThread;
	CSoundList *soundList_;
	

	//初始化录音时候音频头结构体
	void initWaveInHdr(WAVEHDR* _waveHdr);
	//放音音过程
	BOOL RecordProcess();

	bool PerPareBuffer();
	bool FreeBuffer();

private:
	//录音的线程函数
	static DWORD WINAPI WaveInThreadProc(LPVOID lpParameter);

};

#endif // !defined(AFX_AUDIORECORD_H__59D76219_FB89_46B0_914E_47D1AB08F057__INCLUDED_)
