#include "Character.h"
#include "Ally.h"
#include "Player.h"
#include "Enemy.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

#ifndef GAME_H
#define GAME_H


class Game {
public:
	Game();

	void openMap(string filename);

	void updateBattle(int curr);

	bool playerControlBattle(int curr, bool& turn);

	void updateCity();

	void updateWorld();

	bool playerControl();

	void makePlayer();

	int getPartySize();

	int getEnemyNum();

	bool getBattle();

	bool getWorld();

	bool getCity();

	bool nextToChar(Character* c, char in);

	void enemyTurn(int curr);

	double distance(Character* c1, Character* c2);

	int getClosest(Character* enemy);

	void makeEnemies();

	void printMap();

	int getP1Hp();

private:
	static const int MAX_MAP_ROWS = 100;
	static const int MAX_MAP_COLS = 300;

	Player p1;
	vector<Character*> party;
	vector<Enemy*> enemies;
	char map[MAX_MAP_ROWS][MAX_MAP_COLS];
	int rows;
	int cols;

	bool battle;
	bool world;
	bool city;

	int enemyNum;
};
#endif

int main(int argc, char* argv[])
{
	Game* game = new Game();
	game->makePlayer();

	while (true)
	{
		if (game->getBattle())
		{
			game->makeEnemies();
			game->printMap();
			while (game->getP1Hp() > 0 && game->getEnemyNum() > 0)
			{
				for (int i = 0; i < game->getPartySize(); i++)
				{
					game->updateBattle(i);
				}
				for (int x = 0; x < game->getEnemyNum(); x++)
				{
					game->enemyTurn(x);
				}
			}
		}
		else if (game->getWorld())
		{
			game->updateWorld();
		}
		else
		{
			game->updateCity();
		}
	}
}

Game::Game()
{
	battle = false;
	world = false;
	city = true;
	p1.setRow(23);
	p1.setCol(8);
	openMap("city.txt");
}

int Game::getP1Hp()
{
	return p1.getHp();
}

bool Game::nextToChar(Character* c, char in)
{
	if (c->getRow() > 0
		&& map[c->getRow() - 1][c->getCol()] == in)
	{
		return true;
	}
	else if (c->getCol() > 0
		&& map[c->getRow()][c->getCol() - 1] == in)
	{
		return true;
	}
	else if (c->getRow() < rows - 1
		&& map[c->getRow() + 1][c->getCol()] == in)
	{
		return true;
	}
	else if (c->getCol() < cols - 1
		&& map[c->getRow()][c->getCol() + 1] == in)
	{
		return true;
	}
	return false;
}

void Game::updateBattle(int curr)
{
	bool turn = true;
	while (turn)
	{
		Character* c = party[curr];
		if (playerControlBattle(curr, turn))
		{
			if (curr == 0)
				map[c->getRow()][c->getCol()] = '0';
			else
				map[c->getRow()][c->getCol()] = 'o';

			printMap();
		}
	}
	if (nextToChar(party[curr], 'x'))
	{
		//save current map
		//print battle map
	}
}

void Game::printMap()
{
	cout << flush;
	system("CLS");
	for (int i = 0; i < rows; i++)
	{
		for (int x = 0; x < cols; x++)
		{
			cout << map[i][x];
		}
		cout << endl;
	}
}

void Game::makeEnemies()
{
	for (int i = 0; i < enemyNum; i ++)
	{
		Enemy* e = new Enemy;
		e->setRow(i * ((rows - 1) / enemyNum));
		e->setOgRow(i * ((rows - 1) / enemyNum));
		e->setCol(cols - 2);
		e->setOgCol(cols - 2);
		enemies.push_back(e);
		map[i][cols - 2] = 'x';
	}
}

