#include <string>

using namespace std;

#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
public:
	Character();

	int getRow();

	void setRow(int in);

	int getCol();

	void setCol(int in);

	int getOgRow();

	void setOgRow(int in);

	int getOgCol();

	void setOgCol(int in);

	int getSpd();

	void setSpd(int in);

	int getHp();

	void setHp(int in);

protected:
	int speed;
	int maxHp;
	int currHp;
	int strength;
	int def;
	int currRow;
	int currCol;
	int ogRow;
	int ogCol;
	string name;
};

Character::Character()
{
	speed = 1;
	maxHp = 3;
	currHp = 3;
	strength = 1;
	def = 1;
	currRow = 1;
	currCol = 4;
	ogRow = 1;
	ogCol = 4;
	name = "";
}

int Character::getRow()
{
	return currRow;
}

void Character::setRow(int in)
{
	currRow = in;
}

int Character::getCol()
{
	return currCol;
}

void Character::setCol(int in)
{
	currCol = in;
}

int Character::getOgRow()
{
	return ogRow;
}

void Character::setOgRow(int in)
{
	ogRow = in;
}

int Character::getOgCol()
{
	return ogCol;
}

void Character::setOgCol(int in)
{
	ogCol = in;
}

int Character::getSpd()
{
	return speed;
}

void Character::setSpd(int in)
{
	speed = in;
}

int Character::getHp()
{
	return currHp;
}

void Character::setHp(int in)
{
	currHp = in;
}

#endif

