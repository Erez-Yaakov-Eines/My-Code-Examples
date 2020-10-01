#include "Simulate.h"
#include "Queue.h"
using namespace std;

int Simulate::getLeft() 
{
	return m_left;
}

const char * Simulate::getQType() 
{
	return m_qType;
}

algorithm Simulate::getAlgorithm() 
{
	return m_algorithm;
}

int Simulate::getQnumber() 
{
	return m_Qnumber;
}

int Simulate::getMaxInlineCustomers() 
{
	return m_maxTotalInlineCustomers;
}

int Simulate::getInterval() 
{
	return m_interval;
}

void Simulate::startSimulation(int timeCount)
{
	for (int i = m_interval; i <= timeCount; i++) {
		if (m_totalClient > m_maxTotalInlineCustomers) 
			m_maxTotalInlineCustomers = m_totalClient;
		popAll(i);
		if (i % m_interval == 0) {
			if (!addCustomer()) m_left++;
		}
	}
}

Simulate::Simulate(int interval, algorithm algType, const char* qType) : m_interval(interval), m_algorithm(algType), m_qArr(NULL),m_left(0), m_totalClient(0), m_maxTotalInlineCustomers(0)
{
	m_qType = new char[strlen(qType) + 1];
	strcpy_s(m_qType, strlen(qType) + 1, qType);
	m_Qnumber = howManyQs();
	buildQ1();
}

Simulate::~Simulate()
{
	if (strcmp(m_qType, "q10_10") == 0 || strcmp(m_qType, "q10_R1_19") == 0) {// case 10 Queues
		for (int i = 0; i < 10; i++) {
			delete m_qArr[i];
		}
		delete[] m_qArr; m_qArr = NULL;
	}
	else
		if (strcmp(m_qType, "q100_1") == 0) {
			for (int i = 0; i < 100; i++) {
				delete m_qArr[i];
			}
			delete[] m_qArr; m_qArr = NULL;
		}
}

int Simulate::shortest(int first) 
{
	int index = first;

	for (int i = first + 1; i < m_Qnumber; i++) {
		if (!m_qArr[i]->isFull() && m_qArr[i]->getSize() < m_qArr[index]->getSize()) {
			index = i;
		}
	}
	return index;
}

int Simulate::longest(int first) 
{
	int index = first;

	for (int i = first + 1; i < m_Qnumber; i++) {
		if (!m_qArr[i]->isFull() && m_qArr[i]->getSize() > m_qArr[index]->getSize()) {
			index = i;
		}
	}
	return index;
}

int Simulate::fastest(int first) 
{
	int index = first;

	for (int i = first + 1; i < m_Qnumber; i++) {
		if (!m_qArr[i]->isFull() && (int)(*m_qArr[i]) < (int)(*m_qArr[index])) {
			index = i;
		}
	}
	return index;
}

int Simulate::random() 
{
	return rand() % m_Qnumber;
}

void Simulate::buildQ1()
{
	if (strcmp(m_qType, "q10_10") == 0) { m_qArr = buildQ2(10, false); return; }
	if (strcmp(m_qType, "q100_1") == 0) { m_qArr = buildQ2(1, false); return; }
	if (strcmp(m_qType, "q10_R1_19") == 0) { m_qArr = buildQ2(0, true); return; }
}

bool Simulate::addCustomer()
{
	int firstQ = findFirst();
	if (firstQ < 0) return false;

	int Index;

	switch(m_algorithm) {
	case shor: 
		Index = shortest(firstQ);
		break;
	case lon: 
		Index = longest(firstQ);
		break;
	case fas: 
		Index = fastest(firstQ);
		break;
	case ran: 
		Index = random();
		break;
	}

	m_qArr[Index]->push(5);
	m_totalClient = m_totalClient + 1;
	return true;
}

int Simulate::findFirst()
{
	for (int i = 0; i < m_Qnumber; i++) {
		if (!m_qArr[i]->isFull()) return i;
	}
	return -1;
}

int Simulate::howManyQs() 
{
	if (strcmp(m_qType, "q10_10") == 0) return 10;
	if (strcmp(m_qType, "q10_R1_19") == 0) return 10;
	if (strcmp(m_qType, "q100_1") == 0) return 100;
	else return 100;
}

void Simulate::popAll(int t_i)
{
	for (int i = 0; i < m_Qnumber; i++) {
		if (t_i % (*m_qArr[i]).getServiceTime() == 0) {
			(*m_qArr[i]).pop();
			m_totalClient--;
		}
	}
}

Queue ** Simulate::buildQ2(int qLen, bool randomLen)
{
	Queue** returnArr = new Queue*[m_Qnumber];

	for (int i = 0; i < m_Qnumber; i++) {
		int length = (randomLen ? rand() % 19 + 1 : qLen);
		returnArr[i] = new Queue(length);
	}
	return returnArr;
}