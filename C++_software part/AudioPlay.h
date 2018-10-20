// AudioPlay.h: interface for the CAudioPlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOPLAY_H__98443300_C41C_4057_A7B8_FCC2D58C15DB__INCLUDED_)
#define AFX_AUDIOPLAY_H__98443300_C41C_4057_A7B8_FCC2D58C15DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "global.h"
#include <Mmsystem.h>
//#pragma comment (lib, "winmm.lib")
class CSoundList;



class CAudioPlay  
{
public:
	CAudioPlay(CSoundList *_soundList);
	virtual ~CAudioPlay();
	
	//初始化结构体WAVEFORMATEX的参数
	void InitWaveFormat();
	//开始放音
	BOOL StartPlay();
	//结束放音
	BOOL StopPlay();

	void ProcessPlay();

	void play();

	bool IsOpen() const;

protected:
	
	//初始化放音时候音频头结构体
	void initWaveOutHdr(WAVEHDR *_waveHdr);
	WAVEFORMATEX waveFormat_;
	char	dataRecord_[DATA_SIZE];
	
	HWAVEOUT waveOut_;
	WAVEHDR waveHdrOut_;
	HANDLE m_hOutThread;
	DWORD m_idOutTread;
	
	CSoundList *soundList_;

	bool PlayProcess(char *_data, const unsigned int _size);

private:

	//处理放音消息的线程函数
	static DWORD WINAPI WaveOutThreadProc(LPVOID lpParameter);
	
	//放音线程函数
	static DWORD WINAPI PlayThread(LPVOID param);
	
	bool devOpen_;
};

#endif // !defined(AFX_AUDIOPLAY_H__98443300_C41C_4057_A7B8_FCC2D58C15DB__INCLUDED_)
