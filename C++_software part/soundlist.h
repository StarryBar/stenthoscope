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

	typedef list<char*> SOUNDBUFFER;//创建一个list容器的实例*SOUNDBUFFER

	SOUNDBUFFER bufferSound_;//存放声音数据的缓存
	HANDLE		mutex_; //

	int			m_in;
	int			m_out;

};


#endif;