void Game::enemyTurn(int curr)
{
	Enemy* e = enemies[curr];
	int closestPlayerIndex = getClosest(e);
	Character* c = party[closestPlayerIndex];

	if (c->getRow() < e->getRow())
	{
		while (e->getRow() > c->getRow() && e->getRow() >= e->getOgRow() - e->getSpd())
		{
			if (e->getRow() > 0
				&& map[e->getRow() - 1][e->getCol()] == ' ')
			{
				e->setRow(e->getRow() - 1);
			}
			else
				break;
		}
		map[e->getOgRow()][e->getOgCol()] = ' ';
		e->setOgRow(e->getRow());
	}
	else if (c->getRow() > e->getRow())
	{
		while (e->getRow() < c->getRow() && e->getRow() <= e->getOgRow() + e->getSpd())
		{
			if (e->getRow() < rows - 1
				&& map[e->getRow() + 1][e->getCol()] == ' ')
			{
				e->setRow(e->getRow() + 1);
			}
			else
				break;
		}
		map[e->getOgRow()][e->getOgCol()] = ' ';
		e->setOgRow(e->getRow());
	}

	if (c->getCol() < e->getCol())
	{
		while (e->getCol() > c->getCol() && e->getCol() >= e->getOgCol() - 3 * e->getSpd())
		{
			if (e->getCol() > 3
				&& map[e->getRow()][e->getCol() - 3] == ' ')
			{
				e->setCol(e->getCol() - 3);
			}
			else
				break;
		}
		map[e->getOgRow()][e->getOgCol()] = ' ';
		e->setOgCol(e->getCol());
	}
	else if (c->getCol() > e->getCol())
	{
		while (e->getCol() < c->getCol() && e->getCol() <= e->getOgCol() + 3 * e->getSpd())
		{
			if (e->getCol() < cols - 2
				&& map[e->getRow()][e->getCol() + 3] == ' ')
			{
				e->setCol(e->getCol() + 3);
			}
			else
				break;
		}
		map[e->getOgRow()][e->getOgCol()] = ' ';
		e->setOgCol(e->getCol());
	}

	map[e->getRow()][e->getCol()] = 'x';
	printMap();
}

double Game::distance(Character* c1, Character* c2)
{
	return sqrt(pow((c1->getCol() - c2->getCol()), 2) + 
		pow((c1->getRow() - c2->getRow()), 2));
}

int Game::getClosest(Character* enemy)
{
	double min = distance(enemy, party[0]);
	int minIndex = 0;
	for (int i = 0; i < party.size(); i++)
	{
		double d = distance(enemy, party[i]);
		if (d < min)
		{
			min = d;
			minIndex = i;
		}
	}
	return minIndex;
}

bool Game::getBattle()
{
	return battle;
}

bool Game::getWorld()
{
	return world;
}

bool Game::getCity()
{
	return city;
}

bool Game::playerControlBattle(int curr, bool& turn)
{
	Character* c = party[curr];
	if (GetAsyncKeyState(0x0D) & 0x0001)
	{
		c->setOgRow(c->getRow());
		c->setOgCol(c->getCol());
		turn = false;
		return true;
	}
	if (GetAsyncKeyState(0x57) & 0x0001)
	{
		if (c->getRow() > 0
			&& c->getRow() - c->getSpd() >= c->getOgRow() - c->getSpd()
			&& map[c->getRow() - 1][c->getCol()] == ' ')
		{
			map[c->getRow()][c->getCol()] = ' ';
			c->setRow(c->getRow() - 1);
			return true;
		}
	}
	if (GetAsyncKeyState(0x41) & 0x0001)
	{
		if (c->getCol() > 3
			&& c->getCol() - 3 * c->getSpd() >= c->getOgCol() - 3 * c->getSpd()
			&& map[c->getRow()][c->getCol() - 3] == ' ')
		{
			map[c->getRow()][c->getCol()] = ' ';
			c->setCol(c->getCol() - 3);
			return true;
		}
	}
	if (GetAsyncKeyState(0x53) & 0x0001)
	{
		if (c->getRow() < rows - 1
			&& c->getRow() + c->getSpd() <= c->getOgRow() + c->getSpd()
			&& map[c->getRow() + 1][c->getCol()] == ' ')
		{
			map[c->getRow()][c->getCol()] = ' ';
			c->setRow(c->getRow() + 1);
			return true;
		}
	}
	if (GetAsyncKeyState(0x44) & 0x0001)
	{
		if (c->getCol() < cols - 2
			&& c->getCol() + 3 * c->getSpd() <= c->getOgCol() + 3 * c->getSpd()
			&& map[c->getRow()][c->getCol() + 3] == ' ')
		{
			map[c->getRow()][c->getCol()] = ' ';
			c->setCol(c->getCol() + 3);
			return true;
		}
	}
	return false;
}

