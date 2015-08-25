// singleton.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <pthread.h>


class CLock
{
public:
	CLock() { phtread_mutex_init(&m_tMutex);}
	~CLock() {pthread_mutex_destroy(&m_tMutex);}
	void lock() {pthread_mutex_lock(&m_tMutex);}
	void unlock() {pthread_mutex_unlock(&m_tMutex);}
private:
	pthread_mutext_t m_tMutex;
};

class CLockWrap
{
public:
	CLockWrap(CLock* pcLock):m_pcLock(pcLock) { m_pcLock->lock();}
	~CLockWrap() {m_pcLock->unlock();}
private:
	CLock* m_pcLock;
};

class singleton
{
public:
	singleton* Instance(int type = 0);
private:
	singleton() {printf("singleton()\n");}
	virtual ~singleton() {printf("~singleton()\n");}
public:
	void Test() = 0;
};


class sigderive : public singleton
{
public:
	sigderive* Instance();
private:
	sigderive() {printf("sigderive()\n");}
	virtual ~sigderive() {printf("~sigderive()\n");}
public:
	void Test() {printf("sigderive::Test()\n");}
private:
	friend class std::auto_ptr<sigderive>;
	static std::auto_ptr<sigderive> _instance;
	static CLock _lock;
};

std::auto_ptr<sigderive> sigderive::_instance;
CLock sigderive::_lock;

class sigderive2 : public singleton
{
public:
	sigderive2* Instance();
private:
	sigderive2() {printf("sigderive2()\n");}
	virtual ~sigderive2() {printf("~sigderive2()\n");}
public:
	void Test() {printf("sigderive2::Test()\n");};
private:
	friend class std::auto_ptr<sigderive2>;
	static std::auto_ptr<sigderive2> _instance;
	static CLock _lock;
};

std::auto_ptr<sigderive> sigderive2::_instance;
CLock sigderive2::_lock;


singleton* singleton::Instance(int type)
{
	if( type == 0 )
	{
		return sigderive::Instance();
	}
	else if( type == 1 )
	{
		return sigderive2::Instance();
	}
	else
	{
		return NULL;
	}
}

sigderive* sigderive::Instance()
{
	if(  0 == _instance.get() )
	{
		CLockWrap lockwrap(&_lock);
		if( 0 == _instance.get() )
		{
			_instance.reset(new sigderive);
		}
	}

	return _instance.get();
}


sigderive2* sigderive2::Instance()
{
	if(  0 == _instance.get() )
	{
		CLockWrap(&_lock);
		if( 0 == _instance.get() )
		{
			_instance.reset(new sigderive2);
		}
	}

	return _instance.get();
}


int _tmain(int argc, _TCHAR* argv[])
{
	singleton* p1 = singleton::Instance(0);
	p1->Test();

	singleton* p2 = singleton::Instance(1);
	p2->Test();


	singleton* p3 = singleton::Instance(0);
	p3->Test();


	singleton* p4 = singleton::Instance(1);
	p4->Test();

	return 0;
}

