#include "Slot.h"
#include "Asset.h"
#include "MortgagedAsset.h"
#include "Instruction.h"

Slot::Slot(int slotNum) : m_slotNum(slotNum)
{
}

int Slot::getSlotNum() const
{
	return m_slotNum;
}
