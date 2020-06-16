#include "Character.h"
#include <string>

using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

class Player : public Character {
public:
	Player();

	void setName(string nameIn);

private:

};

Player::Player()
{

}

void Player::setName(string nameIn)
{
	name = nameIn;
}

#endif
