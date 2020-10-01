#include "Instruction.h"
#include <iostream>
using namespace std;

Instruction::Instruction(int boardIndex, string instruction, bool isJail) : Slot(boardIndex), m_instructionText(instruction), m_isGoToJail(isJail)
{
}

Instruction::~Instruction()
{
}

bool Instruction::play(Player* p)
{
	if (m_isGoToJail == true) {// TODO - check jail works
		p->updateJailStatus(true);
		cout << "Go to jail.\n";
		return true;
	}

	cout << m_instructionText << endl;
	return true;
}

void Instruction::print() const
{
	cout << "\nSlot # " << m_slotNum << "\nInstruction: " << m_instructionText << endl;
}
