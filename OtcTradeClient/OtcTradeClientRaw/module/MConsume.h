#ifndef _MCONSUME_DEFINE_H
#define _MCONSUME_DEFINE_H

#include <functional>
using fnConsumeCB = std::function<int(const char*, int, long long, int, const char*)>;

class MConsumeImpl;
class MConsume
{
public:
	MConsume();
	~MConsume();

public:
	int				Init(const char* brokers, const char* group, fnConsumeCB pfnCB);
	void			Release();

	void			Subscribe(int topicCount, char* argv[]);

	void			PeriodCall();

protected:
	MConsumeImpl*	m_pImpl;
	fnConsumeCB		m_pfn;
};




#endif


















