#include "MortgagedAsset.h"
#include "Player.h"
#include "Board.h"

bool MortgagedAsset::mortgageAsset(float amount, Asset** a, Board* board)
{
	return false;
}

MortgagedAsset::MortgagedAsset(const Asset & asset, float mortgageAmount) : Asset(asset), m_originalMortgage(mortgageAmount), m_mortgagePayment(mortgageAmount)
{
}

MortgagedAsset::~MortgagedAsset()
{
}

void MortgagedAsset::updateAssetValue()
{
	m_mortgagePayment *= (1 + interestRate);
}

bool MortgagedAsset::buyAsset(Player * p)
{
	if (p->getSerial() == m_owner->getSerial()) //player p owns this mortgaged property
	{
		if (p->getFunds() >= m_mortgagePayment) { // player has sufficient funds
			p->freeAsset(this);
			return true;
		}// player p has insufficient funds
		cout << "player " << p->getName() << " has insufficient funds to unmortgage" << endl;
		return true;
	}
	cout << "player " << p->getName() << " does not own this mortgaged property" << endl;
	return true;
}

bool MortgagedAsset::play(Player * p)
{
	return buyAsset(p);
}

//void MortgagedAsset::toos(ostream & os) const
//{
//	os << "original mortgage is " << m_originalMortgage << "." << endl;
//	os << "mortgage payment is " << m_mortgagePayment << "." << endl << endl;
//}

void MortgagedAsset::print() const
{
	cout << "\nThis asset is mortgaged.";
	Asset::print();
	cout << "original mortgage is " << m_originalMortgage << "." << endl;
	cout << "mortgage payment is " << m_mortgagePayment << "." << endl << endl;
}

float MortgagedAsset::getMortgagePayment() const
{
	return m_mortgagePayment;
}
