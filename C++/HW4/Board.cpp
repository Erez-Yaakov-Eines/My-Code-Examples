#include "Board.h"
#include "Asset.h"
#include "MortgagedAsset.h"
#include <typeinfo.h>
using namespace std;

void Board::addAsset( string city,string propertyName) {

	Slot ** tmp = new Slot *[m_size + 1];
	int i;
	for (i = 0; i < m_size; i++) { tmp[i] = m_arr[i]; }
	tmp[i] = new Asset(city, propertyName, m_size + 1);
	m_size++;
	if (m_arr) delete[] m_arr;
	m_arr = tmp;
}

void Board::addInstructionSlot(string instruction, bool isJail) {
	
	Slot ** tmp = new Slot *[m_size + 1];
	int i;
	for (i = 0; i < m_size ; i++) { tmp[i] = m_arr[i]; }
	tmp[i] = new Instruction(m_size + 1, instruction, isJail);
	m_size++;
	if (m_arr) delete[] m_arr;
	m_arr = tmp;
}

Asset * Board::mortgagedAsset(int slotIdx,float amount) {
	Slot * tmp = m_arr[slotIdx];
	Asset * newAsset = new MortgagedAsset(*(Asset *)tmp,amount);
	m_arr[slotIdx] = newAsset;
	delete tmp;
	return newAsset;
}

Asset* Board::freeAsset(int slotIdx) {
	Slot * tmp = m_arr[slotIdx];
	Asset * newAsset = new Asset(*(Asset *)tmp);
	m_arr[slotIdx] = newAsset;
	delete tmp;
	return newAsset;
}


int Board::getBoardSize() const
{
	return m_size;
}

Slot * Board::getLocationSlot(int idx)
{
	return m_arr[idx];
}

Board::~Board()
{
	for (int i = 0; i < m_size; ++i) delete m_arr[i];
	if (m_arr) delete[] m_arr;
}

ostream & operator<<(ostream & os, const Board & board)
{
	cout << "\n\n*****************\nCurrent board status:\n\n";
	for (int i = 0; i < board.m_size; ++i)
	{
		Slot* currentSlot = board.m_arr[i];
		currentSlot->print();
	}
	cout << "\n\n*****************\n";
	return os;
}
