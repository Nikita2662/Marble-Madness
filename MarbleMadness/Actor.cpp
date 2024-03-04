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
  // only true for Crystal
bool Actor::isCrystal() const { return false;  }
  // returns true if actor can be stolen by ThiefBot (only true for goodies)
bool Actor::isStealable() const { return false; }
void Actor::setDead() { alive = false; }
void Actor::setHP(int amt) { hitPoints = amt; }
int Actor::getHP() const { return hitPoints; }
  // only returns true for Pit or Empty
bool Actor::canContainMarblePush() { return false; }
  // returns if this kind of Actor could possibly be pushed by an Avator (only true for marble)
bool Actor::mayBePushedByPlayer() { return false; }
  // only allowed for Marbles
bool Actor::pushTo(int x, int y) { return false; }
void Actor::changeStealable() {}
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
int Avator::getHealth() const { return 100 * (getHP() / 20.); }

void Avator::incAmmo(int amt) { if (amt > 0) numPeas += amt; }

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

//////////////////////////// EXIT CLASS /////////////////////////
Exit::Exit(int x, int y, StudentWorld* ptr)
	: Actor(ptr, IID_EXIT, x, y)
{
	setVisible(false);
}

void Exit::doSomething()
{
	// if player is currently on same square as exit and exit is visible (all crystals collected)
	if (isVisible() && getWorld()->isPlayerHere(getX(), getY()))
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000);
		getWorld()->completeLevel();
	}

	// if all crystals collected and exit invisible, TRANSITION to visible
	else if (!isVisible() && getWorld()->allCrystalsCollected())
	{
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
	}
}
bool Exit::allowsAgentColocationBy(Actor* a) const
{
	return true; // FIX
}

// returns if Exit can be hit by pea
bool Exit::isHittable() const { return false; }
// when attacked by pea, suffer no damage
void Exit::damageBy(int damageAmt) {};
//////////////////////////// EXIT CLASS /////////////////////////

//////////////////////////// PICKUPABLEITEM CLASS /////////////////////////
PickupableItem::PickupableItem(int x, int y, int ID, StudentWorld* ptr)
	: Actor(ptr, ID, x, y), canPickUp(true)
{}

  // returns true if actor can be stolen by ThiefBot
bool PickupableItem::isStealable() const
{
	if (!isCrystal()) 
		return canPickUp;
	return false;
}

void PickupableItem::changeStealable() 
{ 
	if (canPickUp) canPickUp = false;
	else canPickUp = true;
}

