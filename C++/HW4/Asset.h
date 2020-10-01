#ifndef ASSET_H
#define ASSET_H
#include "Slot.h"
#include <string>
#include "Instruction.h"
using namespace std;

class Slot;
class Player;

class Asset : public Slot
{
public:
					Asset(string city, string propertyName, int boardIndex);
					Asset(const Asset& other);
	virtual			~Asset();
	virtual bool	mortgageAsset(float amount, Asset** a, Board* board);

	virtual bool	play(Player* p);
	virtual void	updateAssetValue();
	virtual bool	buyAsset(Player* p);
	//virtual void	toos(ostream & os) const;
	virtual	void	print() const;

	//friend ostream& operator<<(ostream& os, const Asset& asset);

	bool			isOwned() const;
	float			getPrice() const;
	const Player*	getOwner() const;
	const string&	getCity() const;
	const string&	getPropertyGroup() const;
	float			getRent() const;
	void			setOwner(Player* owner);

protected:
	static const float interestRate;
	string	m_city;
	string	m_propertyGroup;
	float	m_price;
	float	m_rent;
	Player* m_owner;
};

#endif
