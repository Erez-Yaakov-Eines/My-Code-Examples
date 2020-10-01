#ifndef PLAYER_H
#define PLAYER_H

#include "Slot.h"
#include <string>
using namespace std;

class Asset;
class Slot;
class Board;

using namespace std;

class Player {
public:
	static int count;

	Player(const char* name, Board* board);
	~Player();

	void updateJailStatus(bool isJailed);
	bool updateCurrLocation(int idx);
	void addAsset(Asset * a);
	bool payRent(int amount);
	Asset* freeAsset(Asset* a);
	bool rollDice();

	int getSerial() const;
	int getFunds() const;
	const string& getName() const;

	bool setFunds(int amount);

private:
	const int	m_serial;
	Board*		m_board;
	const		string m_name;
	int			m_funds;
	int			m_positionIndex;
	bool		m_isJailed;
	int			m_assetListSize;
	Asset**		m_playerAssets;

	bool		tryMortgage(float amount);
	int			howMuchMortgage(float amount, Asset* asset);
};

#endif
