#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
using namespace std;

Actor::Actor(StudentWorld* ptr, int ID, int x, int y, int startDir)
	: GraphObject(ID, y, x, startDir), myWorld(ptr)
{
	setVisible(true); 
}

Actor::~Actor()
{}

StudentWorld* Actor::getWorld()
{
	return myWorld;
}

Wall::Wall(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_WALL, x, y)
{
	setVisible(true);
	// dummy - ADD!
}

void Wall::doSomething()
{};

bool Wall::canMoveHere()
{
	return false;
}

Avator::Avator(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_PLAYER, x, y, right)
{
	hitPoints = 20;
	numPeas = 20;

	setVisible(true);
	// dummy - ADD!
}

int Avator::getHealth()
{
	return 100 * (hitPoints / 20); // health percentage
}

int Avator::getAmmo()
{
	return numPeas;
}

bool Avator::canMoveHere()
{
	return true; // doesn't really make sense - can avator move to where it already is
}

void Avator::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch)) // user did hit a key
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (getWorld()->checkIfCanMoveHere(getX() - 1, getY()))
				moveTo(getX() - 1, getY()); // move 1 square to the left
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (getWorld()->checkIfCanMoveHere(getX() + 1, getY()))
				moveTo(getX() + 1, getY()); // move 1 square to the right
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (getWorld()->checkIfCanMoveHere(getX(), getY() + 1))
				moveTo(getX(), getY() + 1); // move 1 square up
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (getWorld()->checkIfCanMoveHere(getX(), getY() - 1))
				moveTo(getX(), getY() - 1); // move 1 square down
			break;
		default:
			cerr << "something went wrong"; // EDIT after you account for all key cases
			break;
		}
	}

	/*
		const int KEY_PRESS_SPACE = ' '; // add a pea in the square in front of the avator
		const int KEY_PRESS_ESCAPE = '\x1b';
		const int KEY_PRESS_TAB = '\t';
		const int KEY_PRESS_ENTER
	*/
};
