#pragma once
#include <iostream>
#include <ctime>
using namespace std;

class Queue
{
public:

	Queue(int size);
	Queue(const Queue& other);
	~Queue();

	bool		isFull() ;
	bool		isEmpty() ;
	int			getSize() ;
	int			getMaxSize() ;
	int			getServiceTime() ;
	int			getSerial() ;
	static int	getCurrentSerial();
	int			peek();
	void		push(int elem);
	int			pop();
	
	const Queue& operator=(const Queue& other);
	friend ostream& operator<<(ostream& os, const Queue& Q);
	operator int() const;

private:

	static int	serial;
	int			m_service_time;
	int			m_serialNum;
	int*		m_Q;
	int			m_head;
	int			m_tail;
	int			m_size;
	int			m_maxSize;

};