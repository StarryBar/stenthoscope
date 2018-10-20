#ifndef SOUND_LIST_H
#define SOUND_LIST_H

#include <list>
#include <windows.h>

using namespace std;


class CSoundList
{
public:
	CSoundList();
	~CSoundList();

	void	PutData(const char *_data, const int _len);
	char*	GetData();
	bool	IsEmpty();

private:

	typedef list<char*> SOUNDBUFFER;//����һ��list������ʵ��*SOUNDBUFFER

	SOUNDBUFFER bufferSound_;//����������ݵĻ���
	HANDLE		mutex_; //

	int			m_in;
	int			m_out;

};


#endif;