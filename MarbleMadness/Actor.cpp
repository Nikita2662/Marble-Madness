#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
using namespace std;

//////////////////////////// ACTOR CLASS /////////////////////////
Actor::Actor(StudentWorld* ptr, int ID, int x, int y, int startDir)
	: GraphObject(ID, y, x, startDir), myWorld(ptr), alive(true), hitPoints(-1)
{ setVisible(true); }

Actor::~Actor() {}

StudentWorld* Actor::getWorld() const { return myWorld; }

bool Actor::isAlive() const { return alive; }
void Actor::setDead() { alive = false; }
void Actor::setHP(int amt) { hitPoints = amt; }
int Actor::getHP() const { return hitPoints; }
  // only returns true for Pit or Empty
bool Actor::canContainMarblePush() { return false; }
  // returns if this kind of Actor could possibly be pushed by an Avator (only true for marble)
bool Actor::mayBePushedByPlayer() { return false; }
  // only allowed for Marbles
bool Actor::pushTo(int x, int y) { return false; }
//////////////////////////// ACTOR CLASS /////////////////////////

//////////////////////////// WALL CLASS /////////////////////////
Wall::Wall(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_WALL, x, y)
{}

void Wall::doSomething() {}

bool Wall::allowsAgentColocationBy(Actor* a) const { return false; }

  // returns if Wall can be hit by pea
bool Wall::isHittable() const { return true; }

  // when attacked by pea, suffer damage (here, none)
void Wall::damageBy(int damageAmt) {}
//////////////////////////// WALL CLASS /////////////////////////

//////////////////////////// AVATOR CLASS /////////////////////////
Avator::Avator(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_PLAYER, x, y, right), numPeas(20)
{
	setHP(20);
}

// health percentage
int Avator::getHealth() const { return 100 * (getHP() / 20); }

int Avator::getAmmo() const { return numPeas; }

bool Avator::allowsAgentColocationBy(Actor* a) const 
{
	return true; // EDIT
} 

