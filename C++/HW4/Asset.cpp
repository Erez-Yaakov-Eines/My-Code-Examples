#include "Asset.h"
#include <iostream>
#include <string>
#include <ctime>
#include "Board.h"
using namespace std;

const float Asset::interestRate =  0.05;

Asset::Asset(string city, string propertyName, int boardIndex)
	: m_city(city), m_propertyGroup(propertyName), Slot(boardIndex)
{
	m_price = rand() % 401 + 100;
	m_rent = rand() % 96 + 5;
}

Asset::Asset(const Asset & other) 
	: Slot(other.m_slotNum), m_city(other.m_city), m_propertyGroup(other.m_propertyGroup), m_price(other.m_price), m_rent(other.m_rent), m_owner(other.m_owner)
{
}

bool Asset::mortgageAsset(float amount, Asset** a, Board* board)
{
	*a = board->mortgagedAsset(m_slotNum - 1, amount);
	return true;
}

Asset::~Asset()
{

}

bool Asset::play(Player* p)
{
	return buyAsset(p);
}

void Asset::updateAssetValue()
{
	m_rent *= (1 + interestRate);
}

bool Asset::buyAsset(Player * p)
{
	if (isOwned() && p->getSerial() == m_owner->getSerial()) //player p owns this property
	{
		cout << "Player " << p->getName() << " owns this property" << endl;
		return true;
	}

	if (!isOwned()) { //unOwned property
		if (m_price <= p->getFunds()) {
			p->addAsset(this);
			cout << "Player " << p->getName() << " just bought:"; print();/* << *this << endl;*/
			return true;
		}
		cout << "Player " << p->getName() << " cannot afford to buy:"; print();/* << *this;*/
		return true;
	}

	if (isOwned()) //another player owns this property
	{
		if (!p->payRent(m_rent)) return false; // player could not pay rent
		m_owner->setFunds(m_owner->getFunds() + m_rent);// Rent paid
	}
	return true;
}

void Asset::print() const
{
	cout << "\nSlot #" << (*this).m_slotNum << "." << endl;
	cout << "Asset: " << endl;
	cout << "City name: " << (*this).m_city << "." << endl;
	cout << "Property group: " << (*this).m_propertyGroup << "." << endl;
	if (isOwned()) cout << "Owner: " << (*this).m_owner->getName() << endl;
	else cout << "Owner: not owned" << endl;
	cout << "Rent: " << (*this).m_rent << "." << endl;
	cout << "Buy price is " << (*this).m_price << "." << endl;
}

//void Asset::toos(ostream & os) const
//{
//}

bool Asset::isOwned() const
{
	return (m_owner != NULL) ? true : false;
}

float Asset::getPrice() const
{
	return m_price;
}

const Player * Asset::getOwner() const
{
	return m_owner;
}

const string & Asset::getCity() const
{
	return m_city;
}

const string & Asset::getPropertyGroup() const
{
	return m_propertyGroup;
}

float Asset::getRent() const
{
	return m_rent;
}

void Asset::setOwner(Player* owner)
{
	m_owner = owner;

}

//ostream & operator<<(ostream & os, const Asset & asset)
//{
//	cout << endl << "Asset: " << endl;
//	cout << "City name: " << asset.m_city << "." << endl;
//	cout << "Property group: " << asset.m_propertyGroup << "." << endl;
//	cout << "Slot #" << asset.m_slotNum << "." << endl;
//	cout << "Rent: " << asset.m_rent << "." << endl;
//	cout << "Buy price is " << asset.m_price << "." << endl;
//	asset.toos(os);
//	return os;
//}
