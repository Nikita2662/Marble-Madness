#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
using namespace std;

//////////////////////////// ACTOR CLASS /////////////////////////
Actor::Actor(StudentWorld* ptr, int ID, int x, int y, int startDir)
	: GraphObject(ID, y, x, startDir), myWorld(ptr), alive(true)
{ setVisible(true); }

Actor::~Actor() {}

StudentWorld* Actor::getWorld() const { return myWorld; }

bool Actor::isAlive() const { return alive; }

void Actor::setDead() { alive = false; }
//////////////////////////// ACTOR CLASS /////////////////////////

//////////////////////////// WALL CLASS /////////////////////////
Wall::Wall(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_WALL, x, y)
{
	// dummy - ADD!
}

void Wall::doSomething() {};

bool Wall::allowsColocationBy(Actor* a) const { return false; }
//////////////////////////// WALL CLASS /////////////////////////

//////////////////////////// AVATOR CLASS /////////////////////////
Avator::Avator(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_PLAYER, x, y, right), hitPoints(20), numPeas(20)
{
	// dummy - ADD!
}

// health percentage
int Avator::getHealth() const { return 100 * (hitPoints / 20); }

int Avator::getAmmo() const { return numPeas; }

bool Avator::allowsColocationBy(Actor* a) const 
{
	return true; // EDIT
} 

// move to the adjacent square in the direction avator currently faces, if not blocked
bool Avator::moveIfPossible()
{
	if (getDirection() == left && getWorld()->checkIfCanMoveHere(getX() - 1, getY(), this)) {
		moveTo(getX() - 1, getY()); // move 1 square to the left
		return true;
	}
	else if (getDirection() == right && getWorld()->checkIfCanMoveHere(getX() + 1, getY(), this)) {
		moveTo(getX() + 1, getY()); // move 1 square to the right
		return true;
	}
	else if (getDirection() == up && getWorld()->checkIfCanMoveHere(getX(), getY() + 1, this)) {
		moveTo(getX(), getY() + 1); // move 1 square up
		return true;
	}
	else if (getDirection() == down && getWorld()->checkIfCanMoveHere(getX(), getY() - 1, this)) {
		moveTo(getX(), getY() - 1); // move 1 square down
		return true;
	}
	return false; // invalid direction or blocked --> did not move 
}

void Avator::doSomething()
{
	if (!isAlive()) return;

	int ch;
	if (getWorld()->getKey(ch)) // user did hit a key
	{
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
		case KEY_PRESS_SPACE:
			if (numPeas > 0) {
				// get x and y of where pea should be fired
				int x = getX();
				int y = getY();
				if (getDirection() == left) x--;
				else if (getDirection() == right) x++;
				else if (getDirection() == up) y++;
				else if (getDirection() == down) y--;
				else cerr << "Player's direction is none, cannot fire pea";

				Pea* p = new Pea(x, y, getWorld(), getDirection()); // create new Pea
				getWorld()->addActor(p); // add to actors array
				getWorld()->playSound(SOUND_PLAYER_FIRE); // sound
				numPeas--; // decrement
			}
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			moveIfPossible();
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			moveIfPossible();
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			moveIfPossible();
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			moveIfPossible();
			break;
		default:
			cerr << "invalid key press";
			break;
		}
	}
};
//////////////////////////// AVATOR CLASS /////////////////////////

//////////////////////////// PEA CLASS /////////////////////////
Pea::Pea(int x, int y, StudentWorld* ptr, int dir)
	: Actor(ptr, IID_PEA, x, y, dir)
{}

void Pea::doSomething()
{
	// ADD
}

bool Pea::allowsColocationBy(Actor* a) const
{
	return true; // EDIT AS NEEDED BY PEA SPEC
}

//////////////////////////// PEA CLASS /////////////////////////