// move to the adjacent square in the direction avator currently faces, if not blocked
bool Avator::moveIfPossible()
{
	if (getDirection() == left && getWorld()->canActorMoveHere(this, getX() - 1, getY())) {
			moveTo(getX() - 1, getY()); // move player 1 square to the left
			return true;
		}
	else if (getDirection() == right && getWorld()->canActorMoveHere(this, getX() + 1, getY())) {
			moveTo(getX() + 1, getY()); // move player 1 square to the right
			return true;
		}
	else if (getDirection() == up && getWorld()->canActorMoveHere(this, getX(), getY() + 1)) {
			moveTo(getX(), getY() + 1); // move player 1 square up
			return true;
		}
	else if (getDirection() == down && getWorld()->canActorMoveHere(this, getX(), getY() - 1)) {
			moveTo(getX(), getY() - 1); // move player 1 square down
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
				// create new pea with temp location of player
				Pea* p = new Pea(getX(), getY(), getWorld(), getDirection());

				// adjust position to directly in front of player
				if (getDirection() == left) 
					p->moveTo(getX() - 1, getY());
				else if (getDirection() == right) 
					p->moveTo(getX() + 1, getY());
				else if (getDirection() == up) 
					p->moveTo(getX(), getY() + 1);
				else if (getDirection() == down) 
					p->moveTo(getX(), getY() - 1);
				else 
					cerr << "Player's direction is none, cannot fire pea";

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

  // returns if Avator can be hit by pea
bool Avator::isHittable() const { return true; }

  // when attacked by pea, suffer damage
void Avator::damageBy(int damageAmt) 
{ 
	setHP(getHP() - damageAmt);
	if (getHP() > 0)
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
	else {
		setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
	}
}
//////////////////////////// AVATOR CLASS /////////////////////////

//////////////////////////// PEA CLASS /////////////////////////
Pea::Pea(int x, int y, StudentWorld* ptr, int dir)
	: Actor(ptr, IID_PEA, x, y, dir)
{}

void Pea::doSomething()
{
	// check if alive
	if (!isAlive()) return;

	// returns false meaning Pea did not damage, continued
	if (!getWorld()->tryToDamageLocation(this, getX(), getY())) 
	{
		if (getDirection() == left) moveTo(getX() - 1, getY());
		else if (getDirection() == right) moveTo(getX() + 1, getY());
		else if (getDirection() == up) moveTo(getX(), getY() + 1);
		else if (getDirection() == down) moveTo(getX(), getY() - 1);
		else cerr << "Pea's direction is none, cannot move it";

		getWorld()->tryToDamageLocation(this, getX(), getY());
	}
}

bool Pea::allowsAgentColocationBy(Actor* a) const { return true; }

  // returns if pea can be hit by pea
bool Pea::isHittable() const { return false; }

  // when attacked by pea, suffer no damage
void Pea::damageBy(int damageAmt) {}
//////////////////////////// PEA CLASS /////////////////////////

//////////////////////////// MARBLE CLASS /////////////////////////
Marble::Marble(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_MARBLE, x, y)
{
	setHP(10);
}

void Marble::doSomething() {}

  // avator may push a marble object (depending on position)
bool Marble::mayBePushedByPlayer() { return true; }

bool Marble::allowsAgentColocationBy(Actor* a) const
{
	return false; // not categorically, but depends on position
}

  // returns if Marble can be hit by pea
bool Marble::isHittable() const { return true; }

  // when attacked by pea, suffer damage
void Marble::damageBy(int damageAmt)
{
	setHP(getHP() - damageAmt);
	if (getHP() <= 0)
		setDead();
}

  // avator pushes Marble to given position if possible, returns false otherwise
bool Marble::pushTo(int x, int y)
{
	if (getWorld()->allowsMarble(x, y)) {
		moveTo(x, y);
		return true; // marble was moved
	}
	return false; // marble could not be moved
}

//////////////////////////// MARBLE CLASS /////////////////////////

//////////////////////////// PIT CLASS /////////////////////////
Pit::Pit(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_PIT, x, y)
{}


void Pit::doSomething()
{
	if (!isAlive()) return;

	Actor* ptr = getWorld()->isMarbleHere(this, getX(), getY());

	if (ptr != nullptr)
	{
		setDead();
		ptr->setDead();
	}
}

  // only returns true for Pit or Empty
bool Pit::canContainMarblePush() { return true; }

bool Pit::allowsAgentColocationBy(Actor* a) const
{
	return true; // CHANGE - just for marble and pea? any others?
}

  // returns if Pit can be hit by pea
bool Pit::isHittable() const { return false; }

  // when attacked by pea, suffer no damage
void Pit::damageBy(int damageAmt) {};
//////////////////////////// PIT CLASS /////////////////////////

//////////////////////////// PICKUPABLEITEM CLASS /////////////////////////
PickupableItem::PickupableItem(int x, int y, int ID, StudentWorld* ptr)
	: Actor(ptr, ID, x, y)
{}

void PickupableItem::doSomething()
{
	// check if alive
	if (!isAlive()) return;

	// if player is on same square as the pickupable item
	if (getWorld()->isPlayerHere(getX(), getY()))
	{
		specialized();
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
		
}

  // returns if Actor can be hit by pea
bool PickupableItem::isHittable() const { return false; }

  // when attacked by pea, suffer no damage
void PickupableItem::damageBy(int damageAmt) {};

bool PickupableItem::allowsAgentColocationBy(Actor* a) const
{
	return true; // FIX
}
//////////////////////////// PICKUPABLEITEM CLASS /////////////////////////

//////////////////////////// CRYSTAL CLASS /////////////////////////
Crystal::Crystal(int x, int y, StudentWorld* ptr)
	: PickupableItem(x, y, IID_CRYSTAL, ptr)
{}

void Crystal::specialized()
{
	getWorld()->increaseScore(50);
}
//////////////////////////// CRYSTAL CLASS /////////////////////////