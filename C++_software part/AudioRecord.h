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

	//��ʼ���ṹ��WAVEFORMATEX�Ĳ���
	void InitWaveFormat();
	//��ʼ¼��
	BOOL StartRecord();
	//����¼��
	BOOL StopRecord();

	//�̴߳�����
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
	

	//��ʼ��¼��ʱ����Ƶͷ�ṹ��
	void initWaveInHdr(WAVEHDR* _waveHdr);
	//����������
	BOOL RecordProcess();

	bool PerPareBuffer();
	bool FreeBuffer();

private:
	//¼�����̺߳���
	static DWORD WINAPI WaveInThreadProc(LPVOID lpParameter);

};

#endif // !defined(AFX_AUDIORECORD_H__59D76219_FB89_46B0_914E_47D1AB08F057__INCLUDED_)
