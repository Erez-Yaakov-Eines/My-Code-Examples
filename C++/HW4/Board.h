#ifndef BOARD_H
#define BOARD_H
//#include "Slot.h"

#include "MortgagedAsset.h"
#include "Instruction.h"
#include <string>
using namespace std;

class Board {

private:
	int m_size = 0;
	Slot** m_arr = NULL;

public:

	~Board();

	void		addAsset(string city, string propertyName);
	void		addInstructionSlot(string instruction, bool isJail=false);
	Asset*		mortgagedAsset(int slotIdx, float amount);
	Asset*		freeAsset(int slotIdx);
	int			getBoardSize()const;
	Slot*		getLocationSlot(int idx);

	friend ostream& operator<<(ostream& os, const Board& board);
};

#endif
