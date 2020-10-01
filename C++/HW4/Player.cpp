#include "Player.h"
#include <iostream>
using namespace std;
#include "Asset.h"
#include "Board.h"

int Player::count; // statics are auto initialized to 0

Player::Player(const char * name, Board * board) 
	: m_board(board), m_name(name), m_serial(++count), m_assetListSize(0), m_funds(350), m_playerAssets(NULL), m_positionIndex(0), m_isJailed(false)
{
}

Player::~Player()
{
	if (m_playerAssets) delete[] m_playerAssets;
}

void Player::updateJailStatus(bool isJailed)
{
	m_isJailed = isJailed;
}

bool Player::updateCurrLocation(int idx)
{
	int oldPosition = m_positionIndex;
	m_positionIndex = idx;
	return (m_positionIndex < oldPosition) ? true : false; // assumes dice roll is < board size
}

void Player::addAsset(Asset * a)
{
	if (a->getPrice() <= m_funds) {
		a->setOwner(this);
		m_funds -= a->getPrice();
		Asset** tempAssets = new Asset*[m_assetListSize + 1];
		int i;
		for (i = 0; i < m_assetListSize; ++i) { tempAssets[i] = m_playerAssets[i]; }
		tempAssets[i] = a;
		if (m_playerAssets) delete[] m_playerAssets;
		m_playerAssets = tempAssets;
		++m_assetListSize;
	}
}

bool Player::payRent(int amount)// TODO - work hard on streamlining this function
{
	cout << m_name << " has to pay " << amount << " in rent.\n";
	if (amount <= m_funds) { m_funds -= amount; cout << "Rent paid\n"; return true; }// player has rent money

	bool canPayRent = tryMortgage(amount);// player lacks money. try mortgaging.
	if (canPayRent == true) { m_funds -= amount; cout << "Rent paid\n"; return true; }
	//if reached, player cannot make rent -> player lost
	cout << m_name << " cannot pay rent." << endl;
	cout << "Game Over. " << m_name << " lost!" << endl;
	return false;
}

Asset * Player::freeAsset(Asset * a)
{
	for (int i = 0; i < m_assetListSize; ++i) {
		if (m_playerAssets[i] == a) {
			m_funds -= static_cast<MortgagedAsset*>(a)->getMortgagePayment();
			Asset* tempAsset = m_board->freeAsset(a->getSlotNum() - 1);
			m_playerAssets[i] = tempAsset;
			cout << "Player " << m_name << "freed Asset:"; tempAsset->print(); /*<< *tempAsset;*/
			return tempAsset;
		}
	}
	return nullptr;
}

bool Player::rollDice()
{
	if (m_isJailed) {
		m_isJailed = false;
		cout << "\nPlayer " << m_name << " got out of jail.\n";
		return true;
	}

	int dieOutcome = rand() % 6 + 1;// roll dice
	cout << "Player " << m_name << " rolled " << dieOutcome << endl;
	int newPosition = (m_positionIndex + dieOutcome) % m_board->getBoardSize();
	if (updateCurrLocation(newPosition) == true) // player passed GO
	{// player gets 350 funds & all asset values are increased
		m_funds += 350;
		for (int i = 0; i < m_assetListSize; ++i) m_playerAssets[i]->updateAssetValue();
	}
	cout << m_name << " is now at slot number " << newPosition + 1 << endl;
	cout << m_name << "'s funds before playing are " << m_funds << endl;
	bool result = (m_board->getLocationSlot(m_positionIndex))->play(this);
	cout << m_name << "'s funds after playing are " << m_funds << endl;

	return result;
}

int Player::getSerial() const
{
	return m_serial;
}

int Player::getFunds() const
{
	return m_funds;
}

const string & Player::getName() const
{
	return m_name;
}

bool Player::setFunds(int amount)
{
	m_funds = amount;
	return (m_funds > 0) ? true : false;
}

bool Player::tryMortgage(float amount)
{
	Asset** tempAssetPtr = new Asset*;

	for (int i = 0; i < m_assetListSize; ++i) {
		
		int mortgage2Take = howMuchMortgage(amount, m_playerAssets[i]);
		if (m_playerAssets[i]->mortgageAsset(mortgage2Take, tempAssetPtr, m_board) == true) {
			m_funds += mortgage2Take;
			m_playerAssets[i] = *tempAssetPtr;
			cout << endl << m_name << " just mortgaged:"; (**tempAssetPtr).print(); /*<< **tempAssetPtr;*/
		}
		if (amount - m_funds <= 0) break;
	}
	delete tempAssetPtr;

	if (amount - m_funds <= 0) { return true; }
	return false;
}

int Player::howMuchMortgage(float amount, Asset * asset)
{
	float remainingNeeded = amount - m_funds;
	int currentMortgage2Take = asset->getPrice();
	if (currentMortgage2Take >= remainingNeeded) currentMortgage2Take = remainingNeeded;
	return currentMortgage2Take;
}
