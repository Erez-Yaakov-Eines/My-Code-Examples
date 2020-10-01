#include "Queue.h"
#include <exception>

int Queue::serial;

bool Queue::isFull() 
{
	if (m_size == m_maxSize) return true;
	return false;
}

bool Queue::isEmpty() 
{
	if (m_size == 0) return true;
	return false;
}

int Queue::getSize() 
{
	return m_size;
}

int Queue::getMaxSize() 
{
	return m_maxSize;
}

int Queue::getServiceTime() 
{
	return m_service_time;
}

int Queue::getSerial() 
{
	return m_serialNum;
}

int Queue::getCurrentSerial()
{
	return serial;
}

int Queue::peek()
{
	return m_Q[m_head];
}

void Queue::push(int elem)
{

	m_Q[m_tail] = elem;
	m_tail = (m_tail + 1) % m_maxSize;
	++m_size;
}

int Queue::pop()
{
	int temp = m_Q[m_head];
	m_head = (m_head + 1) % m_maxSize;
	return temp;
}

Queue::Queue(int size) : m_maxSize(size), m_Q(NULL), m_size(0), m_head(0), m_tail(0)
{
	m_Q = new int[size];
	m_serialNum = ++serial;
	m_service_time = rand() % (101) + 50;
}

Queue::Queue(const Queue & other) 
{
	m_maxSize = other.m_maxSize;
	m_head = other.m_head;
	m_tail = other.m_tail;
	m_size = other.m_size;
	m_Q = NULL;
	m_service_time = other.m_service_time;
	m_Q = new int[m_maxSize];
	m_serialNum = serial;
}


Queue::~Queue()
{
	serial = serial - 1;
	delete[] m_Q;
}

const Queue & Queue::operator=(const Queue & other)
{
	m_head = other.m_head;
	m_tail = other.m_tail;
	m_size = other.m_size;
	int temp_head = m_head, size = m_size;
	while (size > 0) {
		m_Q[temp_head] = other.m_Q[temp_head];
		temp_head = (temp_head + 1) % m_maxSize;
		--size;
	}

	return (*this);
}

Queue::operator int() const
{
	return m_size * m_service_time;
}

ostream & operator<<(ostream & os, const Queue & Q)
{
	os << "printing Queue " << Q.m_serialNum << "." << endl;
	os << "size: " << Q.m_size << endl;
	os << "waiting time: " << Q.m_size*Q.m_service_time << endl;
	return os;
}