void Game::updateCity()
{
	if (p1.getCol() == cols - 1)
	{
		//battle = true;
		city = false;
		world = true;
		p1.setRow(14);
		p1.setCol(0);
		openMap("world.txt");
	}

	if (playerControl())
	{
		cout << flush;
		system("CLS");
		for (int i = 0; i < rows; i++)
		{
			for (int x = 0; x < cols; x++)
			{
				if (i == p1.getRow() && x == p1.getCol())
				{
					cout << '0';
				}
				else
				{
					cout << map[i][x];
				}
			}
			cout << endl;
		}
	}
}

bool Game::playerControl()
{
	if (GetAsyncKeyState(0x57) & 0x0001)
	{
		if (p1.getRow() > 0
			&& map[p1.getRow() - 1][p1.getCol()] == ' ')
		{
			p1.setRow(p1.getRow() - 1);
			return true;
		}
	}
	if (GetAsyncKeyState(0x41) & 0x0001)
	{
		if (p1.getCol() > 0
			&& map[p1.getRow()][p1.getCol() - 1] == ' ')
		{
			p1.setCol(p1.getCol() - 1);
			return true;
		}
	}
	if (GetAsyncKeyState(0x53) & 0x0001)
	{
		if (p1.getRow() < rows - 1
			&& map[p1.getRow() + 1][p1.getCol()] == ' ')
		{
			p1.setRow(p1.getRow() + 1);
			return true;
		}
	}
	if (GetAsyncKeyState(0x44) & 0x0001)
	{
		if (p1.getCol() < cols - 1
			&& map[p1.getRow()][p1.getCol() + 1] == ' ')
		{
			p1.setCol(p1.getCol() + 1);
			return true;
		}
	}
	return false;
}

void Game::updateWorld()
{
	if (p1.getCol() == 0)
	{
		city = true;
		world = false;
		openMap("city.txt");
		p1.setRow(14);
		p1.setCol(cols - 1);
	}

	if (nextToChar(&p1, 'x'))
	{
		if (p1.getCol() == 35)
		{
			battle = true;
			world = false;
			openMap("battle1.txt");
			p1.setRow(5);
			p1.setCol(4);
			p1.setOgRow(5);
			p1.setOgCol(4);
			map[5][4] = '0';
		}
		else if (p1.getCol() == 59)
		{

		}
		else if (p1.getRow() == 20 && p1.getCol() != 64)
		{

		}
		else if (p1.getCol() == 64)
		{

		}
		else if (p1.getCol() == 58)
		{

		}
		else if (p1.getCol() == 42 && p1.getRow() != 31)
		{

		}
		else if (p1.getRow() == 31)
		{

		}
		else if (p1.getRow() == 39)
		{

		}
	}

	if (playerControl())
	{
		cout << flush;
		system("CLS");
		for (int i = 0; i < rows; i++)
		{
			for (int x = 0; x < cols; x++)
			{
				if (i == p1.getRow() && x == p1.getCol())
				{
					cout << '0';
				}
				else
				{
					cout << map[i][x];
				}
			}
			cout << endl;
		}
	}
}

void Game::makePlayer()
{
	cout << "Please input a name for your character." << endl;
	string name = "";
	cin >> name;
	p1.setName(name);
	party.push_back(&p1);
}

int Game::getPartySize()
{
	return party.size();
}

int Game::getEnemyNum()
{
	return enemies.size();
}

void Game::openMap(string filename)
{
	fstream f;
	f.open(filename);
	if (!f.is_open())
	{
		cout << "ERROR: Invalid Filename" << endl;
		return;
	}
	bool enemies;

	string temp;
	f >> temp;
	rows = stoi(temp);
	f >> temp;
	cols = stoi(temp);
	f >> temp;
	enemies = stoi(temp);
	f >> temp;
	enemyNum = stoi(temp);

	char in;
	int row = 0;
	int col = 0;
	f >> noskipws;
	while (f >> in)
	{
		if (in != '\n')
		{
			if (col == cols)
			{
				col = 0;
				row++;
			}
			if (row == rows)
				break;
			map[row][col] = in;
			col++;
		}
	}
}