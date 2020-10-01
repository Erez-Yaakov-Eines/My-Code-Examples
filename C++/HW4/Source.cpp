#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Slot.h"
#include "Asset.h"
#include "Player.h"
#include "Board.h"

using namespace std;

void addStandardAssets(Board& b)
{
	b.addInstructionSlot("start-get 350 Nis", false);
	b.addAsset("Tel-Aviv", "Bograshov");
	b.addAsset("Tel-Aviv", "Dizengoff");
	b.addAsset("Utilities", "Electric Company");
	b.addInstructionSlot("Go to jail", true);

	b.addAsset("Jerusalem", "Ben Yehuda");
	b.addInstructionSlot("take lucky ticket", false);
	b.addAsset("Jerusalem", "Emek Refaim");
	b.addAsset("Utilities", "Railroad");
	b.addInstructionSlot("take lucky ticket", false);

	b.addAsset("Haifa", "Sderot HaMeginim");
	b.addInstructionSlot("take lucky ticket", false);
	b.addAsset("Haifa", "Stella Maris");
	b.addAsset("Haifa", "Hatzionut Ave");

	b.addAsset("Utilities", "Water");
	b.addAsset("Eilat", "Derech HaGai");
	b.addAsset("Eilat", "Eilat Airport");
	b.addAsset("Eilat", "Derech Harim");
}
void runMonopol(Board& b, Player* playersArr, int numOfPlayers)
{
	bool play = true;
	int count = 0;

	while (play) {
		Player * p = &playersArr[count % Player::count];
		cout << "\nPlayer no: " << count % Player::count + 1 << "'s turn. ";

		cout << "\nto continue press (1), To end game press (0)\n";
		cin >> play;
		if (play == false)
			break;

		if (!p->rollDice())
			break;
		count++;
		cout << "\n##########################################################\n";
	}
	cout << "\n\nthe game lasted for " << count << " rounds.\n\n";
}

int main() {
	srand(time(NULL));

	Board b; addStandardAssets(b);
	Player playersArr[2] = { Player("Neta",&b), Player("Yael",&b) };
	runMonopol(b, playersArr, 3);

	cout << b;

	return EXIT_SUCCESS;
}