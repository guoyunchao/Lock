#pragma once
#include <Windows.h>
class CLock
{
	CRITICAL_SECTION _cs;

public:
	CLock (){::InitializeCriticalSection(&_cs);}
	~CLock(){::DeleteCriticalSection(&_cs);}

	void Lock(){
		::EnterCriticalSection(&_cs);
	}
	void Unlock(){
		::LeaveCriticalSection(&_cs);
	}
}; 
static char bufForPrint[50] = {0};
class CReadWriteLock
{
public:
	CReadWriteLock()
		: read_cnt(0)
	{
	}

	void lockForRead()
	{
		_read.Lock();
		if (++read_cnt == 1)
			_write.Lock();

		_read.Unlock();
	}

	void unlockRead()
	{
		_read.Lock();
		if (--read_cnt == 0)
			_write.Unlock();

		_read.Unlock();
	}

	void lockForWrite()
	{
		_write.Lock();
	}

	void unlockWrite()
	{
		_write.Unlock();
	}

private:
	CLock _read;
	CLock _write;
	int read_cnt; // 已加读锁个数
};

class CAutoLock
{
	CLock* m_pLock;

public:
	CAutoLock(CLock* pLock):m_pLock(pLock)
	{
		m_pLock->Lock();
	}

	~CAutoLock()
	{
		m_pLock->Unlock();
	}
};

class CAutoReadLock
{
	CReadWriteLock* m_pLock;

public:
	CAutoReadLock(CReadWriteLock* pLock):m_pLock(pLock)
	{
		m_pLock->lockForRead();
	}

	~CAutoReadLock()
	{
		m_pLock->unlockRead();
	}
};

class CAutoWriteLock
{
	CReadWriteLock* m_pLock;

public:
	CAutoWriteLock(CReadWriteLock* pLock):m_pLock(pLock)
	{
		m_pLock->lockForWrite();
	}

	~CAutoWriteLock()
	{
		m_pLock->unlockWrite();
	}
};

/////////////////////////////////////////////////////
/*
	CLock _lk;

	void fun()
	{
		//do somethint
		CAutoLock _auto(&_lk);
		//do somethint
	}

	CReadWriteLock _rwlk;
	void read()
	{
		CAutoReadLock _auto(&_rwlk);
	}

	void write()
	{
		CAutoWriteLock _auto(&_rwlk);
	}

*/