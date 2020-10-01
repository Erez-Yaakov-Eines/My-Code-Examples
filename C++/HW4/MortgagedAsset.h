#ifndef MORTGAGEDASSET_H

#define MORTGAGEDASSET_H
#include "Asset.h"
#include <iostream>
using namespace std;

class MortgagedAsset : public Asset
{
public:
					MortgagedAsset(const Asset& asset, float mortgageAmount);
	virtual			~MortgagedAsset();
	virtual bool	mortgageAsset(float amount, Asset** a, Board* board);
	virtual void	updateAssetValue();
	virtual bool	buyAsset(Player* p);
	virtual bool	play(Player* p);
	//virtual void	toos(ostream & os) const;
	virtual	void	print() const;
	float			getMortgagePayment() const;

protected:

	float m_originalMortgage;
	float m_mortgagePayment;

};

#endif
