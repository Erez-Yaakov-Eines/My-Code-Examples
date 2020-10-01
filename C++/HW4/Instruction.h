#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Slot.h"
#include <string>
using namespace std;

class Slot;

class Instruction : public Slot
{
public:
					Instruction(int boardIndex, string instruction, bool isJail);
	virtual			~Instruction();
	virtual bool	play(Player* p);
	virtual	void	print() const;

protected:
	bool			m_isGoToJail;
	string			m_instructionText;
};

#endif
