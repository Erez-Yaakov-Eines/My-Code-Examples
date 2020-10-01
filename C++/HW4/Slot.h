#pragma once
#include "Player.h"
using namespace std;
class Player;

class Slot
{
public:

					Slot(int slotNum);
	virtual			~Slot() {}
	virtual bool	play(Player* p) = 0;
	virtual	void	print() const = 0;
	int				getSlotNum() const;

protected:
	int				m_slotNum;
};