void PickupableItem::doSomething()
{
	// check if alive
	if (!isAlive()) return;

	// if player is on same square as the pickupable item, pick it up
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
	return canPickUp; // FIX
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

  // only true for Crystal
bool Crystal::isCrystal() const { return true; }
//////////////////////////// CRYSTAL CLASS /////////////////////////

//////////////////////////// EXTRALIFEGOODIE CLASS /////////////////////////
ExtraLifeGoodie::ExtraLifeGoodie(int x, int y, StudentWorld* ptr)
	: PickupableItem(x, y, IID_EXTRA_LIFE, ptr)
{}

void ExtraLifeGoodie::specialized()
{
	getWorld()->increaseScore(1000);
	getWorld()->incLives();
}
//////////////////////////// EXTRALIFEGOODIE CLASS /////////////////////////

//////////////////////////// RESTOREHEALTHGOODIE CLASS /////////////////////////
RestoreHealthGoodie::RestoreHealthGoodie(int x, int y, StudentWorld* ptr)
	: PickupableItem(x, y, IID_RESTORE_HEALTH, ptr)
{}

void RestoreHealthGoodie::specialized()
{
	getWorld()->increaseScore(500);
	getWorld()->restorePlayerToFullHealth();
}
//////////////////////////// RESTOREHEALTHGOODIE CLASS /////////////////////////

//////////////////////////// AMMMOGOODIE CLASS /////////////////////////
AmmoGoodie::AmmoGoodie(int x, int y, StudentWorld* ptr)
	: PickupableItem(x, y, IID_AMMO, ptr)
{}

void AmmoGoodie::specialized()
{
	getWorld()->increaseScore(100);
	getWorld()->addAmmoToPlayer(20);
}
//////////////////////////// AMMMOGOODIE CLASS /////////////////////////

//////////////////////////// ROBOT CLASS /////////////////////////
Robot::Robot(int x, int y, int ID, int dir, StudentWorld* ptr)
	: Actor(ptr, ID, x, y, dir), tickCount(0), numTicks(-1)
{}

void Robot::doSomething()
{
	if (!isAlive()) return;

	tickCount++; 
	if (tickCount % numTicks != 0) return; // rest
	
	act(); // otherwise, act - specialized
}

void Robot::determineNumTicks()
{
	numTicks = (28 - getWorld()->getLevel()) / 4;
	if (numTicks < 3) numTicks = 3; // no Robot moves faster than this
}

int Robot::getTickCount() { return tickCount; }
int Robot::getNumTicks() { return numTicks; }

bool Robot::allowsAgentColocationBy(Actor* a) const { return false; }
  // returns if Actor can be hit by pea
bool Robot::isHittable() const { return true; }
  // when attacked by pea, suffer damage
void Robot::damageBy(int damageAmt) 
{ 
	setHP(getHP() - damageAmt);
	if (getHP() > 0)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else // dead
	{
		damageSpecial();
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
	}
}
//////////////////////////// ROBOT CLASS /////////////////////////

//////////////////////////// THIEFBOT CLASS /////////////////////////
ThiefBot::ThiefBot(int x, int y, int ID, StudentWorld* ptr)
	: Robot(x, y, ID, right, ptr), holding(false), p(nullptr), distMoved(0)
{
	setHP(5);
	distanceBeforeTurning = rand() % 6 + 1;
	determineNumTicks();
}

void ThiefBot::act()
{
	// if on same square as goodie + does not hold goodie currently
	p = getWorld()->isGoodieHere(this, getX(), getY());
	if (p != nullptr && holding == false && (rand() % 10 + 1) == 4) { // 1 in 10 chance TB picks goodie up
		p->setVisible(false);
		p->changeStealable();
		holding = true;
		getWorld()->playSound(SOUND_ROBOT_MUNCH);
		return;
	}
	// hasn't moved full possible distance in this direction
	else if (distMoved < distanceBeforeTurning)
	{
		double botX = getX();
		double botY = getY();
		int botDir = getDirection();

		if (botDir == left && getWorld()->allowsBot(botX - 1, botY))
		{
			moveTo(botX - 1, botY);
			distMoved++;
			return;
		}
		else if (botDir == right && getWorld()->allowsBot(botX + 1, botY))
		{
			moveTo(botX + 1, botY);
			distMoved++;
			return;
		}
		else if (botDir == up && getWorld()->allowsBot(botX, botY + 1))
		{
			moveTo(botX, botY + 1);
			distMoved++;
			return;
		}
		else if (botDir == down && getWorld()->allowsBot(botX, botY - 1))
		{
			moveTo(botX, botY - 1);
			distMoved++;
			return;
		}
	}
	// already moved the full distance in straight line or encountered obstruction
	distanceBeforeTurning = rand() % 6 + 1;
	int d = (rand() % 4) * 90;
	int dx, dy;

	for (int i = 0; i < 4; i++) { // iterate through all 4 dirs
		if (d == 0) { dx = 1; dy = 0; }
		else if (d == 180) { dx = -1; dy = 0; }
		else if (d == 90) { dx = 0; dy = 1; }
		else { dx = 0; dy = -1; }

		if (getWorld()->allowsBot(getX() + dx, getY() + dy)) {
			setDirection(d);
			moveTo(getX() + dx, getY() + dy);
			return;
		}
		
		// change dir
		if (d < 360) d += 90;
		else d = 0;
	}

	// obstruction in all 4 directions
	setDirection(d);
	return;
}

void ThiefBot::damageSpecial()
{
	// drop goodie
	p->moveTo(getX(), getY());
	p->setVisible(true);
	p->changeStealable();

	getWorld()->increaseScore(10);
}
//////////////////////////// THIEFBOT CLASS /////////////////////////

//////////////////////////// MEANTHIEFBOT CLASS /////////////////////////
MeanThiefBot::MeanThiefBot(int x, int y, StudentWorld* ptr)
	: ThiefBot(x, y, IID_MEAN_THIEFBOT, ptr)
{
	setHP(8);
}

void MeanThiefBot::act()
{
	double playerX = getWorld()->getPlayer()->getX();
	double playerY = getWorld()->getPlayer()->getY();
	double botX = getX();
	double botY = getY();
	int botDir = getDirection();

	// if should fire pea cannon
	if (botDir == right && botY == playerY && playerX > botX && getWorld()->existsClearShotToPlayer(botX, botY, 1, 0)
		|| botDir == left && botY == playerY && playerX < botX && getWorld()->existsClearShotToPlayer(botX, botY, -1, 0)
		|| botDir == up && botX == playerX && playerY > botY && getWorld()->existsClearShotToPlayer(botX, botY, 0, 1)
		|| botDir == down && botX == playerX && playerY < botY && getWorld()->existsClearShotToPlayer(botX, botY, 0, -1))

	{
		// create new pea with temp location of bot
		Pea* pea = new Pea(botX, botY, getWorld(), botDir);

		// adjust position to directly in front of bot
		if (botDir == left) pea->moveTo(botX - 1, botY);
		else if (botDir == right) pea->moveTo(botX + 1, botY);
		else if (botDir == up) pea->moveTo(botX, botY + 1);
		else if (botDir == down) pea->moveTo(botX, botY - 1);
		else cerr << "bot's direction is none, cannot fire pea";

		getWorld()->addActor(pea); // add to actors array
		getWorld()->playSound(SOUND_ENEMY_FIRE); // sound
		return;
	}
	else ThiefBot::act();
}

void MeanThiefBot::damageSpecial()
{
	ThiefBot::damageSpecial();
	getWorld()->increaseScore(10); // increase score a total of 20 points
}
//////////////////////////// MEANTHIEFBOT CLASS /////////////////////////

//////////////////////////// RAGEBOT CLASS /////////////////////////
RageBot::RageBot(int x, int y, int dir, StudentWorld* ptr)
	: Robot(x, y, IID_RAGEBOT, dir, ptr)
{
	setHP(10);
	determineNumTicks();
}

void RageBot::damageSpecial()
{
	getWorld()->increaseScore(100);
}

void RageBot::act()
{
	double playerX = getWorld()->getPlayer()->getX();
	double playerY = getWorld()->getPlayer()->getY();
	double botX = getX();
	double botY = getY();
	int botDir = getDirection();

	// if should fire pea cannon
	if (botDir == right && botY == playerY && playerX > botX && getWorld()->existsClearShotToPlayer(botX, botY, 1, 0)
		|| botDir == left && botY == playerY && playerX < botX && getWorld()->existsClearShotToPlayer(botX, botY, -1, 0)
		|| botDir == up && botX == playerX && playerY > botY && getWorld()->existsClearShotToPlayer(botX, botY, 0, 1)
		|| botDir == down && botX == playerX && playerY < botY && getWorld()->existsClearShotToPlayer(botX, botY, 0, -1))

	{
		// create new pea with temp location of bot
		Pea* p = new Pea(botX, botY, getWorld(), botDir);

		// adjust position to directly in front of bot
		if (botDir == left) p->moveTo(botX - 1, botY);
		else if (botDir == right) p->moveTo(botX + 1, botY);
		else if (botDir == up) p->moveTo(botX, botY + 1);
		else if (botDir == down) p->moveTo(botX, botY - 1);
		else cerr << "bot's direction is none, cannot fire pea";

		getWorld()->addActor(p); // add to actors array
		getWorld()->playSound(SOUND_ENEMY_FIRE); // sound

		return;
	}
	else // didn't fire, tries to move
	{
		if (botDir == left && getWorld()->allowsBot(botX - 1, botY))
			moveTo(botX - 1, botY);
		else if (botDir == right && getWorld()->allowsBot(botX + 1, botY))
			moveTo(botX + 1, botY);
		else if (botDir == up && getWorld()->allowsBot(botX, botY + 1))
			moveTo(botX, botY + 1);
		else if (botDir == down && getWorld()->allowsBot(botX, botY - 1))
			moveTo(botX, botY - 1);
		else
			setDirection(botDir - 180);
	}
			
}
//////////////////////////// RAGEBOT CLASS /////////////////////////

//////////////////////////// THIEFBOTFACTORY CLASS /////////////////////////
ThiefBotFactory::ThiefBotFactory(int x, int y, StudentWorld* ptr, bool mean)
	: Actor(ptr, IID_ROBOT_FACTORY, x, y), ifMean(mean)
{}

void ThiefBotFactory::doSomething()
{
	
}

bool ThiefBotFactory::allowsAgentColocationBy(Actor* a) const { return false; }

  // returns if Actor can be hit by pea
bool ThiefBotFactory::isHittable() const { return true; }
  // when attacked by pea, suffer no damage
void ThiefBotFactory::damageBy(int damageAmt) {}

//////////////////////////// THIEFBOTFACTORY CLASS /////////////////////////