#pragma once
#include "Queue.h"

typedef enum algorithm{
	shor,lon,fas,ran
}algorithm;

class Simulate
{
public:

	Simulate(int interval, algorithm alg, const char* qType);
	~Simulate();
	int				getLeft();
	const char*		getQType();
	algorithm		getAlgorithm();
	int				getQnumber();
	int				getMaxInlineCustomers();
	int				getInterval();
	void			startSimulation(int timeCount);

private:


	Queue**			m_qArr;
	const int		m_interval;
	algorithm		m_algorithm;
	char*			m_qType;
	int				m_left;
	int				m_totalClient;
	int				m_maxTotalInlineCustomers;
	int				m_Qnumber;

	int		shortest(int first);
	int		longest(int first);
	int		fastest(int first);
	int		random();
	bool	addCustomer();
	int		findFirst();
	int		howManyQs();
	void	popAll(int t_i);
	Queue** buildQ2(int qLen, bool randomLen);
	void	buildQ1();
};

