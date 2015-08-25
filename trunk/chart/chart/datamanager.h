#ifndef __DATA_MANAGER_H__
#define	__DATA_MANAGER_H__


class CDataManager
{
public:
	CDataManager();
	virtual ~CDataManager();

	virtual BOOL AddData(int nStatis, time_t tFrom);
	virtual BOOL RemoveData(time_t tUntill);
	virtual BOOL PushData(int nStatis);
	virtual BOOL PopData();

	virtual BOOL SetInterval();
	virtual BOOL GetData();
	
private:

	std::vector<int> m_nStatisVector;
	time_t m_tStartTime;
};



#endif //__DATA_MANAGER_H__

